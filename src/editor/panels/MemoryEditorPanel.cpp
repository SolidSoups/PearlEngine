#include "MemoryEditorPanel.h"

#include <string>
#include <cstdio>

#include "imgui.h"

#include "ServiceLocator.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "memoryUsage.h"

std::string formatBytes(float bytes) {
  const char *units[] = {"B", "kB", "MB", "GB"};
  int unitIndex = 0;

  while (bytes >= 1024.0f && unitIndex < 3) {
    bytes /= 1024.0f;
    unitIndex++;
  }

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.2f %s", bytes, units[unitIndex]);
  return std::string(buffer);
}

void MemoryEditorPanel::OnImGuiRender() {
  if (!m_IsOpen)
    return;

  MeshManager &meshMng = ServiceLocator::Get<MeshManager>();
  TextureManager &texMng = ServiceLocator::Get<TextureManager>();
  ShaderManager &shdMng = ServiceLocator::Get<ShaderManager>();

  ImGui::Begin(m_Name.c_str());

  ImGui::Text("Memory checking");
  ImGui::Separator();

  size_t meshMemSize = meshMng.calcMemorySize();
  std::string formMeshBytes = formatBytes((float)meshMemSize);
  ImGui::Text("Mesh Size: %s", formMeshBytes.c_str());
  ImGui::Text("Mesh Count: %zu", meshMng.getCacheSize());
  ImGui::Separator();
  size_t texMemSize = texMng.calcMemorySize();
  std::string formTexBytes = formatBytes((float)texMemSize);
  ImGui::Text("Texture Size: %s", formTexBytes.c_str());
  ImGui::Text("Texture Count: %zu", texMng.getCacheSize());
  ImGui::Separator();
  size_t shaderMemSize = shdMng.calcMemorySize();
  std::string formShdBytes = formatBytes((float)shaderMemSize);
  ImGui::Text("Shader Size: %s", formShdBytes.c_str());
  ImGui::Text("Shader Count: %zu", shdMng.getCacheSize());
  ImGui::Separator();
  long procMem = getMemoryUsageMB();
  ImGui::Text("Total Program Memory: %ld MB", procMem);
  ImGui::End();
}
