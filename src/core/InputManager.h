#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "Input.h"

typedef struct GLFWwindow GLFWwindow;


class InputManager {
public:
  struct Chord{
    Chord(const std::vector<int>& _keys,
          const std::function<void()>& _cb,
          int _progress) : keys(_keys), callback(_cb), progress(_progress) {}
    std::vector<int> keys;
    std::function<void()> callback;
    int progress = 0;
  };
public:
  InputManager(GLFWwindow *window);
  bool IsKeyPressed(int key);
  bool IsKeyReleased(int key);

  bool IsStringKeyPressed(const std::string& key);

  void PollCallbacks();
public: // for lua
  void RegisterChordCallback(const std::string& chords, const std::function<void()>&callback);

  const std::vector<Chord>& GetChords() const { return mChords; }
private:
  std::vector<int> ParseKeys(const std::string& input); 

private:
  GLFWwindow *mWindow = nullptr;

private:
  std::vector<Chord> mChords;
  std::unordered_set<int> mTrackedKeys;
  std::unordered_map<int, bool> mPrevKeyState;
};
