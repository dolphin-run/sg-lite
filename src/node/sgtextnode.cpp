
#include "sgtextnode.h"
#include "sgtextnode_p.h"
#include "geometry/sgrectanglegeometry.h"
#include "material/sgglyphmaterial.h"
#include <algorithm>

SGTextNode::SGTextNode(SGNode * parent) :
    SGNode(E_NodeType::TextNodeType, new SGTextNodePrivate, parent)
{
}

SGTextNode::~SGTextNode()
{
}

void SGTextNode::setAlignment(HAlignment ha, VAlignment va)
{
    SG_D(SGTextNode);
    d->m_halignment = ha;
    d->m_valignment = va;

    d->reAlign();
    markDirty(SGNode::E_DirtyType::PositionDirty);
}

void SGTextNode::setHAlignment(HAlignment ha)
{
    SG_D(SGTextNode);
    d->m_halignment = ha;

    d->reAlign();
    markDirty(SGNode::E_DirtyType::PositionDirty);
}

void SGTextNode::setVAlignment(VAlignment va)
{
    SG_D(SGTextNode);
    d->m_valignment = va;

    d->reAlign();
    markDirty(SGNode::E_DirtyType::PositionDirty);
}

void SGTextNode::setText(const std::wstring & txt)
{
    SG_D(SGTextNode);
    d->setText(txt);

    d->reAlign();
    markDirty(SGNode::E_DirtyType::SizeDirty);
}

void SGTextNode::setColor(const Color & color)
{
    SG_D(SGTextNode);
    d->m_color = color;
}

void SGTextNode::setPointSize(int point)
{
    SG_D(SGTextNode);
    d->m_pointSize = point;

    d->reAlign();
    markDirty(SGNode::E_DirtyType::PositionDirty);
    markDirty(SGNode::E_DirtyType::SizeDirty);
}

bool SGTextNode::compare(const SGNode *node) const
{
    const SGTextNode *tnode = dynamic_cast<const SGTextNode*>(node);
    return d_func()->m_color == tnode->d_func()->m_color;
}

SGNodeState SGTextNode::nodeState() const
{
    SGNodeState stat;
    stat.color = d_func()->m_color;
    return stat;
}

//PRIVATE IMPLEMENTATION
SGTextNodePrivate::SGTextNodePrivate() :
    m_color(0, 0, 0)
{
    m_valignment = VAlignment::AlignVCenter;
    m_halignment = HAlignment::AlignHCenter;

    m_material = m_glyphMaterial = new SGGlyphMaterial;
    m_geometry = m_textGeometry = new SGTextRectangleGeometry;

}

void SGTextNodePrivate::addLineText(const std::wstring & s, size_t first, size_t last)
{
    assert(s.size());
    //assert(first < last);//allow an empty line

    LineRenderInfo lineinf;
    int cx = 0;
    for (int i = first; i < last && i < s.size(); i++)
    {
        auto &glyinf = m_glyphMaterial->setGlyph(s.at(i));

        cx += glyinf.advance;
        lineinf.lineInf.push_back(&glyinf);
    }
    lineinf.width = cx;
    m_glyphLines.push_back(lineinf);
}

//to do... make multi-thread
void SGTextNodePrivate::setText(const std::wstring & txt)
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

    m_boundingWidth = 0;
    m_boundingHeight = m_glyphLines.size() * GLYPH_LINE_SIZE;
    int chnum = 0;
    //rewrite geometry position and texture.
    for (auto line : m_glyphLines)
    {
        chnum += line.lineInf.size();
        m_boundingWidth = std::max(m_boundingWidth, line.width);
    }

    m_textGeometry->resize(chnum);

    resetGeometry();
}

////////////////////////////////////////////////////////////
//
//      +--------------------------+
//      |                          |
//      |   ABC                    |
//      |                          |
//      +--------------------------+
//1. glyph pos may be not exactly as bounding rect pos.
//2. scale and rotation should be oriented from bounding rect center.
//make glyph as a sub-item of the bounding rect.
void SGTextNodePrivate::reAlign()
{
    float ratio = float(m_pointSize) / GLYPH_PIXEL_SIZE;
    m_implicitWidth = m_boundingWidth*ratio;
    m_implicitHeight = m_boundingHeight*ratio;

    //refix implicit position.
    int deltaX = 0;
    int deltaY = 0;
    switch (m_halignment)
    {
    case HAlignment::AlignLeft:
        deltaX = 0;
        break;
    case HAlignment::AlignHCenter:
        deltaX = (m_width - m_implicitWidth) / 2;
        break;
    case HAlignment::AlignRight:
        deltaX = m_width - m_implicitWidth;
        break;
    }
    switch (m_valignment)
    {
    case VAlignment::AlignTop:
        deltaY = 0;
        break;
    case VAlignment::AlignVCenter:
        deltaY = (m_height - m_implicitHeight) / 2;
        break;
    case VAlignment::AlignBottom:
        deltaY = m_height - m_implicitHeight;
        break;
    }

    m_implicitX = m_x + deltaX;
    m_implicitY = m_y + deltaY;
}

void SGTextNodePrivate::resetGeometry()
{
    int chidx = 0;
    int posX = 0;
    int posY = 0;
    for (int i = 0; i < m_glyphLines.size(); i++)
    {
        auto &lineinf = m_glyphLines.at(i);

        posX = 0;
        switch (m_halignment)
        {
        case HAlignment::AlignLeft:
            posX = 0;
            break;
        case HAlignment::AlignHCenter:
            posX = (m_boundingWidth - lineinf.width) / 2;
            break;
        case HAlignment::AlignRight:
            posX = m_boundingWidth - lineinf.width;
            break;
        }

        for (int j = 0; j < lineinf.lineInf.size(); j++)
        {
            auto inf = lineinf.lineInf.at(j);
            m_textGeometry->setTexCoord(chidx, inf->tex);

            m_textGeometry->setCoord(chidx,
                float(posX + inf->leftBearing) / m_boundingWidth - 0.5f,
                float(posY + inf->topBearing) / m_boundingHeight - 0.5f,
                float(posX + inf->leftBearing + inf->width) / m_boundingWidth - 0.5f,
                float(posY + inf->topBearing + inf->height) / m_boundingHeight - 0.5f);
            posX += inf->advance;
            chidx++;
        }

        posY += GLYPH_LINE_SIZE;
    }
}