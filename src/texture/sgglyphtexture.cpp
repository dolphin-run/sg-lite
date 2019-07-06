#include "sgglyphtexture.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>


const short WIDPIXEL = 2048;
const short HEIPIXEL = 2048;

void TexGlyphBuffer::init(int wid, int hei, int comp)
{
    TexBuffer::init(wid, hei, comp);

    maxCol = wid / cellWid;
    maxRow = hei / cellHei;
}

//
SGGlyphTexture::SGGlyphTexture()
{
    build();
}

SGGlyphTexture::~SGGlyphTexture()
{
}

void SGGlyphTexture::build()
{
    if (!m_texBuffer)
    {
        m_texBuffer = &m_glyphBuf;
        m_texBuffer->init(WIDPIXEL, HEIPIXEL, 1);
    }
}

void SGGlyphTexture::bind()
{
    QOpenGLFunctions *funcs = QOpenGLContext::currentContext()->functions();
    if (!m_id)
    {
        funcs->glGenTextures(1, &m_id);
        funcs->glActiveTexture(GL_TEXTURE0);
        funcs->glBindTexture(GL_TEXTURE_2D, m_id);
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_texBuffer->pxWid, m_texBuffer->pxHei
            , 0, GL_RED, GL_UNSIGNED_BYTE, m_texBuffer->data);
        funcs->glGenerateMipmap(GL_TEXTURE_2D);

        //Qt debug
        //QImage img((uchar*)m_texBuffer->data, m_texBuffer->pxWid, m_texBuffer->pxHei, QImage::Format::Format_Alpha8);
        //img.save("img.png");
    }
    else
    {
        funcs->glActiveTexture(GL_TEXTURE0);
        funcs->glBindTexture(GL_TEXTURE_2D, m_id);
    }
}

int SGGlyphTexture::textureId() const
{
    return m_id;
}

Size SGGlyphTexture::size() const
{
    return Size{ m_texBuffer->pxWid, m_texBuffer->pxHei };
}

unsigned SGGlyphTexture::add(const char * filename)
{
    assert(false);
    
    return 0;
}

unsigned SGGlyphTexture::add(const Image &img)
{
    int col = m_glyphBuf.sentinal % m_glyphBuf.maxCol;
    int row = m_glyphBuf.sentinal / m_glyphBuf.maxCol;
    if (row >= m_glyphBuf.maxRow)
    {
        printf("image texture full...\n");
        return 0;
    }


    int key = 0;
    if (img.format == m_glyphBuf.pxComp && img.width <= m_glyphBuf.cellWid && img.height <= m_glyphBuf.cellHei)
    {
        //write data and move sentinel to next;
        m_glyphBuf.sentinal++;

        float rleft = col / float(m_glyphBuf.maxCol);
        float rwid = float(img.width) / m_glyphBuf.pxWid;
        float rtop = row / float(m_glyphBuf.maxRow);
        float rhei = float(img.height) / m_glyphBuf.pxHei;
        RectF rec(rleft, rtop, rwid, rhei);

        m_rects.emplace(std::make_pair(keyUid, rec));
        key = keyUid++;

        if (!m_id)
        {
            //write to cache
            int offset = m_glyphBuf.cellHei*row*m_glyphBuf.pxWid*m_glyphBuf.pxComp;
            int xstep = m_glyphBuf.cellWid*m_glyphBuf.pxComp*col;

            char *p = (char *)m_glyphBuf.data + offset + xstep;
            const uchar *psrc = img.data.get();
            for (int i = 0; i < img.height; i++)
            {
                memcpy(p, psrc, img.width *m_glyphBuf.pxComp);
                p += m_glyphBuf.pxWid*m_glyphBuf.pxComp;
                psrc += img.width*m_glyphBuf.pxComp;
            }
        }
        else
        {
            //sync to gl
            QOpenGLFunctions *funcs = QOpenGLContext::currentContext()->functions();
            funcs->glBindTexture(GL_TEXTURE_2D, m_id);
            funcs->glTexSubImage2D(GL_TEXTURE_2D, 0,
                m_glyphBuf.cellWid*col, m_glyphBuf.cellHei*row,
                img.width, img.height, GL_R8, GL_UNSIGNED_BYTE, img.data.get());
        }
    }
    return key;
}
