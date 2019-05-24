#ifndef GRAPH2D_H
#define GRAPH2D_H

#include "sgnode.h"

class Graph2D
{
public:
    Graph2D();
};

class GEntity
{
    SGNode *m_background;
    SGNode *m_icon;
    SGNode *m_label;
};

class GLink
{
    SGNode *m_line;
    SGNode *m_startArrow;
    SGNode *m_endArrow;
    SGNode *m_label;
};

#endif // GRAPH2D_H
