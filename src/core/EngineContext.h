#pragma once

#include "Scene.h"
#include <exception>
#include <stdexcept>
#include <utility>

class EngineContext{
public:
  static Scene& GetScene() { return m_Instance->m_Scene; }

public:
  EngineContext(Scene& scene)
   : m_Scene(scene){
    if(!m_Instance)
      m_Instance = this;
    else
      throw new std::runtime_error("EngineContext is already constructed!");
  }

private:
  static EngineContext* m_Instance;
  Scene& m_Scene;
};
