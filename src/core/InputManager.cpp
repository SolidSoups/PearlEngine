#include "InputManager.h"

#include <sstream>
#include <algorithm>

#include <GLFW/glfw3.h>
#include "Logger.h"

InputManager::InputManager(GLFWwindow *window) { mWindow = window; }

namespace {
inline bool isWithinRange(int key) {
  return key >= 32 && key <= 162 || key >= 256 || key <= 348;
}
}; // namespace

bool InputManager::IsKeyPressed(int key) {
  if (!isWithinRange(key)) {
    LOG_ERROR << "Key '" << key << "' is invalid";
    return false;
  }
  return mWindow && glfwGetKey(mWindow, key) == GLFW_PRESS;
}

bool InputManager::IsStringKeyPressed(const std::string& key){
  auto key_it = StringToKey.find(key);
  if(key_it == StringToKey.end()){
    LOG_ERROR << "Cannot bind key '" << key << "' as it doesn't exist";
    return false;
  }

  return mWindow && glfwGetKey(mWindow, key_it->second) == GLFW_PRESS;
}

bool InputManager::IsKeyReleased(int key) {
  if (!isWithinRange(key)) {
    LOG_ERROR << "Key '" << key << "' is invalid";
    return false;
  }

  return mWindow && glfwGetKey(mWindow, key) == GLFW_RELEASE;
}

void InputManager::PollCallbacks() {
  if (!mWindow)
    return;

  // find keys that transitioned this frame
  std::unordered_set<int> justPressed;
  for(int key : mTrackedKeys){
    bool now = glfwGetKey(mWindow, key) == GLFW_PRESS;
    if(now && !mPrevKeyState[key])
      justPressed.insert(key);
    mPrevKeyState[key] = now;
  }

  // advance or reset next chord
  if(!justPressed.empty())
    for(auto& chord : mChords){
      int expected = chord.keys[chord.progress];

      if(justPressed.count(expected)){
        chord.progress++;
        if(chord.progress >= (int)chord.keys.size()){
          chord.callback();
          chord.progress = 0;
        } 
      } else {
          // wrong key pressed - reset
          chord.progress = 0;
        }
    }
}

void InputManager::RegisterChordCallback(const std::string &chords,
                                         const std::function<void()> &callback) {
  std::vector<int> keys = ParseKeys(chords);
  if(keys.empty()) return;

  LOG_INFO << "Got keys: ";
  for(auto key : keys){
    mTrackedKeys.insert(key);
    LOG_INFO << key;
  }


  mChords.emplace_back(keys, callback, 0); 
}


std::vector<int> InputManager::ParseKeys(const std::string &input){
  // split 'space+g' into ["Space", "G"]
  std::vector<std::string> tokens;
  std::stringstream ss(input);
  std::string token;
  while (std::getline(ss, token, '+')) {
    tokens.push_back(token);
  }

  std::vector<int> keys;
  for (auto& t : tokens) {
    std::transform(t.begin(), t.end(), t.begin(), ::toupper);

    // find key
    auto it = StringToKey.find(t);
    if (it == StringToKey.end()) {
      LOG_ERROR << "Failed to parse key chords for '" << input << "'";
      return {};
    }

    keys.push_back(it->second);
  }
  return keys;
}
