#pragma once

#include <functional>
#include <string>
#include <vector>

#include "FileSystem.h"
#include "IPopupDialog.h"

class FilePopupDialog : public IPopupDialog{
private:
  std::function<void(const std::string&)> m_Callback;
  std::vector<FileSystem::FileDescriptor> m_FoundFiles;

public:
  FilePopupDialog(std::function<void(const std::string&)> callback, const std::vector<std::string>& extensions);
  void Draw() override;
};
