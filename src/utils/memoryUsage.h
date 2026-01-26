#include <fstream>
#include <string>

// TODO: wrap in #ifdef __linux__

inline long getMemoryUsageMB(){
  std::ifstream status("/proc/self/status");
  std::string line;
  long vmRSS = 0;

  while(std::getline(status, line)){
    if(line.find("VmRSS:") == 0){
      sscanf(line.c_str(), "VmRSS: %ld", &vmRSS);
      break;
    }
  }

  return vmRSS / 1024;
}
