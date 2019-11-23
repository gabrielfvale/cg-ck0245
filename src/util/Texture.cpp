#include "Texture.hpp"

Texture::Texture(unsigned char *pixels, int w, int h)
{
  this->data = pixels;
  this->w = w;
  this->h = h;
}

RGB Texture::value(float u, float v, const Point& p)
{
  int i = (    u)*w;
	int j = (1 - v)*h - 0.001;

	if (i < 0) i = 0;
	if (j < 0) j = 0;

	if (i > w - 1) i = w - 1;
	if (j > h - 1) j = h - 1;

	float r = int(data[3 * i + 3 * w*j]  ) / 255.0;
	float g = int(data[3 * i + 3 * w*j+1]) / 255.0;
	float b = int(data[3 * i + 3 * w*j+2]) / 255.0;

	return RGB(r, g, b);
}
