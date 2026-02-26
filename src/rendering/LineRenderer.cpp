#include "LineRenderer.h"

std::shared_ptr<ShaderData> LineRenderer::mShader = nullptr;
std::vector<LineRenderer::Line> LineRenderer::mLines{};
GLuint LineRenderer::mVAO = 0;
GLuint LineRenderer::mVBO = 0;


void LineRenderer::Initialize(const std::shared_ptr<ShaderData> shader){
  mShader = shader; 

  // initalize objects
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

  // color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}


void LineRenderer::DrawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color){
  mLines.push_back({a, b, color});
}

void LineRenderer::Flush(const glm::mat4 viewProj){
  if(mVAO == 0 || mVBO == 0) return;
  if(mLines.empty()) return;

  // interleave: a, b, color
  std::vector<float> verts;
  for(auto &l : mLines){
    verts.insert(verts.end(), {l.a.x, l.a.y, l.a.z, l.color.r, l.color.g, l.color.b});
    verts.insert(verts.end(), {l.b.x, l.b.y, l.b.z, l.color.r, l.color.g, l.color.b});
  }

  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);

  mShader->use();
  mShader->setMatrix4("uViewProj", viewProj);

  glDrawArrays(GL_LINES, 0, mLines.size() * 2);

  mLines.clear();
}

void LineRenderer::Destroy(){
  // delete objects if used
  if(mVAO) glDeleteVertexArrays(1, &mVAO);
  if(mVBO) glDeleteBuffers(1, &mVBO);
}
