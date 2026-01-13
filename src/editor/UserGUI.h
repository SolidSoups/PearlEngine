#pragma once

#include <functional>
#include <memory>
#include <queue>


#include "Mesh.h"
#include "IPopupDialog.h"
#include "TextureData.h"

class UserGUI {
private:
  static IPopupDialog* m_CurrentDialog;

public:
  static void StartMeshPopup(std::function<void(std::shared_ptr<Mesh>)> callback);
  static void StartTexturePopup(std::function<void(std::shared_ptr<TextureData>)> callback);
  static void Render();
  static void Destroy();
};
