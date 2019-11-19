#include "sgyuvtexture.h"
#include "sgglwrapper.h"


//
SGYuvTexture::SGYuvTexture(unsigned components):
    m_components(components)
{
}

SGYuvTexture::~SGYuvTexture()
{
}

void SGYuvTexture::build()
{
}

void SGYuvTexture::bind()
{
    if (!m_texture[0])
    {
        glGenTextures(m_components, m_texture);
        for (int i = 0; i < m_components; i++)
        {
            glBindTexture(GL_TEXTURE_2D, m_texture[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }
    else
    {
        for (int i = 0; i < m_components; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_texture[i]);
        }
    }
}

Size SGYuvTexture::size() const
{
    return Size{ m_texBuffer->pxWid, m_texBuffer->pxHei };
}

unsigned SGYuvTexture::add(const Image &img)
{
    assert(false);
    return 0;
}

bool SGYuvTexture::update(unsigned compidx, const Image & img)
{
    if(compidx < 0 || compidx >= m_components) return false;

    auto const *ptr = img.data;
    GLenum eTexture = GL_TEXTURE0 + compidx;
    GLuint texture = m_texture[compidx];

    if(!ptr || !eTexture || !texture) return false;

    //sync to gl
    glActiveTexture(eTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint innerfmt = GL_R8, inputfmt = GL_RED;
    switch (img.format)
    {
    case Image::ImageFormat::Format_Alpha:
        innerfmt = GL_R8;
        inputfmt = GL_RED;
        break;
    case Image::ImageFormat::Format_RA:
        innerfmt = GL_RG8;
        inputfmt = GL_RG;
        break;
    case Image::ImageFormat::Format_RGB:
        innerfmt = GL_RGB8;
        inputfmt = GL_RGB;
        break;
    case Image::ImageFormat::Format_RGBA:
        innerfmt = GL_RGBA8;
        inputfmt = GL_RGBA;
        break;
    default:
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, innerfmt, img.width, img.height
        , 0, inputfmt, GL_UNSIGNED_BYTE, ptr);

    return true;
}