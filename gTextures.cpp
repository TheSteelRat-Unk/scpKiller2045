#include "gTextures.h"

Texture::Texture(const std::string & filepath)
{
    glGenTextures(1, &textureId);
    // stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);    
    if(!data) {
        throw std::runtime_error("failed to load texture");
    }
    //std::cout << " successful loaded  " << std::endl;
    std::map<int, GLenum> formatMap= 
    {
        {1, GL_RED},
        {3, GL_RGB},
        {4, GL_RGBA}
    };

    GLenum format = formatMap[nrChannels];

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

}


void Texture::Bind()
{  
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::GetWidth()
{
    return width;
}

int Texture::GetHeight()
{
    return height;
}
