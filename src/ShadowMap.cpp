#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
  depthMapFBO = 0;
  shadowMap = 0;
}

bool ShadowMap::init(GLuint width, GLuint height)
{
  SHADOW_WIDTH = width; SHADOW_HEIGHT = height;

  glGenFramebuffers(1, &depthMapFBO);

  float borderCorlor[] = { 1.0f, 1.0f, 1.0f };
  glGenTextures(1, &shadowMap);
  glBindTexture(GL_TEXTURE_2D, shadowMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderCorlor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(status != GL_FRAMEBUFFER_COMPLETE)
  {
    printf("Framebuffer Error: %i\n", status);
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}

void ShadowMap::Write()
{
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, shadowMap);
}

ShadowMap::~ShadowMap()
{
  if(depthMapFBO)
  {
    glDeleteFramebuffers(1, &depthMapFBO);
  }
  if(shadowMap)
  {
    glDeleteTextures(1, &shadowMap);
  }
}
