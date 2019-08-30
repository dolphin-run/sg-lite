#ifndef SGSTRUCT_H
#define SGSTRUCT_H

#include <cfloat>
#include <assert.h>
#include <memory>
#include "sgglobal.h"

struct SG_EXPORT Point {
    int x, y;

    void set(int nx, int ny) {
        x = nx;
        y = ny;
    }

    Point operator- (const Point &r) {
        Point val;
        val.x = x - r.x;
        val.y = y - r.y;
        return val;
    }
};

struct SG_EXPORT PointF {
    float x, y;

    void set(float nx, float ny) {
        x = nx;
        y = ny;
    }

    PointF operator- (const PointF &r) {
        PointF val;
        val.x = x - r.x;
        val.y = y - r.y;
        return val;
    }
};

struct Size {
    int width;
    int height;
};

struct SG_EXPORT Rect {
    Point tl, br; // Top-Left (min) and Bottom-Right (max)

    Rect() {
        tl.x = 0;
        tl.y = 0;
        br.x = 0;
        br.y = 0;
    }
    Rect(int x, int y, int wid, int hei) {
        tl.x = x;
        tl.y = y;
        br.x = x + wid;
        br.y = y + hei;
    }

    Point center() {
        return Point{ (tl.x + br.x) / 2, (tl.y + br.y) / 2 };
    }

    void move(int x, int y) {
        tl.x += x;
        tl.y += y;
        br.x += x;
        br.y += y;
    }
    void move(const Point &p) {
        move(p.x, p.y);
    }

    void operator |= (const Point &pt) {
        if (pt.x < tl.x)
            tl.x = pt.x;
        if (pt.x > br.x)
            br.x = pt.x;
        if (pt.y < tl.y)
            tl.y = pt.y;
        if (pt.y > br.y)
            br.y = pt.y;
    }

    void operator |= (const Rect &r) {
        if (r.tl.x < tl.x)
            tl.x = r.tl.x;
        if (r.tl.y < tl.y)
            tl.y = r.tl.y;
        if (r.br.x > br.x)
            br.x = r.br.x;
        if (r.br.y > br.y)
            br.y = r.br.y;
    }

    void set(int left, int top, int right, int bottom) {
        tl.set(left, top);
        br.set(right, bottom);
    }

    bool intersects(const Rect &r) {
        bool xOverlap = r.tl.x < br.x && r.br.x > tl.x;
        bool yOverlap = r.tl.y < br.y && r.br.y > tl.y;
        return xOverlap && yOverlap;
    }
};

struct SG_EXPORT RectF {
    PointF tl, br; // Top-Left (min) and Bottom-Right (max)

    RectF() {
        tl.x = 0;
        tl.y = 0;
        br.x = 0;
        br.y = 0;
    }
    RectF(float x, float y, float wid, float hei) {
        tl.x = x;
        tl.y = y;
        br.x = x + wid;
        br.y = y + hei;
    }

    PointF center() {
        return PointF{ (tl.x + br.x) / 2, (tl.y + br.y) / 2 };
    }

    void move(float x, float y) {
        tl.x += x;
        tl.y += y;
        br.x += x;
        br.y += y;
    }
    void move(const PointF &p) {
        move(p.x, p.y);
    }

    void operator |= (const PointF &pt) {
        if (pt.x < tl.x)
            tl.x = pt.x;
        if (pt.x > br.x)
            br.x = pt.x;
        if (pt.y < tl.y)
            tl.y = pt.y;
        if (pt.y > br.y)
            br.y = pt.y;
    }

    void operator |= (const RectF &r) {
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

    void set(float left, float top, float right, float bottom) {
        tl.set(left, top);
        br.set(right, bottom);
    }

    bool intersects(const RectF &r) {
        bool xOverlap = r.tl.x < br.x && r.br.x > tl.x;
        bool yOverlap = r.tl.y < br.y && r.br.y > tl.y;
        return xOverlap && yOverlap;
    }

    Rect toRect() const {
        Rect rec;
        rec.set(tl.x, tl.y, br.x, br.y);
        return rec;
    }
};

struct SG_EXPORT Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0)
    {
    }

    Color(uint32_t x) {
        b = x & 0xff;
        x >>= 8;
        g = x & 0xff;
        x >>= 8;
        r = x & 0xff;
    }
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    static Color random();
};

struct SG_EXPORT Image {
    //       1           grey
    //       2           grey, alpha
    //       3           red, green, blue
    //       4           red, green, blue, alpha
    enum ImageFormat {
        Format_Alpha = 1,
        Format_RA,
        Format_RGB,
        Format_RGBA
    };
    Image();
    Image(uint32_t w, uint32_t h, const uint8_t *px, ImageFormat fmt);

    bool valid = false;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t component = 0;
    std::shared_ptr<uint8_t> data = nullptr;
    ImageFormat format;
};

#endif // SGSTRUCT_H
