#include "sggridimagetexture.h"
#include "sgglwrapper.h"


//
SGGridImageTexture::SGGridImageTexture(int pxwid, int pxhei, Image::ImageFormat pxfmt, int cellwid, int cellhei)
{
    m_imgBuf.cellWid = cellwid;
    m_imgBuf.cellHei = cellhei;
    m_imgBuf.sentinal = 0;

    m_imgBuf.maxCol = pxwid / cellwid;
    m_imgBuf.maxRow = pxhei / cellhei;

    switch(pxfmt)
    {
    case Image::ImageFormat::Format_Alpha:
        m_imgBuf.gl_fmt = GL_RED;
        break;
    case Image::ImageFormat::Format_RGB:
        m_imgBuf.gl_fmt = GL_RGB;
        break;
    case Image::ImageFormat::Format_RGBA:
        m_imgBuf.gl_fmt = GL_RGBA;
        break;
    }

    m_imgBuf.init(pxwid, pxhei, pxfmt);

    m_texBuffer = &m_imgBuf;
}

SGGridImageTexture::~SGGridImageTexture()
{
}

void SGGridImageTexture::build()
{
}

void SGGridImageTexture::bind()
{
    if (!m_id)
    {
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, m_imgBuf.gl_fmt, m_texBuffer->pxWid, m_texBuffer->pxHei
            , 0, m_imgBuf.gl_fmt, GL_UNSIGNED_BYTE, m_texBuffer->data);
        //glGenerateMipmap(GL_TEXTURE_2D);//to do... need to support
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}

Size SGGridImageTexture::size() const
{
    return Size{ m_texBuffer->pxWid, m_texBuffer->pxHei };
}

unsigned SGGridImageTexture::add(const Image &img)
{
    int col = m_imgBuf.sentinal % m_imgBuf.maxCol;
    int row = m_imgBuf.sentinal / m_imgBuf.maxCol;
    if (row >= m_imgBuf.maxRow)
    {
        printf("image texture full...\n");
        return 0;
    }


    int key = 0;
    if (img.format == m_imgBuf.pxComp && img.width <= m_imgBuf.cellWid && img.height <= m_imgBuf.cellHei)
    {
        //write data and move sentinel to next;
        m_imgBuf.sentinal++;

        float rleft = col / float(m_imgBuf.maxCol);
        float rwid = float(img.width) / m_imgBuf.pxWid;
        float rtop = row / float(m_imgBuf.maxRow);
        float rhei = float(img.height) / m_imgBuf.pxHei;
        RectF rec(rleft, rtop, rwid, rhei);

        m_rects.emplace(std::make_pair(keyUid, rec));
        key = keyUid++;

        if (!m_id)
        {
            //write to cache
            int offset = m_imgBuf.cellHei*row*m_imgBuf.pxWid*m_imgBuf.pxComp;
            int xstep = m_imgBuf.cellWid*m_imgBuf.pxComp*col;

            char *p = (char *)m_imgBuf.data + offset + xstep;
            const unsigned char *psrc = img.data;
            for (int i = 0; i < img.height; i++)
            {
                memcpy(p, psrc, img.width *m_imgBuf.pxComp);
                p += m_imgBuf.pxWid*m_imgBuf.pxComp;
                psrc += img.width*m_imgBuf.pxComp;
            }
        }
        else
        {
            //alignment check
            bool realignment = m_imgBuf.pxComp != Image::ImageFormat::Format_RGBA;
            int savealignment = 0;
            if(realignment)
            {
                glGetIntegerv(GL_UNPACK_ALIGNMENT, &savealignment);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            }

            //sync to gl
            glBindTexture(GL_TEXTURE_2D, m_id);
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                m_imgBuf.cellWid*col, m_imgBuf.cellHei*row,
                img.width, img.height, m_imgBuf.gl_fmt, GL_UNSIGNED_BYTE, img.data);

            //restore alignment
            if (realignment)
            {
                glPixelStorei(GL_UNPACK_ALIGNMENT, savealignment);
            }

        }
    }
    return key;
}

bool SGGridImageTexture::update(unsigned key, const Image & img)
{
    return false;
}
