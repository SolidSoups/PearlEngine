#pragma once

#include "Message.h"
#include "ecs_common.h"

class CreateMeshMessage : public Message {
public:
  std::string filePath;
  ecs::Entity renderCompEntity;
};
