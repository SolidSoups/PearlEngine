#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include <string>


#include "Mesh.h"
#include "IPopupDialog.h"
#include "TextureData.h"

class UserGUI {
private:
  static IPopupDialog* m_CurrentDialog;

public:
  static void StartMeshPopup(std::function<void(std::shared_ptr<Mesh>)> callback);
  static void StartTexturePopup(std::function<void(std::shared_ptr<TextureData>)> callback);
  static void StartFilePopup(
    std::function<void(const std::string&)> callback,
    const std::vector<std::string>& extensions
  );
  static void StartInputPopup(const std::string& title, std::function<void(const std::string&)> callback);
  static bool DrawFile(std::string& filepath);
  static void Render();
  static void Destroy();
};
