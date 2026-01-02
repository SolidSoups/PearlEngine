#pragma once

#include "Scene.h"
#include <exception>
#include <stdexcept>
#include <utility>

class EngineContext{
public:
  Scene& GetScene() const { return m_Scene; }

public:
  static EngineContext* Instance;
  EngineContext(Scene& scene)
   : m_Scene(scene){
    if(!Instance)
      Instance = this;
    else
      throw new std::runtime_error("EngineContext is already constructed!");
  }

private:
  Scene& m_Scene;
};
