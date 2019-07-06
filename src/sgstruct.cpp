#include "sgstruct.h"

struct QMatrix4x4_Accessor
{
    float m[4][4];
    int flagBits;

    static bool isTranslate(const QMatrix4x4 &m) { return ((const QMatrix4x4_Accessor &)m).flagBits <= 0x1; }
    static bool isScale(const QMatrix4x4 &m) { return ((const QMatrix4x4_Accessor &)m).flagBits <= 0x2; }
    static bool is2DSafe(const QMatrix4x4 &m) { return ((const QMatrix4x4_Accessor &)m).flagBits < 0x8; }
};

void Rect::map(const QMatrix4x4 &matrix)
{
    const float *m = matrix.constData();
    if (QMatrix4x4_Accessor::isScale(matrix)) {
        tl.x = tl.x * m[0] + m[12];
        tl.y = tl.y * m[5] + m[13];
        br.x = br.x * m[0] + m[12];
        br.y = br.y * m[5] + m[13];
        if (tl.x > br.x)
            qSwap(tl.x, br.x);
        if (tl.y > br.y)
            qSwap(tl.y, br.y);
    }
    else {
        Point mtl = tl;
        Point mtr = { br.x, tl.y };
        Point mbl = { tl.x, br.y };
        Point mbr = br;

        mtl.map(matrix);
        mtr.map(matrix);
        mbl.map(matrix);
        mbr.map(matrix);

        set(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
        (*this) |= mtl;
        (*this) |= mtr;
        (*this) |= mbl;
        (*this) |= mbr;
    }
}

void RectF::map(const QMatrix4x4 & matrix)
{
    const float *m = matrix.constData();
    if (QMatrix4x4_Accessor::isScale(matrix)) {
        tl.x = tl.x * m[0] + m[12];
        tl.y = tl.y * m[5] + m[13];
        br.x = br.x * m[0] + m[12];
        br.y = br.y * m[5] + m[13];
        if (tl.x > br.x)
            qSwap(tl.x, br.x);
        if (tl.y > br.y)
            qSwap(tl.y, br.y);
    }
    else {
        PointF mtl = tl;
        PointF mtr = { br.x, tl.y };
        PointF mbl = { tl.x, br.y };
        PointF mbr = br;

        mtl.map(matrix);
        mtr.map(matrix);
        mbl.map(matrix);
        mbr.map(matrix);

        set(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
        (*this) |= mtl;
        (*this) |= mtr;
        (*this) |= mbl;
        (*this) |= mbr;
    }
}

Color Color::random()
{
    unsigned char r = rand() % 256;
    unsigned char g = rand() % 256;
    unsigned char b = rand() % 256;
    return Color(r, g, b);
}

Image::Image() {}

Image::Image(uint32_t w, uint32_t h, const uint8_t * px, E_ImageFormat fmt):
    width(w),
    height(h),
    format(fmt),
    valid(true)
{
    switch (fmt)
    {
    case Image::Format_RGBA:
        component = 4;
        break;
    case Image::Format_Alpha:
        component = 1;
        break;
    default:
        valid = false;
        break;
    }

    if (valid)
    {
        uint32_t size = width*height*component;
        uint8_t *p = (uint8_t*)malloc(size);
        memcpy(p, px, size);
        data = std::make_shared<uint8_t*>(p);
    }
}
