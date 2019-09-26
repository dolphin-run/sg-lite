#include "sgdynimagetexture.h"
#include "sgglwrapper.h"


//
SGDynImageTexture::SGDynImageTexture()
{
}

SGDynImageTexture::~SGDynImageTexture()
{
}

void SGDynImageTexture::build()
{
}

void SGDynImageTexture::bind()
{
    if (!m_texture)
    {
        glGenTextures(1, &m_texture);

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }
}

Size SGDynImageTexture::size() const
{
    return Size{ m_texBuffer->pxWid, m_texBuffer->pxHei };
}

unsigned SGDynImageTexture::add(const Image &img)
{
    assert(false);
    return 0;
}

bool SGDynImageTexture::update(unsigned key, const Image & img)
{
    assert(key == 0);
    auto const *ptr = img.data;

    if(!ptr || !m_texture) return false;

    //sync to gl
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height
        , 0, GL_RGB, GL_UNSIGNED_BYTE, ptr);

    return true;
}