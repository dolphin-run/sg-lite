#include "sgstruct.h"


Color Color::random()
{
    unsigned char r = rand() % 256;
    unsigned char g = rand() % 256;
    unsigned char b = rand() % 256;
    return Color(r, g, b);
}

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
        uint32_t size = width*height*component;
        uint8_t *p = (uint8_t*)malloc(size);
        memcpy(p, px, size);
        data = std::shared_ptr<uint8_t>(p, free);
    }
}
