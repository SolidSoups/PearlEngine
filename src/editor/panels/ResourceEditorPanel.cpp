#include "ResourceEditorPanel.h"

#include "MenuRegistry.h"
#include "Time.h"
#include "imgui.h"

#include "Mesh.h"
#include "MaterialData.h"
#include "ShaderData.h"
#include "TextureData.h"

ResourceEditorPanel::ResourceEditorPanel(ResourceSystem& resourceSystem)
    : EditorPanel("Resource Overview"), m_LastUpdateTime(0.0f),
      m_ResourceSystem(resourceSystem) {
  // Register all resource data getters
  RegisterResource([this]() {
    auto &mgr = m_ResourceSystem.GetManagerByData<TextureData>();
    auto &alloc = mgr.GetAllocator();
    return ResourceData{"Textures", mgr.GetResourceSize(),
                        alloc.GetTotalAllocations(), alloc.GetFreeIDCount()};
  });

  RegisterResource([this]() {
    auto &mgr = m_ResourceSystem.GetManagerByData<MaterialData>();
    auto &alloc = mgr.GetAllocator();
    return ResourceData{"Materials", mgr.GetResourceSize(),
                        alloc.GetTotalAllocations(), alloc.GetFreeIDCount()};
  });

  RegisterResource([this]() {
    auto &mgr = m_ResourceSystem.GetManagerByData<Mesh>();
    auto &alloc = mgr.GetAllocator();
    return ResourceData{"Meshes", mgr.GetResourceSize(),
                        alloc.GetTotalAllocations(), alloc.GetFreeIDCount()};
  });

  RegisterResource([this]() {
    auto &mgr = m_ResourceSystem.GetManagerByData<ShaderData>();
    auto &alloc = mgr.GetAllocator();
    return ResourceData{"Shaders", mgr.GetResourceSize(),
                        alloc.GetTotalAllocations(), alloc.GetFreeIDCount()};
  });

  // register menu item
  MenuRegistry::Get().Register("Windows/Resource Overview", &m_IsOpen);
}

void ResourceEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  if (Time::time - m_LastUpdateTime >= c_UpdateTime) {
    m_LastUpdateTime = Time::time;
    UpdateResourceData();
  }

  ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(ImVec2(250, 150),
                                      ImVec2(FLT_MAX, FLT_MAX));

  ImGui::Begin(m_Name.c_str(), &m_IsOpen);
  DrawResourceTable();
  ImGui::End();
}

void ResourceEditorPanel::DrawResourceTable() {
  constexpr ImGuiTableFlags tableFlags =
      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
  if (ImGui::BeginTable("ResourceStats", 4, tableFlags)) {
    ImGui::TableSetupColumn("Resource", ImGuiTableColumnFlags_WidthFixed,
                            120.0f);
    ImGui::TableSetupColumn("Loaded", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Allocated", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Freed", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    for (const auto &data : m_CachedResourceData) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%s", data.name.c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%zu", data.loaded);
      ImGui::TableNextColumn();
      ImGui::Text("%zu", data.allocated);
      ImGui::TableNextColumn();
      ImGui::Text("%zu", data.freed);
    }

    ImGui::EndTable();
  }
}

void ResourceEditorPanel::UpdateResourceData() {
  m_CachedResourceData.clear();
  for (auto &getter : m_ResourceGetters) {
    m_CachedResourceData.push_back(getter());
  }
}
