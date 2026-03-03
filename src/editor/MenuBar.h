#pragma once

#include <memory>
#include "TextureData.h"
#include "RuntimeState.h"

class MenuBar{
public:
  void RenderMenuBar(RuntimeState runtimeState);

  void SetButtonCallbacks(
    std::function<void()>& onPlayClicked,
    std::function<void()>& onPauseClicked,
    std::function<void()>& onStopClicked,
    std::function<void()>& onReloadClicked,
    std::shared_ptr<TextureData>& playTex,
    std::shared_ptr<TextureData>& pauseTex,
    std::shared_ptr<TextureData>& stopTex,
    std::shared_ptr<TextureData>& reloadTex
  );

private:
  std::shared_ptr<TextureData> mPlayTex;
  std::shared_ptr<TextureData> mPauseTex;
  std::shared_ptr<TextureData> mStopTex;
  std::shared_ptr<TextureData> mReloadTex;
  std::function<void()> mOnPlayClicked;
  std::function<void()> mOnPauseClicked;
  std::function<void()> mOnStopClicked;
  std::function<void()> mOnReloadClicked;
};
