# Sphere vs Terrain Collision

## Overview

The terrain has no rigid body — it is static. The sphere is dynamic (has `RigidBodyComponent`).
The collision test samples the terrain heightmap CPU-side every physics tick to find the ground
height under the sphere, then resolves penetration using the existing `Resolve()` infrastructure.

---

## Step 1 — Cache pixel data on TerrainComponent

`getPixelData()` calls `stbi_load` from disk. Calling it every physics tick would be very slow.
Cache the result on the component when the terrain is generated.

**`TerrainComponent.h`**
```cpp
#include "StbiImage.h"
#include <optional>

struct TerrainComponent {
    // ... existing fields ...
    std::optional<StbiImage> pixelCache;  // ADD — populated by TerrainSystem
};
```

**`TerrainSystem.cpp`** — inside `generateTerrain()`, after the existing `auto img = ...` call:
```cpp
auto img = aTerrain.heightMap->getPixelData(1);
if (!img) { /* ... existing error ... */ return; }

aTerrain.pixelCache = std::move(*img);  // ADD
```

---

## Step 2 — Give PhysicsSystem a pointer to TerrainSystem

PhysicsSystem needs to iterate terrain entities. Since terrain entities are not in
PhysicsSystem's `Entities` set (they have no `RigidBodyComponent`), the pointer to
`TerrainSystem` gives access to its `Entities` set. `Get<T>(entity)` already goes through
the coordinator so it works for any entity.

**`PhysicsSystem.h`**
```cpp
#include "TerrainSystem.h"
#include "TerrainComponent.h"

class PhysicsSystem : public ecs::System {
    // ...
    TerrainSystem* mTerrainSystem = nullptr;  // ADD

public:
    // change Init signature, or add a setter:
    void SetTerrainSystem(TerrainSystem* ts) { mTerrainSystem = ts; }  // ADD

private:
    void CheckSpheresAgainstTerrains();  // ADD
    void TestSphereTerrain(
        TransformComponent& sphereTf, SphereColliderComponent& sphere,
        RigidBodyComponent* rb, ecs::Entity sphereEntity,
        TransformComponent& terrainTf, TerrainComponent& terrain,
        ecs::Entity terrainEntity);  // ADD
};
```

**`Scene.cpp`** — after both systems are registered (end of `Scene::Init()`):
```cpp
mPhysicsSystem->SetTerrainSystem(mTerrainSystem.get());  // ADD
```

---

## Step 3 — Implement the height query

Height sampling mirrors what `TerrainSystem::generateVertices` does.
Terrain vertices span **−0.5 to +0.5** in local XZ before scale/position is applied.

```
u = (sphereCenter.x - terrainTf.position.x) / terrainTf.scale.x + 0.5
v = (sphereCenter.z - terrainTf.position.z) / terrainTf.scale.z + 0.5
```

If u or v is outside [0, 1] the sphere is not over the terrain — skip.

```
px = clamp(u * (width  - 1), 0, width  - 1)
py = clamp(v * (height - 1), 0, height - 1)
heightNorm = pixelCache.data[px + py * width] / 255.0f
worldTerrainY = terrainTf.position.y + heightNorm * terrainTf.scale.y
```

---

## Step 4 — Detect and resolve penetration

```
sphereBottom = sphereCenter.y - sphere.radius
penetration  = worldTerrainY - sphereBottom
```

If `penetration <= 0`, no collision — return early.

Otherwise call `Resolve()`:
```cpp
glm::vec3 normal = glm::vec3(0, -1, 0);  // points sphere → terrain (downward)
// Resolve pushes tf1 by -normal * penetration = upward
Resolve(sphereTf, rb, terrainTf, nullptr, normal, penetration,
        sphereEntity, terrainEntity);
```

`Resolve` already handles the `rb2 == nullptr` case — it only moves tf1 (the sphere).
The velocity impulse flips the downward component using the existing restitution logic.

---

## Step 5 — Hook into UpdatePhysics

**`PhysicsSystem.cpp`** — `CheckSpheresAgainstTerrains()` implementation:
```cpp
void PhysicsSystem::CheckSpheresAgainstTerrains() {
    if (!mTerrainSystem) return;

    for (ecs::Entity physEnt : Entities) {
        auto* sphere = TryGet<SphereColliderComponent>(physEnt);
        auto* rb     = TryGet<RigidBodyComponent>(physEnt);
        if (!sphere || !rb) continue;

        auto& sphereTf = Get<TransformComponent>(physEnt);

        for (ecs::Entity terrEnt : mTerrainSystem->Entities) {
            auto& terrainTf = Get<TransformComponent>(terrEnt);
            auto& terrain   = Get<TerrainComponent>(terrEnt);
            if (!terrain.pixelCache) continue;

            TestSphereTerrain(sphereTf, *sphere, rb, physEnt,
                              terrainTf, terrain, terrEnt);
        }
    }
}
```

Call it at the end of `UpdatePhysics()`, after `FlushCollisions()`:
```cpp
void PhysicsSystem::UpdatePhysics(float timestep) {
    // ... existing integrate + CheckCollision + FlushCollisions ...
    CheckSpheresAgainstTerrains();  // ADD
}
```

---

## Data flow summary

```
UpdatePhysics()
  └─ CheckSpheresAgainstTerrains()
       └─ for each (sphere entity) × (terrain entity)
            └─ TestSphereTerrain()
                 ├─ sample pixelCache at sphere XZ → worldTerrainY
                 ├─ penetration = worldTerrainY - (sphere.y - radius)
                 └─ Resolve(sphere, rb, terrain, nullptr, (0,-1,0), penetration)
                      └─ position correction: sphere.y += penetration
                      └─ velocity impulse:   sphere.vy flipped by restitution
```

---

## Limitations (known, acceptable for now)

- Single-point sample — no averaging over the sphere footprint. Small resolution terrains
  may let the sphere clip through between samples.
- No slope normal — collision response is always straight up `(0,1,0)`. The sphere won't
  roll or slide realistically on steep slopes.
- No `OnCollisionEnter` event dispatched for sphere-terrain hits (unlike the entity-entity
  path through `RegisterCollision`). Add a `RegisterCollision` call in `TestSphereTerrain`
  if scripting needs it.
