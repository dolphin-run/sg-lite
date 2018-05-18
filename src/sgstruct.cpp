#include "sgstruct.h"

void Point::set(int nx, int ny) {
    x = nx;
    y = ny;
}

Point Point::operator- (const Point &r) {
    Point val;
    val.x = x - r.x;
    val.y = y - r.y;
    return val;
}

void PointF::set(float nx, float ny) {
    x = nx;
    y = ny;
}

PointF PointF::operator- (const PointF &r) {
    PointF val;
    val.x = x - r.x;
    val.y = y - r.y;
    return val;
}

Rect::Rect() {
    tl.x = 0;
    tl.y = 0;
    br.x = 0;
    br.y = 0;
}
Rect::Rect(int x, int y, int wid, int hei) {
    tl.x = x;
    tl.y = y;
    br.x = x + wid;
    br.y = y + hei;
}

Point Rect::center() {
    return Point{ (tl.x + br.x) / 2, (tl.y + br.y) / 2 };
}

void Rect::move(int x, int y) {
    tl.x += x;
    tl.y += y;
    br.x += x;
    br.y += y;
}
void Rect::move(const Point &p) {
    move(p.x, p.y);
}

void Rect::operator |= (const Point &pt) {
    if (pt.x < tl.x)
        tl.x = pt.x;
    if (pt.x > br.x)
        br.x = pt.x;
    if (pt.y < tl.y)
        tl.y = pt.y;
    if (pt.y > br.y)
        br.y = pt.y;
}

void Rect::operator |= (const Rect &r) {
    if (r.tl.x < tl.x)
        tl.x = r.tl.x;
    if (r.tl.y < tl.y)
        tl.y = r.tl.y;
    if (r.br.x > br.x)
        br.x = r.br.x;
    if (r.br.y > br.y)
        br.y = r.br.y;
}

void Rect::set(int left, int top, int right, int bottom) {
    tl.set(left, top);
    br.set(right, bottom);
}

bool Rect::intersects(const Rect &r) {
    bool xOverlap = r.tl.x < br.x && r.br.x > tl.x;
    bool yOverlap = r.tl.y < br.y && r.br.y > tl.y;
    return xOverlap && yOverlap;
}

//
RectF::RectF() {
    tl.x = 0;
    tl.y = 0;
    br.x = 0;
    br.y = 0;
}
RectF::RectF(float x, float y, float wid, float hei) {
    tl.x = x;
    tl.y = y;
    br.x = x + wid;
    br.y = y + hei;
}

PointF RectF::center() {
    return PointF{ (tl.x + br.x) / 2, (tl.y + br.y) / 2 };
}

void RectF::move(float x, float y) {
    tl.x += x;
    tl.y += y;
    br.x += x;
    br.y += y;
}
void RectF::move(const PointF &p) {
    move(p.x, p.y);
}

void RectF::operator |= (const PointF &pt) {
    if (pt.x < tl.x)
        tl.x = pt.x;
    if (pt.x > br.x)
        br.x = pt.x;
    if (pt.y < tl.y)
        tl.y = pt.y;
    if (pt.y > br.y)
        br.y = pt.y;
}

void RectF::operator |= (const RectF &r) {
    if (r.tl.x < tl.x)
        tl.x = r.tl.x;
    if (r.tl.y < tl.y)
        tl.y = r.tl.y;
    if (r.br.x > br.x)
        br.x = r.br.x;
    if (r.br.y > br.y)
        br.y = r.br.y;
}

//void map(const glm::mat4 &m);

void RectF::set(float left, float top, float right, float bottom) {
    tl.set(left, top);
    br.set(right, bottom);
}

bool RectF::intersects(const RectF &r) {
    bool xOverlap = r.tl.x < br.x && r.br.x > tl.x;
    bool yOverlap = r.tl.y < br.y && r.br.y > tl.y;
    return xOverlap && yOverlap;
}

Rect RectF::toRect() const {
    Rect rec;
    rec.set(tl.x, tl.y, br.x, br.y);
    return rec;
}

//Color
Color::Color() : r(0), g(0), b(0)
{}

Color::Color(uint32_t x) {
    b = x & 0xff;
    x >>= 8;
    g = x & 0xff;
    x >>= 8;
    r = x & 0xff;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) 
{}

bool Color::operator==(const Color & c)
{
    return r == c.r && g == c.g && b == c.b && a == c.a;
}

Color Color::random()
{
    unsigned char r = rand() % 256;
    unsigned char g = rand() % 256;
    unsigned char b = rand() % 256;
    return Color(r, g, b);
}

//Image
Image::Image() {}

Image::Image(uint32_t w, uint32_t h, const uint8_t * px, ImageFormat fmt):
    width(w),
    height(h),
    format(fmt),
    valid(true)
{
    switch (fmt)
    {
    case Image::Format_Alpha:
        component = 1;
        break;
    case Image::Format_RGB:
        component = 3;
        break;
    case Image::Format_RGBA:
        component = 4;
        break;
    default:
        valid = false;
        break;
    }

    if (valid)
    {
        //uint32_t size = width*height*component;
        //uint8_t *p = (uint8_t*)malloc(size);
        //memcpy(p, px, size);
        //data = std::shared_ptr<uint8_t>(p, free);
        data = px;
    }
}
