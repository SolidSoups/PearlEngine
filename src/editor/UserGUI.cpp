#include "UserGUI.h"
#include "IPopupDialog.h"
#include "Logger.h"

#include "MeshPopupDialog.h"
#include "TexturePopupDialog.h"

IPopupDialog* UserGUI::m_CurrentDialog = nullptr;

void UserGUI::StartMeshPopup(std::function<void(std::shared_ptr<Mesh>)> callback){
  if(!m_CurrentDialog)
    m_CurrentDialog = new MeshPopupDialog(callback);
  else
    LOG_WARNING << "Cannot start more than one dialog at a time";
}

void UserGUI::StartTexturePopup(std::function<void(std::shared_ptr<TextureData>)> callback){
  if(!m_CurrentDialog)
    m_CurrentDialog = new TexturePopupDialog(callback);
  else
    LOG_WARNING << "Cannot start more than one dialog at a time";
}

void UserGUI::Render(){
  if(!m_CurrentDialog) return;

  if(!m_CurrentDialog->NeedsDestroy()){
    m_CurrentDialog->Draw();
  }
  else{
    delete m_CurrentDialog;
    m_CurrentDialog = nullptr;
  }
}

void UserGUI::Destroy(){
  if(m_CurrentDialog)
    delete m_CurrentDialog;
}
