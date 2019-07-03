#include "sgglyphtexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
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

        funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texBuffer->pxWid, m_texBuffer->pxHei
            , 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texBuffer->data);
        funcs->glGenerateMipmap(GL_TEXTURE_2D);
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
    int col = m_glyphBuf.sentinal % m_glyphBuf.maxCol;
    int row = m_glyphBuf.sentinal / m_glyphBuf.maxCol;
    if (row >= m_glyphBuf.maxRow)
    {
        printf("image texture full...\n");
        return 0;
    }

    int x=0, y=0, n=0;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    if (!data)
    {
        printf("image open failed...[%s]\n", filename);
        return 0;
    }
    
    int key = 0;
    if (n == m_glyphBuf.pxComp && x <= m_glyphBuf.cellWid && y <= m_glyphBuf.cellHei)
    {
        //write data and move sentinel to next;
        m_glyphBuf.sentinal++;

        RectF rec;// (0, 0, 0.1, 0.2);
        rec.set(
            col / float(m_glyphBuf.maxCol),
            row / float(m_glyphBuf.maxRow),
            (col + 1) / float(m_glyphBuf.maxCol),
            (row + 1) / float(m_glyphBuf.maxRow)
        );

        m_rects.emplace(std::make_pair(keyUid, rec));
        key = keyUid++;

        //write data
        int offset = m_glyphBuf.cellHei*row*m_glyphBuf.pxWid*m_glyphBuf.pxComp;
        int xstep = m_glyphBuf.cellWid*m_glyphBuf.pxComp*col;

        char *p = (char *)m_glyphBuf.data + offset + xstep;
        uchar *psrc = data;
        for (int i = 0; i < y; i++)
        {
            memcpy(p, psrc, x*4);
            p += m_glyphBuf.pxWid*m_glyphBuf.pxComp;
            psrc += m_glyphBuf.cellWid*m_glyphBuf.pxComp;
        }

        //sync to 
        if (m_id)
        {
            QOpenGLFunctions *funcs = QOpenGLContext::currentContext()->functions();
            funcs->glBindTexture(GL_TEXTURE_2D, m_id);
            funcs->glTexSubImage2D(GL_TEXTURE_2D, 0, m_glyphBuf.cellWid*col, m_glyphBuf.cellHei*row, x, y, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
    }
    else
    {
        printf("image illegal...[%s]\n", filename);
    }
    stbi_image_free(data);
    return key;
}
