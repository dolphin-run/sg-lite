#ifndef SGTEXTURE_H
#define SGTEXTURE_H
#include <list>
#include <assert.h>

#include "sgstruct.h"
#include <unordered_map>

struct TexBuffer {
    int pxWid = 0;
    int pxHei = 0;
    int pxComp = 0;
    void* data = nullptr;

    virtual ~TexBuffer();
    virtual void init(int wid, int hei, int comp);

private:
    void cleanup();
};

class SGTexture
{
public:
    SGTexture();
    virtual ~SGTexture();
    virtual void build() = 0;
    virtual void bind() = 0;
    virtual Size size() const = 0;

    virtual unsigned add(const Image &img) = 0;
    virtual bool update(unsigned key, const Image &img) = 0;
    const RectF& retrieve(unsigned key) const;

protected:
    TexBuffer *m_texBuffer = nullptr;
    std::unordered_map<unsigned, RectF> m_rects;
    unsigned keyUid = 1;
    static RectF g_invalidRect;
};

#endif // SGTEXTURE_H
