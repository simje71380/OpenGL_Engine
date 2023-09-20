#include "Texture.h"

Texture::Texture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(path, &widthImg, &heightImg, &numColCh, 0);

    //create a texture in opengl
    GLCall(glGenTextures(1, &texture));
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));

    //setup how the texture should apply 
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    //use GL_RGB for jpeg/jpg/png else GL_RGBA
    std::string extension = Get_Extension(path);
    if (extension == ".jpeg" || extension == ".jpg" || extension == ".png") {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes));
    }
    else {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes));
    }  
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &texture));
}

void Texture::Bind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
}

void Texture::Unbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


