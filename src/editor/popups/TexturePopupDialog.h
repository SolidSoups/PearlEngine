#pragma once

#include <memory>
#include <functional>

#include "IPopupDialog.h"
#include "FileSystem.h"
#include "TextureData.h"

class TexturePopupDialog : public IPopupDialog {
private:
  std::function<void(std::shared_ptr<TextureData>)> m_Callback;
  std::vector<FileSystem::FileDescriptor> m_FoundFiles;

public:
  TexturePopupDialog(std::function<void(std::shared_ptr<TextureData>)> callback);
  void Draw() override;
};
