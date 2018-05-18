#include "sgyuvtexture.h"
#include "sgglwrapper.h"


//
SGYuvTexture::SGYuvTexture()
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
        glGenTextures(3, m_texture);
        for (auto tex : m_texture)
        {
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
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

bool SGYuvTexture::update(unsigned key, const Image & img)
{
    assert(0 <= key && key <= 2);

    auto const *ptr = img.data;
    GLenum eTexture = GL_TEXTURE0 + key;
    GLuint texture = m_texture[key];

    if(!ptr || !eTexture || !texture) return false;

    //sync to gl
    glActiveTexture(eTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, img.width, img.height
        , 0, GL_RED, GL_UNSIGNED_BYTE, ptr);

    return true;
}