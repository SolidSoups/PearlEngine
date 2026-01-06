#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Message.h"
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
  Selection_GameObject,

  Selection_MAX
};

struct SelectionData {
  SelectionType type;
  std::variant<
    std::monostate,
    GameObject*,
    Camera*> selection;
  template<typename T>
  T* SelectionAs(){
    if(auto** ptr = std::get_if<T*>(&selection)){
      return *ptr;
    }
    return nullptr;
  }
};
struct SelectionMessage {
  SelectionData* data;
};

class SelectionWizard {
public:
  SelectionWizard(ServiceLocator* locator) 
    : r_MessageQueue(locator){}
private:
  SelectionData m_SelectionData;
  ServiceHandle<MessageQueue> r_MessageQueue;

public:
  SelectionData& GetSelectionData() { return m_SelectionData; } 
  template<typename T> T* GetSelectedObject() {
    return m_SelectionData.SelectionAs<T>();
  }

public:
  template<typename T>
  void SetSelection(SelectionType type, T* selectionObject) {
    LOG_INFO << "Setting selection for " << typeid(T).name();
    m_SelectionData.selection = selectionObject;
    m_SelectionData.type = type;
    SendMessage();
  };
  void ClearSelection(){
    m_SelectionData.selection = std::monostate{};
    m_SelectionData.type = Selection_Clear;
    SendMessage();
  }
  
private:
  void SendMessage(){
    r_MessageQueue->Dispatch(SelectionMessage{&m_SelectionData});
  }
};
