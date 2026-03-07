#include "TerrainComponentEditor.h"

#include <imgui.h>

#include "Scene.h"
#include "ServiceLocator.h"
#include "TerrainSystem.h"

void TerrainComponentEditor::OnInit() {
  auto& scene = ServiceLocator::Get<Scene>();
  myTerrainSystem = scene.mTerrainSystem.get();
  if(!myTerrainSystem)
    LOG_ERROR << "Could not get terrain system";
}

void TerrainComponentEditor::OnDrawComponent(void* target, ecs::Entity entity){
  TerrainComponent* terrain = static_cast<TerrainComponent*>(target);
  if(!terrain) return;

  // ensure material exists
  if(!terrain->material && myTerrainSystem->getShader()){
    terrain->material = std::make_shared<Material>(myTerrainSystem->getShader());
  }
  
  // height map texture
  myHeightMap.renderImGui("Height Map");  
  if(myHeightMap.isDirty()){
    if(auto newTex = myHeightMap.create()){
      terrain->heightMap = newTex;
      terrain->material->setTexture("uHeightMap", newTex);

      // regenerate the terrain
      myTerrainSystem->generateTerrain(entity);
    }
  }

  // show resolution
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Resolution");
  ImGui::SameLine();
  ImGui::DragInt("##Resolution", &terrain->resolution);
}
