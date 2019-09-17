#include "sgtexturematerial.h"
#include "texture/sggridimagetexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>

static std::unordered_map<std::string, unsigned> g_nameMapToKey;
static SGGridImageTexture g_imgTex(2048, 2048, Image::Format_RGBA, 256, 256);

class SGTextureMaterialShader : public SGMaterialShader
{
public:
    SGTextureMaterialShader(const SGTextureMaterial *parent) : m_parent(parent) { build(); }
    ~SGTextureMaterialShader() {}

    virtual void updateState(const float *mat4, const SGNodeState *stat) override
    {
        m_program.setMat4(m_matrixUniform, mat4);

        m_parent->texture()->bind();
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
            "uniform sampler2D uSampler;\n"
            "void main() {\n"
            "   gl_FragColor = texture2D(uSampler, vec2(tex.x, tex.y)); \n"
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

        return true;
    }
private:
    unsigned m_matrixUniform;
    const SGTextureMaterial *m_parent;
};

SGTextureMaterial::SGTextureMaterial()
{
    m_tex = &g_imgTex;
}

E_MaterialType SGTextureMaterial::type() const
{
    return E_MaterialType::TextureMaterialType;
}

SGMaterialShader * SGTextureMaterial::createShader() const
{
    return new SGTextureMaterialShader(this);
}

const RectF& SGTextureMaterial::setImage(const char * path)
{
    unsigned uid = 0;
    if (g_nameMapToKey.count(path))
    {
        uid = g_nameMapToKey.at(path);
    }
    else
    {
        //come new image
        int x = 0, y = 0, n = 0;
        unsigned char *data = stbi_load(path, &x, &y, &n, 0);
        if (!data)
        {
            printf("image open failed...[%s]\n", path);
        }
        else
        {
            Image img(x, y, data, (Image::ImageFormat)n);
            uid = g_imgTex.add(img);

            //update cache
            g_nameMapToKey.emplace(std::make_pair(path, uid));
        }
    }

    return g_imgTex.retrieve(uid);
}

SGTexture * SGTextureMaterial::texture() const
{
    return m_tex;
}
