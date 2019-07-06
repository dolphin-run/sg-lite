#include "sgtexture.h"

Rect SGTexture::g_invalidRect;

SGTexture::SGTexture()
{
}

SGTexture::~SGTexture()
{
}

const Rect & SGTexture::retrieve(int key) const
{
    if (m_vals.count(key)) return m_vals.at(key);
    else return g_invalidRect;
}
