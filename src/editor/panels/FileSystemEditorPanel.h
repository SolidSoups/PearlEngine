#pragma once

#include <vector>

#include "EditorPanel.h"
#include "MenuRegistry.h"
#include "FileSystem.h"
#include "Logger.h"

class FileSystemEditorPanel : public EditorPanel{
public:
  FileSystemEditorPanel() 
    : EditorPanel("FileSystem") {
    MenuRegistry::Get().Register("Windows/File System", &m_IsOpen);
    m_Files = FileSystem::getAllFiles();
  }
  void OnImGuiRender() override;

private:
  void OpenContextMenu();
  std::vector<FileSystem::FileDescriptor> m_Files;
  int m_SelectedIndex = -1;
};
