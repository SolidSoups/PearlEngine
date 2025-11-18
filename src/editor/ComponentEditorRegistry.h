#pragma once
#include <unordered_map>

#include <typeindex>
#include <memory>

#include "ComponentEditor.h"


class ComponentEditorRegistry{
public:
  static ComponentEditorRegistry& Get(){
    static ComponentEditorRegistry instance;
    return instance;
  }

  template<typename Component>
  void RegisterComponentEditor(std::unique_ptr<ComponentEditor> editor){
    m_Map[std::type_index(typeid(Component))] = std::move(editor);
  }

  template<typename Component>
  ComponentEditor* GetComponentEditor(){
    auto it = m_Map.find(std::type_index(typeid(Component)));
    if(it != m_Map.end()){
      return it->second.get();
    }
    return nullptr;
  }

  ComponentEditor* GetComponentEditorByTypeIndex(std::type_index typeIndex){
    auto it = m_Map.find(typeIndex);
    if(it != m_Map.end()){
      return it->second.get();
    }
    return nullptr;
  }

  template<typename Component>
  bool Has() const{
    return m_Map.find(std::type_index(typeid(Component))) != m_Map.end();
  }

private:
  std::unordered_map<std::type_index, std::unique_ptr<ComponentEditor>> m_Map;
};

#define GET_COMPONENT_EDITOR(type) \
  ComponentEditorRegistry::Get().GetComponentEditor<type>()
#define GET_COMPONENT_EDITOR_WITH_TYPEID(typeid) \
  ComponentEditorRegistry::Get().GetComponentEditorByTypeIndex(typeid);
#define REGISTER_COMPONENT_EDITOR(type, value) \
  ComponentEditorRegistry::Get().RegisterComponentEditor<type>(value)
#define STATIC_REGISTER_COMPONENT_EDITOR(ComponentType, EditorType) \
  namespace { \
    struct EditorType##Registrar { \
      EditorType##Registrar() { \
        ComponentEditorRegistry::Get().RegisterComponentEditor<ComponentType>( \
          std::make_unique<EditorType>() \
        ); \
      } \
    }; \
    static EditorType##Registrar s_##EditorType##Registrar; \
  }
