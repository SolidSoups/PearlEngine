#pragma once
#include <any>
#include <cstdint>

enum SelectionType : uint8_t {
  Selection_None = 0,

  Selection_GameObject,

  Selection_MAX
};
struct SelectionData {
  SelectionType type;
  std::any &sample;
  SelectionData &operator=(SelectionData &&other) {
    this->type = other.type;
    this->sample = other.sample;
    return *this;
  }
};
struct SelectionDataMessage {
  SelectionData* data;
};

class SelectionWizard {
public:
  SelectionWizard() = default;
private:
  SelectionData* m_SelectionData=nullptr;

public:
  void SetSelection(SelectionType type, std::any &selectionObject) {
    m_SelectionData = new SelectionData{type, selectionObject};
  };
};
