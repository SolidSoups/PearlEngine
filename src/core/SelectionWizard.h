#pragma once
#include "ecs_common.h"

class SelectionWizard {
public:
  SelectionWizard() = delete;

  static void Init() { s_SelectedEntity = ecs::NULL_ENTITY; }
  static void Set(ecs::Entity entity) { s_SelectedEntity = entity; }
  static ecs::Entity Get() { return s_SelectedEntity; }
  static void Clear() { s_SelectedEntity = ecs::NULL_ENTITY; }
  static bool HasSelection() { return s_SelectedEntity != ecs::NULL_ENTITY; }

private:
  static inline ecs::Entity s_SelectedEntity = ecs::NULL_ENTITY;
};
