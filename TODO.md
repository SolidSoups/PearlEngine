# PearlEngine Resource System TODO

## Overview
This document tracks planned improvements to the resource system based on a comprehensive analysis conducted on 2025-11-11.

---

## High Priority

### 1. Implement Resource Caching (Path-Based)
**Status:** Not Started
**Files:** `src/resource/resources/TextureData.cpp`, `src/resource/resources/ShaderData.cpp`

**Problem:**
Currently, calling `LoadTexture("brick.png")` multiple times loads the same file from disk repeatedly, creates duplicate GPU resources, and wastes VRAM. For example, 100 objects using the same texture would create 100 copies in GPU memory instead of sharing one.

**Solution:**
Add a `std::unordered_map<std::string, TextureHandle>` cache that checks if a resource is already loaded before reading from disk. Return the existing handle if found, otherwise load and cache it.

**Benefits:**
- Faster loading times (disk I/O only once per unique file)
- Reduced VRAM usage (shared GPU resources)
- Enables hot-reloading and better debugging
- Prevents accidental resource duplication bugs

**Estimated Impact:** High - Critical for scalability and performance

---

### 2. Implement Reference Counting for Safe Resource Sharing
**Status:** Not Started
**Files:** `src/resource/Handle.h`, `src/resource/ResourceManager.h`

**Problem:**
There's no tracking of how many objects are using a resource. Destroying a texture that's still referenced by a material causes undefined behavior. No way to automatically free unused resources.

**Solution:**
Add reference counting to handles or implement an intrusive reference counting system. Track how many handles point to each resource. Only allow destruction when refcount reaches zero, or implement automatic cleanup.

**Benefits:**
- Safe resource sharing between multiple objects
- Automatic cleanup of unused resources
- Prevents use-after-free bugs
- Enables smart resource management

**Estimated Impact:** High - Essential for safety and automatic resource management

---

### 3. Complete Material Destruction Logic
**Status:** Not Started
**File:** `src/resource/resources/MaterialData.cpp:73-76`

**Problem:**
`DestroyMaterial()` currently does nothing except remove from ResourceManager (comment: "nothing for now..."). No cleanup of material properties or validation that referenced textures/shaders are still valid.

**Solution:**
Implement proper cleanup:
- Clear all property maps (floats, vec3s, vec4s, ints, textureHandles, mat4s)
- Consider decrementing reference counts on owned resources (once ref counting is implemented)
- Validate that material is not currently bound
- Add any necessary OpenGL state cleanup

**Benefits:**
- Proper resource lifecycle management
- Prevents memory leaks in material property maps
- Cleaner shutdown process

**Estimated Impact:** Medium - Important for correctness and completeness

---

## Medium Priority

### 4. Cache Uniform Locations for Shader Performance
**Status:** Not Started
**File:** `src/resource/resources/ShaderData.cpp:33-54` (see TODO comment)

**Problem:**
Currently, every call to `ShaderSetFloat()`, `ShaderSetVec3()`, etc. calls `glGetUniformLocation()`, which queries OpenGL for the uniform's location by string name. This is slow and happens repeatedly for the same uniforms every frame.

**Solution:**
Add a `std::unordered_map<std::string, GLint>` to `ShaderData` that caches uniform locations after first lookup. Modify all `ShaderSet*()` functions to check cache first.

```cpp
struct ShaderData {
  GLuint id = 0;
  std::unordered_map<std::string, GLint> uniformLocationCache;
};
```

**Benefits:**
- Significant performance improvement (uniform sets happen every frame)
- Reduces OpenGL API calls
- Industry-standard optimization

**Estimated Impact:** Medium-High - Important for rendering performance

---

### 5. Add Validation Checks (Null Handle Detection)
**Status:** Not Started
**Files:** Multiple resource files

**Problem:**
Missing validation in several places:
- `MaterialData.cpp:17` - `CreateMaterial()` doesn't validate shader handle
- No checks that handles are valid before using them in many operations
- `UnbindMaterial()` declared but not implemented (`MaterialData.h:30`)

**Solution:**
- Add `Handle<T>::IsValid()` checks before all resource operations
- Implement `UnbindMaterial()` function
- Add shader handle validation in `CreateMaterial()`
- Add assertions/error messages for invalid handles
- Consider adding debug-only validation layers

**Benefits:**
- Easier debugging (clear error messages instead of crashes)
- Prevents hard-to-track bugs
- Better API safety

**Estimated Impact:** Medium - Important for developer experience and stability

---

## Low Priority / Future

### 6. Add Thread-Safety Enhancements
**Status:** Not Started
**Files:** `src/resource/ResourceManager.h`, `src/resource/HandleAllocator.h`

**Problem:**
The entire resource system has no thread safety:
- No mutexes protecting ResourceManager operations
- HandleAllocator's free list and generation counters are not thread-safe
- Concurrent Create/Destroy operations would cause data races and corruption

**Current Assumption:** Single-threaded use only (all resources created/destroyed on main thread)

**Solution (if multi-threading is needed):**
- Add `std::mutex` to ResourceManager for map access
- Add `std::mutex` to HandleAllocator for allocator operations
- Consider `std::atomic` for generation numbers
- Implement lock-free resource pools if high-performance multi-threaded loading is required
- Document thread-safety guarantees in API

**Benefits:**
- Enable background resource loading
- Support multi-threaded asset streaming
- Safer for future engine expansion

**Estimated Impact:** Low - Only needed if multi-threading is planned

**Note:** This is intentionally low priority. Only implement if you plan to:
- Load resources from background threads
- Support multi-threaded rendering
- Build an asset streaming system

---

## Additional Findings from Analysis

### Current System Strengths
- ✅ Clean handle-based architecture with type safety
- ✅ Generation-based validation prevents use-after-free
- ✅ Proper GPU resource cleanup (glDeleteTextures/glDeleteProgram)
- ✅ Ordered destruction (Materials → Textures → Shaders)
- ✅ Memory-efficient handles (8 bytes each)

### Memory Footprint
- TextureData: 16 bytes + GPU VRAM
- ShaderData: 4 bytes + GPU memory
- MaterialData: ~304 bytes base + property entries
- Handles: 8 bytes each

### Key Architecture Files
- `src/resource/Handle.h` - Handle structure and type safety
- `src/resource/HandleAllocator.h` - ID pooling with generation numbers
- `src/resource/ResourceManager.h` - Generic resource container
- `src/resource/ResourceSystem.h` - Central singleton registry

---

## Implementation Order Recommendation

1. **Resource Caching** - Immediate performance/correctness wins
2. **Uniform Location Caching** - Easy performance improvement
3. **Validation Checks** - Quick safety improvements
4. **Reference Counting** - More complex, enables advanced features
5. **Material Destruction** - Cleanup and polish
6. **Thread Safety** - Only if needed for future features

---

**Last Updated:** 2025-11-11
**Analysis Location:** Full analysis available in conversation history
