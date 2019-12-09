#pragma once

#include <GL/glew.h>

#include <cstdio>

class ShadowMap
{
private:
  /* data */
protected:
  GLuint depthMapFBO, shadowMap;
  GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
public:
  ShadowMap();
  virtual bool init(GLuint width, GLuint height);
  virtual void Write();
  virtual void Read(GLenum textureUnit);
  GLuint GetShadowWidth() { return SHADOW_WIDTH; };
  GLuint GetShadowHeight() { return SHADOW_HEIGHT; };
  ~ShadowMap();
};
