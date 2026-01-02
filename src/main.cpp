
// src
#include "PearlEngine.h"
#include <iostream>

int main() {
  PearlEngine engine;
  if (!engine.IsInitialized()) {
    return -1;
  }

  engine.RunUpdateLoop();
  return 0;
}
