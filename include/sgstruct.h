#ifndef SGSTRUCT_H
#define SGSTRUCT_H

#include <cfloat>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <cstdlib>

#include "sgglobal.h"

struct SG_DECL_EXPORT Point {
    int x, y;

    void set(int nx, int ny);
    Point operator- (const Point &r);
};

struct SG_DECL_EXPORT PointF {
    float x, y;

    void set(float nx, float ny);
    PointF operator- (const PointF &r);
};

struct Size {
    int width;
    int height;
};

struct SG_DECL_EXPORT Rect {
    Point tl, br; // Top-Left (min) and Bottom-Right (max)

    Rect();
    Rect(int x, int y, int wid, int hei);

    Point center();
    void move(int x, int y);
    void move(const Point &p);
    void operator |= (const Point &pt);
    void operator |= (const Rect &r);
    void set(int left, int top, int right, int bottom);
    bool intersects(const Rect &r);
};

struct SG_DECL_EXPORT RectF {
    PointF tl, br; // Top-Left (min) and Bottom-Right (max)

    RectF();
    RectF(float x, float y, float wid, float hei);

    PointF center();
    void move(float x, float y);
    void move(const PointF &p);
    void operator |= (const PointF &pt);
    void operator |= (const RectF &r);
    //void map(const glm::mat4 &m);
    void set(float left, float top, float right, float bottom);
    bool intersects(const RectF &r);
    Rect toRect() const;
};

struct SG_DECL_EXPORT Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color();
    Color(uint32_t rgb);
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    bool operator==(const Color &c) const;

    static Color random();
};

enum HAlignment { AlignLeft, AlignHCenter, AlignRight };
enum VAlignment { AlignTop, AlignVCenter, AlignBottom };

struct SG_DECL_EXPORT Image {
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
    //`px` should be valid util Image destroyed.
    Image(uint32_t w, uint32_t h, const uint8_t *px, ImageFormat fmt);

    bool valid = false;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t component = 0;
    const uint8_t *data = nullptr;
    ImageFormat format;
};

struct SGNodeState
{
    Color color;
    float opacity;
};


enum SGYuvFormat { YUV_420P, YUV_420SP };

#endif // SGSTRUCT_H
