#ifndef GLVBO_HPP
#define GLVBO_HPP

#include <GL/glew.h>
#include "uniforms.hpp"

//class DrawableToTexture;
//class DrawableToScreen;


class GlVbo
{
public:
    GlVbo(GLsizeiptr size, const GLvoid *data, GLenum usage = GL_STATIC_DRAW);

    GLuint GetglID() const {
        return mGlBufferId;
    }

    void enable();
    void disable();

private:
    GLuint mGlBufferId;

    //Might be called by DrawableToTexture::Draw()
    //friend class DrawableToTexture;
};

class MappedGlVbo: public MappedGlObject
{
public:
    MappedGlVbo(GlVbo* vbo, GLuint programID, const char* attribName);

    void enable();
    void disable();

private:
    GlVbo *mVbo;
    GLuint mAttribId;
    bool mIsEnabled;
};

#endif
