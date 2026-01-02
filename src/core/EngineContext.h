#pragma once

#include "MessageQueue.h"
#include "Scene.h"
#include "SelectionWizard.h"
#include <exception>
#include <stdexcept>
#include <utility>

class EngineContext{
public:
  static Scene& GetScene() { return m_Instance->m_Scene; }
  static MessageQueue& GetMessageQueue() { return m_Instance->m_MessageQueue; }
  static SelectionWizard& GetSelectionWizard() { return m_Instance->m_SelectionWizard; }

public:
  EngineContext(Scene& scene, MessageQueue& msgQueue)
   : m_Scene(scene), m_MessageQueue(msgQueue){
    if(!m_Instance)
      m_Instance = this;
    else
      throw new std::runtime_error("EngineContext is already constructed!");
  }

private:
  static EngineContext* m_Instance;

private: // references
  Scene& m_Scene;
  MessageQueue& m_MessageQueue;

private:
  SelectionWizard m_SelectionWizard{};
};
