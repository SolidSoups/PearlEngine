#pragma once

#include <cstdint>
#include <bitset>

#include <glm/glm.hpp>

namespace ecs {
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

// Invalid entity constant
constexpr Entity NULL_ENTITY = 0;

// comp
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;
}

// global definitions
using Entity = ecs::Entity;
using EntityID = ecs::Entity;
constexpr Entity MAX_ENTITIES = ecs::MAX_ENTITIES;
using ComponentType = ecs::ComponentType;
constexpr ComponentType MAX_COMPONENTS = ecs::MAX_COMPONENTS;
using Signature = ecs::Signature;

// nhloan/json
