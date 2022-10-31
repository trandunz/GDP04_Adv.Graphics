// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : TextureLoader.cpp 
// Description : TextureLoader Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "TextureLoader.h"
#include "Statics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STBI/stb_image.h"

TextureLoader::~TextureLoader()
{
    // Cleanup All Stored Textures
    for (auto& item : m_Textures)
    {
        glDeleteTextures(1, &item.ID);
    }
}

void TextureLoader::Init(std::vector<const char*>&& _textures)
{
    for (auto& item : _textures)
    {
        LoadTexture(item);
    }
}

Texture TextureLoader::LoadTexture(std::string&& _fileName)
{
    // Checks If A Texture With The Same File path Has Already Been Created
    for (auto& item : m_Textures)
    {
        if (item.FilePath == _fileName)
        {
            return item;
        }
    }

    stbi_set_flip_vertically_on_load(true);

    GLint width, height, components;

    // Grab Image Data Using STB_Image And Store It In A const char*
    GLubyte* imageData = stbi_load(("Resources/Textures/" + _fileName).data(), & width, & height, & components, 0);
    
    // Generate And Bind A New Texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Check And Assigns Number Of Components (e.g RGB)
    components = components == 4 ? GL_RGBA : components == 3 ? GL_RGB : components == 2 ? GL_RG : GL_R;
    
    // Assigns The Image Data To The Bound Texture
    glTexImage2D(GL_TEXTURE_2D, 0 ,components, width, height, 0, components, GL_UNSIGNED_BYTE, imageData);
    
    // Generates MipMaps For Bound Texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set Texture Parameters For Bound Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Free The Loaded Data And Unbind Texture
    stbi_image_free(imageData);
    imageData = nullptr;
    glBindTexture(GL_TEXTURE_2D, 0);

    // Add Newly Created Texture To Vector
    m_Textures.emplace_back(Texture{ id , {width,height},_fileName });

    // Return Newly Created Texture
    return m_Textures.back();
}

Texture TextureLoader::CreateRenderTexture()
{
    // Generate the texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the storage type (RGB)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // set the texture parameeters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // geenrate mip maps
    glGenerateMipmap(GL_TEXTURE_2D);

    return Texture{ id , Statics::WindowSize};
}

Texture TextureLoader::LoadCubemap(std::vector<std::string> _fileNames)
{
    for (auto& item : m_Textures)
    {
        if (item.FilePath == _fileNames[0])
        {
            return item;
        }
    }

    stbi_set_flip_vertically_on_load(false);

    GLint width, height, components;
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    // Load in all sides of cubemap
    for(int i = 0; i < 6; i++)
    {
        GLubyte* imageData = stbi_load(("Resources/Textures/Cubemaps/" + _fileNames[i]).data(), &width, &height, &components, 0);
        
        components = components == 4 ? GL_RGBA : components == 3 ? GL_RGB : components == 2 ? GL_RG : GL_R;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, components, width, height, 0,
            components, GL_UNSIGNED_BYTE, imageData);

        stbi_image_free(imageData);
        imageData = nullptr;
    }

    // Generates MipMaps For Bound Texture
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Set Texture Parameters For Bound Texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    m_Textures.emplace_back(Texture{ id , {0,0}, _fileNames[0]});

    return m_Textures.back();
}

std::vector<unsigned char> TextureLoader::LoadHeightMap(std::string&& _fileName)
{
    stbi_set_flip_vertically_on_load(false);

    // A height for each vertex
    std::vector<unsigned char> in(513 * 513);

    GLint width, height, components;

    // Grab Image Data Using STB_Image And Store It In A const char*
    GLubyte* imageData = stbi_load(("Resources/Textures/Heightmaps/" + _fileName).data(), &width, &height, &components, 0);

    for (int i = 0; i < width * height; i++)
    {
        in[i] = imageData[i];
    }

    stbi_image_free(imageData);
    imageData = nullptr;

    return in;
}

Texture TextureLoader::CreatePositionTexture()
{
    // Generate the texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the storage type (RGB)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_RGB, GL_FLOAT, NULL);

    // set the texture parameeters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return Texture{ id , Statics::WindowSize };
}

Texture TextureLoader::CreateAlbedoTexture()
{
    // Generate the texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the storage type (RGB)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

    // set the texture parameeters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return Texture{ id , Statics::WindowSize };
}

Texture TextureLoader::CreateDepthTexture()
{
    // Generate the texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the storage type (RGB)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // geenrate mip maps
    glGenerateMipmap(GL_TEXTURE_2D);

    return Texture{ id , Statics::WindowSize };
}
