#pragma once

class DebugConsole{

private:
  static DebugConsole& Get(){
    static DebugConsole instance;
    return instance;
  }
};
