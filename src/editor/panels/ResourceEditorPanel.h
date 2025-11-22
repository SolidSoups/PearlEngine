#pragma once

#include <string>
#include <functional>
#include <vector>

#include "EditorPanel.h"
#include "ResourceCommon.h"

struct ResourceData{
  std::string name;
  size_t loaded;
  size_t allocated;
  size_t freed;
};

class ResourceEditorPanel : public EditorPanel {
public:
  ResourceEditorPanel(ResourceSystem& resourceSystem);

  void OnImGuiRender() override;

private:
  ResourceSystem& m_ResourceSystem;
  std::vector<std::function<ResourceData()>> m_ResourceGetters;
  std::vector<ResourceData> m_CachedResourceData;
  void RegisterResource(std::function<ResourceData()> getter){
    m_ResourceGetters.push_back(getter);
  }

  // seconds
  float m_LastUpdateTime;
  const float c_UpdateTime = 0.5f;

  void UpdateResourceData();
  void DrawResourceTable();
};
