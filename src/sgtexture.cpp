#include "sgtexture.h"

RectF SGTexture::g_invalidRect;

SGTexture::SGTexture()
{
}

SGTexture::~SGTexture()
{
}

const RectF & SGTexture::retrieve(unsigned key) const
{
    if (m_rects.count(key)) return m_rects.at(key);
    else return g_invalidRect;
}


//
TexBuffer::~TexBuffer()
{
    cleanup();
}

void TexBuffer::init(int wid, int hei, int comp)
{
    assert(comp >= 1 && comp <= 4);
    cleanup();

    pxWid = wid;
    pxHei = hei;
    pxComp = comp;

    data = malloc(wid*hei*comp);
    memset(data, 0, wid*hei*comp);
}

void TexBuffer::cleanup()
{
    if (data)
    {
        free(data);
        data = nullptr;
    }

    pxWid = 0;
    pxHei = 0;
    pxComp = 0;
}
