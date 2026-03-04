#version 330 core
uniform uint entityID;
out uint FragColor;

void main(){
  FragColor = entityID;
}
