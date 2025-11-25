
// src
#include "PearlEngine.h"

#include "Project.h"

int main() {
    PearlEngine engine;
    if (!engine.IsInitialized()) {
        return -1;
    }

    engine.RunUpdateLoop();
    return 0;
}
