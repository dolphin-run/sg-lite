
#include "sgtextnode.h"
#include "sgtextnode_p.h"
#include "geometry/sgrectanglegeometry.h"
#include "material/sgglyphmaterial.h"
#include <algorithm>
#include "sgnode_p.h"

SGTextNode::SGTextNode(SGNode * parent) :
    SGNode(E_NodeType::TextNodeType, parent)
{
    SG_INIT_PRIVATE(SGTextNode);

    d_ptr->m_glyphMaterial = new SGGlyphMaterial;
    d_ptr->m_textGeometry = new SGTextRectangleGeometry;

    setGeometry(d_ptr->m_textGeometry);
    setMaterial(d_ptr->m_glyphMaterial);
}

SGTextNode::~SGTextNode()
{
    SG_FREE_PRIVATE();
}

void SGTextNode::setAlignment(HAlignment ha, VAlignment va)
{
    d_ptr->m_halignment = ha;
    d_ptr->m_valignment = va;

    d_ptr->reAlign();
}

void SGTextNode::setHAlignment(HAlignment ha)
{
    d_ptr->m_halignment = ha;

    d_ptr->reAlign();
}

void SGTextNode::setVAlignment(VAlignment va)
{
    d_ptr->m_valignment = va;

    d_ptr->reAlign();
}

void SGTextNode::setText(const std::wstring & txt)
{
    Size bounding = d_ptr->setText(txt);
    setSize(bounding.width, bounding.height);

    d_ptr->reAlign();
}

void SGTextNode::setColor(const Color & color)
{
    //to do...
    d_ptr->m_color = color;

    markDirty(SGNode::E_DirtyType::SizeDirty);
}

bool SGTextNode::compare(const SGNode *node) const
{
    const SGTextNode *tnode = dynamic_cast<const SGTextNode*>(node);
    return d_ptr->m_color == tnode->d_ptr->m_color;
}

SGNodeState SGTextNode::nodeState() const
{
    SGNodeState stat;
    stat.color = d_ptr->m_color;
    return stat;
}

//PRIVATE IMPLEMENTATION
SGTextNodePrivate::SGTextNodePrivate() :
    m_color(0, 0, 0)
{
    m_valignment = VAlignment::AlignVCenter;
    m_halignment = HAlignment::AlignHCenter;
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
Size SGTextNodePrivate::setText(const std::wstring & txt)
{
    m_glyphLines.clear();
    if (txt.empty()) return Size();

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

    //to do...
    return Size{ m_boundingWidth, m_boundingHeight };
}

void SGTextNodePrivate::reAlign()
{
    int chidx = 0;
    int posY = 0;
    switch (m_valignment)
    {
    case VAlignment::AlignTop:
        posY = 0;
        break;
    case VAlignment::AlignVCenter:
        posY = (q_ptr->height() - m_boundingHeight) / 2;
        break;
    case VAlignment::AlignBottom:
        posY = q_ptr->height() - m_boundingHeight;
        break;
    }

    for (int i = 0; i < m_glyphLines.size(); i++)
    {
        auto &lineinf = m_glyphLines.at(i);

        int posX = 0;
        switch (m_halignment)
        {
        case HAlignment::AlignLeft:
            posX = 0;
            break;
        case HAlignment::AlignHCenter:
            posX = (q_ptr->width() - lineinf.width) / 2;
            break;
        case HAlignment::AlignRight:
            posX = q_ptr->width() - lineinf.width;
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

    q_ptr->markDirty(SGNode::E_DirtyType::SizeDirty);
}