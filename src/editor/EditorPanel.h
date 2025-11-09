#pragma once
#include <string>

class EditorPanel{
public:
  EditorPanel(const std::string& name)
    : m_Name(name)
    , m_IsOpen(true) {}
  virtual ~EditorPanel() = default;

public:
  virtual void OnImGuiRender() = 0;

public:
  bool IsOpen() const { return m_IsOpen; }
  void SetOpen(bool open) { m_IsOpen = open; }
  const std::string& GetName() const { return m_Name; }

protected:
  std::string m_Name;
  bool m_IsOpen;
};
