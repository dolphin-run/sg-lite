#include "sgyuvtexture.h"
#include "glad/glad.h"

SYUVInfo::SYUVInfo(int pixWid, int pixHei)
{
    update(pixWid, pixHei);
    setViewSize(pixWid, pixHei);
}

bool SYUVInfo::update(int pixWid, int pixHei)
{
    m_wid = pixWid;
    m_hei = pixHei;
    m_semiWid = m_wid >> 1;
    m_semiHei = m_hei >> 1;

    m_yWid = pixWid;
    m_yHei = pixHei;
    m_uWid = (pixWid + 1) / 2;
    m_uHei = (pixHei + 1) / 2;

    m_yLen = m_yWid*m_yHei;
    m_uLen = m_uWid*m_uHei;
    m_len = m_yLen + m_uLen * 2;

    m_yuvRadio = m_semiHei / m_semiWid;

    return true;
}

bool SYUVInfo::setViewSize(int pixWid, int pixHei)
{
    m_viewWid = pixWid;
    m_viewHei = pixHei;

    //adapt to render pixels
    m_renderScale = min(float(m_viewWid) / m_wid, float(m_viewHei) / m_hei);
    return true;
}

//
SGYuvTexture::SGYuvTexture()
{
    static SYUVInfo info;
    m_yuvInfo = &info;
}

SGYuvTexture::~SGYuvTexture()
{
}

void SGYuvTexture::setYuvInfo(SYUVInfo * info)
{
    m_yuvInfo = info;
}

void SGYuvTexture::build()
{
}

void SGYuvTexture::bind()
{
    if (!m_textureY)
    {
        glGenTextures(1, &m_textureY);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureY);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureU);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureU);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &m_textureV);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureV);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureY);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureU);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureV);
    }
}

Size SGYuvTexture::size() const
{
    return Size{ m_texBuffer->pxWid, m_texBuffer->pxHei };
}

unsigned SGYuvTexture::add(const Image &img)
{
    int key = 0;
    //if (img.format == Image::Format_Alpha && img.width == m_yuvInfo->m_wid && img.height <= m_yuvInfo->m_hei)
    {
        if (!m_textureY)
        {
        }
        else
        {
            //sync to gl
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureY);
            auto const *ptr = img.data.get();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_yuvInfo->m_yWid, m_yuvInfo->m_yHei
                , 0, GL_RED, GL_UNSIGNED_BYTE, ptr);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_textureU);
            ptr += m_yuvInfo->m_yLen;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_yuvInfo->m_uWid, m_yuvInfo->m_uHei
                , 0, GL_RED, GL_UNSIGNED_BYTE, ptr);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_textureV);
            ptr += m_yuvInfo->m_uLen;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_yuvInfo->m_uWid, m_yuvInfo->m_uHei
                , 0, GL_RED, GL_UNSIGNED_BYTE, ptr);

            key = 1;
        }
    }
    return key;
}
