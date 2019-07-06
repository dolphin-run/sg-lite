
#include "sgtextnode.h"
#include "geometry/sgrectanglegeometry.h"
#include "util/sgglyphtexture.h"

SGTextNode::SGTextNode(SGNode * parent) :
    SGNode(E_NodeType::TextNodeType, parent)
{
    m_material = m_glyphMaterial = new SGGlyphMaterial;
    m_geometry = m_textGeometry = new SGTextRectangleGeometry;
}

void SGTextNode::addLineText(const std::wstring & s, size_t first, size_t last)
{
    assert(s.size());
    //assert(first < last);//alow empty line

    LineRenderInfo lineinf;
    int cx = 0;
    for (int i = first; i < last && i < s.size(); i++)
    {
        auto &glyinf = m_glyphMaterial->setGlyph(s.at(i));
        //auto &inf = g_glyphLoader.retrieve(s.at(i));

        //cx += inf->m_width + inf->m_leftBearing;
        cx += glyinf.advance;
        lineinf.lineInf.push_back(&glyinf);
    }
    lineinf.width = cx;
    //rendinf.m_startHPix = cx / 2 - WORDLOADER_PIXEL / 2;
    m_glyphLines.push_back(lineinf);
}

//to do... make multi-thread
void SGTextNode::setText(const std::wstring & txt)
{
    m_glyphLines.clear();
    if (txt.empty()) return;

    size_t pos = -1;
    do
    {
        size_t start = pos + 1;
        pos = txt.find(L'\n', start);
        addLineText(txt, start, pos);
    } while (pos != std::wstring::npos);

    int boundingWidth = 0;
    int boundingHeight = m_glyphLines.size()*WORDLOADER_WORD_PIXEL;
    int chnum = 0;
    //rewrite geometry position and texture.
    for (auto line : m_glyphLines)
    {
        chnum += line.lineInf.size();
        boundingWidth = std::max(boundingWidth, line.width);
    }

    m_textGeometry->resize(chnum);

    int chidx = 0;
    int posY = 0;
    for (int i = 0; i < m_glyphLines.size(); i++)
    {
        int posX = 0;
        for (int j = 0; j < m_glyphLines.at(i).lineInf.size(); j++)
        {
            auto inf = m_glyphLines.at(i).lineInf.at(j);
            m_textGeometry->setTexCoord(chidx, inf->tex);

            m_textGeometry->setCoord(chidx,
                float(posX + inf->leftBearing) / boundingWidth - 0.5f,
                float(posY + inf->topBearing) / boundingHeight - 0.5f,
                float(posX + inf->leftBearing + inf->width) / boundingWidth - 0.5f,
                float(posY + inf->topBearing + inf->height) / boundingHeight - 0.5f);
            posX += inf->advance;
            chidx++;
        }
        posY += WORDLOADER_WORD_PIXEL;
    }

    SGNode::m_width = boundingWidth;
    SGNode::m_height = boundingHeight;
}
