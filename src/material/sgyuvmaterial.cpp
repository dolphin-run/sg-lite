#include "sgyuvmaterial.h"
#include <algorithm>


SYUVInfo::SYUVInfo(int pixWid, int pixHei)
{
    update(pixWid, pixHei);
}

bool SYUVInfo::update(int pixWid, int pixHei)
{
    m_wid = pixWid;
    m_hei = pixHei;
    m_semiWid = m_wid >> 1;
    m_semiHei = m_hei >> 1;

    m_yWid = pixWid;
    m_yHei = pixHei;
    m_uWid = (pixWid + 1) >> 1;
    m_uHei = (pixHei + 1) >> 1;

    m_yLen = m_yWid*m_yHei;
    m_uLen = m_uWid*m_uHei;
    m_len = m_yLen + m_uLen * 2;

    m_yuvRadio = m_semiHei / m_semiWid;

    return true;
}


class SGYuvMaterialShader : public SGMaterialShader
{
public:
    SGYuvMaterialShader() { build(); }
    ~SGYuvMaterialShader() {}

    virtual void updateState(const float *mat4, const SGNodeState *stat) override
    {
        m_program.setMat4(m_matrixUniform, mat4);

        m_program.setInt(m_texYUniform, 0);
        m_program.setInt(m_texUUniform, 1);
        m_program.setInt(m_texVUniform, 2);
    }

    virtual const char* vertexShader() const override
    {
        return  "attribute vec4 posAttr;\n"
            "attribute vec4 texAttr;\n"
            "varying vec4 tex;\n"
            "uniform mat4 matrix;\n"
            "void main() {\n"
            "   tex = texAttr;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";
    }

    virtual const char* fragmentShader() const override
    {
        return  "varying vec4 tex;\n"
            "uniform sampler2D tex_y;\n"
            "uniform sampler2D tex_u;\n"
            "uniform sampler2D tex_v;\n"
            "void main() {\n"
                "vec3 yuv;\n"
                "vec3 rgb;\n"
                "yuv.x = texture2D(tex_y, vec2(tex.x, tex.y)).r;\n"
                "yuv.y = texture2D(tex_u, vec2(tex.x, tex.y)).r - 0.5;\n"
                "yuv.z = texture2D(tex_v, vec2(tex.x, tex.y)).r - 0.5;\n"
                "\n"
                "//yuv420p\n"
                "rgb = mat3(1.0, 1.0, 1.0,\n"
                "    0.0, -0.395, 2.032,\n"
                "    1.140, -0.581, 0.0) * yuv;\n"
                "gl_FragColor = vec4(rgb, 1.0); \n"
            "}\n";
    }

    virtual char const* const* attributeNames() const override
    {
        static const char* const attr[] = { "posAttr", "texAttr", 0 };
        return attr;
    }

    bool build()
    {
        compile();
        m_matrixUniform = m_program.uniformLocation("matrix");
        m_texYUniform = m_program.uniformLocation("tex_y");
        m_texUUniform = m_program.uniformLocation("tex_u");
        m_texVUniform = m_program.uniformLocation("tex_v");

        return true;
    }
private:
    unsigned m_matrixUniform;
    unsigned m_texYUniform;
    unsigned m_texUUniform;
    unsigned m_texVUniform;
    const SGYuvMaterial *m_parent;
};

SGYuvMaterial::SGYuvMaterial()
{
    static SYUVInfo info;
    m_yuvInfo = &info;
    m_yuvChanged = 0;
}

E_MaterialType SGYuvMaterial::type() const
{
    return E_MaterialType::YuvMaterialType;
}

SGMaterialShader * SGYuvMaterial::createShader() const
{
    return new SGYuvMaterialShader;
}

void SGYuvMaterial::setYuvInfo(const SYUVInfo * info)
{
    std::lock_guard<std::mutex> guard(m_yuvLock);

    m_yuvInfo = info;

    //realloc
    if (m_yuvBufSize < info->m_len)
    {
        delete[]m_yuvBuf;
        m_yuvBufSize = info->m_len;
        m_yuvBuf = new unsigned char[m_yuvBufSize];
    }
}

void SGYuvMaterial::setYuv(const unsigned char * src)
{
    std::lock_guard<std::mutex> guard(m_yuvLock);

    if (src)
    {
        memcpy(m_yuvBuf, src, m_yuvInfo->m_len);
    }
    else
    {
        memset(m_yuvBuf, 0, m_yuvInfo->m_len);
    }
    m_yuvChanged = 1;
}

void SGYuvMaterial::syncState()
{
    m_tex.bind();
    if (!m_yuvChanged) return;

    std::lock_guard<std::mutex> guard(m_yuvLock);
    m_yuvChanged = 0;

    auto const *ptr = m_yuvBuf;
    m_tex.update(SGYuvTexture::Y_Component, Image(m_yuvInfo->m_yWid, m_yuvInfo->m_yHei, ptr, Image::Format_Alpha));

    ptr += m_yuvInfo->m_yLen;
    m_tex.update(SGYuvTexture::U_Component, Image(m_yuvInfo->m_uWid, m_yuvInfo->m_uHei, ptr, Image::Format_Alpha));

    ptr += m_yuvInfo->m_uLen;
    m_tex.update(SGYuvTexture::V_Component, Image(m_yuvInfo->m_uWid, m_yuvInfo->m_uHei, ptr, Image::Format_Alpha));
}

SGTexture * SGYuvMaterial::texture() const
{
    return &m_tex;
}
