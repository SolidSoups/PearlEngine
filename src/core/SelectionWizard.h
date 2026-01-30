#pragma once
#include "Camera.h"
#include "Message.h"
#include "ecs_common.h"
#include <typeindex>
#include "MessageQueue.h"
#include "ServiceHandle.h"
#include "ServiceLocator.h"
#include <any>
#include "Logger.h"
#include <cstdint>
#include <variant>

enum SelectionType : uint8_t {
  Selection_None = 0,

  Selection_Clear,

  // types of selections
  Selection_Entity,

  Selection_MAX
};

struct SelectionData {
  SelectionType type = Selection_None;
  ecs::Entity selectedEntity = ecs::NULL_ENTITY;

  bool HasSelection() const {
    return type == Selection_Entity && selectedEntity != ecs::NULL_ENTITY;
  }
};

struct SelectionMessage {
  SelectionType type;
  ecs::Entity selectedEntity;
};

class SelectionWizard {
public:
  SelectionWizard() = default;
private:
  SelectionData m_SelectionData;
  ServiceHandle<MessageQueue> r_MessageQueue;

public:
  SelectionData& GetSelectionData() { return m_SelectionData; }
  ecs::Entity GetSelectedEntity() const {
    return m_SelectionData.selectedEntity;
  }
  bool HasSelection() const {
    return m_SelectionData.HasSelection();
  }

public:
  void SetSelection(ecs::Entity entity) {
    m_SelectionData.selectedEntity = entity;
    m_SelectionData.type = Selection_Entity;
    SendMessage();
  };
  void ClearSelection(){
    m_SelectionData.selectedEntity = ecs::NULL_ENTITY;
    m_SelectionData.type = Selection_Clear;
    SendMessage();
  }

private:
  void SendMessage(){
    SelectionMessage msg{m_SelectionData.type, m_SelectionData.selectedEntity};
    r_MessageQueue->Dispatch(msg);
  }
};
