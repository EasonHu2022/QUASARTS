#include "Texture2D.h"

namespace Engine
{
	Texture2D::Texture2D(std::string path, int _width, int _height): ITexture(path)
	{
		if (_width != 0)
			width = _width;
		if (_height != 0)
			height = _height;
		set_up();
	}
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &textureObject);
	}
	void Texture2D::set_up()
	{
		glGenTextures(1, &textureObject);
		glBindTexture(GL_TEXTURE_2D, textureObject);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if(nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::use(GLenum texture)
	{
		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_2D, textureObject);
	}
};