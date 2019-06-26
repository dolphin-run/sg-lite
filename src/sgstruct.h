#ifndef SGSTRUCT_H
#define SGSTRUCT_H

#include <QMatrix4x4>

struct Pt {
    float x, y;

    void map(const QMatrix4x4 &mat) {
        Pt r;
        const float *m = mat.constData();
        r.x = x * m[0] + y * m[4] + m[12];
        r.y = x * m[1] + y * m[5] + m[13];
        x = r.x;
        y = r.y;
    }

    void set(float nx, float ny) {
        x = nx;
        y = ny;
    }
};

inline QDebug operator << (QDebug d, const Pt &p) {
    d << "Pt(" << p.x << p.y << ")";
    return d;
}



struct Rect {
    Pt tl, br; // Top-Left (min) and Bottom-Right (max)

    void operator |= (const Pt &pt) {
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

    void map(const QMatrix4x4 &m);

    void set(float left, float top, float right, float bottom) {
        tl.set(left, top);
        br.set(right, bottom);
    }

    bool intersects(const Rect &r) {
        bool xOverlap = r.tl.x < br.x && r.br.x > tl.x;
        bool yOverlap = r.tl.y < br.y && r.br.y > tl.y;
        return xOverlap && yOverlap;
    }

    bool isOutsideFloatRange() const {
        return tl.x < -FLT_EPSILON
            || tl.y < -FLT_EPSILON
            || br.x > FLT_EPSILON
            || br.y > FLT_EPSILON;
    }
};

inline QDebug operator << (QDebug d, const Rect &r) {
    d << "Rect(" << r.tl.x << r.tl.y << r.br.x << r.br.y << ")";
    return d;
}

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    //uint8_t a;
};

#endif // SGSTRUCT_H
