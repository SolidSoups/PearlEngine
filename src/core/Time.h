#pragma once
#include <GLFW/glfw3.h>

class Time{
public:
  static void Initialize(){
    // intialize with beginning times (in seconds)
    m_StartTime = glfwGetTime();
    m_LastTime = m_StartTime;
    deltaTime = 0.0f;
    time = 0.0f;
  }

  static void Update(){
    float currentTime = glfwGetTime();    

    // update deltaTime
    deltaTime = currentTime - m_LastTime;
    m_LastTime = currentTime;

    // update time
    time = currentTime - m_StartTime;
  }

  // the frame time in seconds
  static float deltaTime;
  // the time of the application in seconds
  static float time;

private:
  static float m_LastTime;
  static float m_StartTime;
};
