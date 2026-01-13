#pragma once


class IPopupDialog {
private:
  bool m_Destroy = false;
protected:
  void DestroySelf() { m_Destroy = true; }
public:
  virtual ~IPopupDialog() = default;
  virtual void Draw() = 0;

public:
  bool NeedsDestroy() const { return m_Destroy; }
};
