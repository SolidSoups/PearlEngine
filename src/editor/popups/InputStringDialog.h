#pragma once
#include "IPopupDialog.h"

#include <functional>
#include <string>

class InputStringDialog : public IPopupDialog {
private:
  std::function<void(const std::string&)> mCallback;
  std::string mInput;
  std::string mTitle;

public:
  InputStringDialog(const std::string& title, std::function<void(const std::string&)> callback);
  void Draw() override;
};
