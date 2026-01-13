#pragma once

#include <memory>
#include <functional>

#include "IPopupDialog.h"
#include "FileSystem.h"
#include "Mesh.h"

class MeshPopupDialog : public IPopupDialog {
private:
  std::function<void(std::shared_ptr<Mesh>)> m_Callback;
  std::vector<FileSystem::FileDescriptor> m_FoundFiles;

public:
  MeshPopupDialog(std::function<void(std::shared_ptr<Mesh>)> callback);
  void Draw() override;
};
