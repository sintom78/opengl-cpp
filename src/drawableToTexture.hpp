#ifndef DRAWBLETOTEXTURE_HPP
#define DRAWBLETOTEXTURE_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glvbo.hpp"

class Program;

class DrawableToTexture
{
public:
    DrawableToTexture(GLuint width, GLuint height);
    void setProgram(Program* program);
    bool AttachVboToAttrib(GlVbo* mappedVbo, const char* attribName);
    bool DetachVbo(GlVbo* mappedVbo);

    GLuint GetTextureId() const {
        return mRenderedTexture;
    }

    void Draw();

private:
    std::string mAttribName;

    typedef std::vector<MappedGlVbo> MappedGlVboContainer;
    MappedGlVboContainer mMappedVbos;

    GLuint mFramebufferName;
    GLuint mRenderedTexture;
    Program* mProgram;

    void enableAllMappedVbos();
    void disableAllMappedVbos();
};

#endif
