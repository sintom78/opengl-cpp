#ifndef UNIFORMS_HPP
#define UNIFORMS_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

enum eUniformType {
    UNIFORMTYPE_INT1,
    UNIFORMTYPE_INT2,
    UNIFORMTYPE_INT3,
    UNIFORMTYPE_INT4,
    UNIFORMTYPE_UINT1,
    UNIFORMTYPE_UINT2,
    UNIFORMTYPE_UINT3,
    UNIFORMTYPE_UINT4,
    UNIFORMTYPE_FLOAT1
};


class MappedGlObject
{
public:
    virtual ~MappedGlObject(){}
    virtual void enable(){}
    virtual void disable(){}
};

class MappedGlUniformBase:public MappedGlObject
{
public:
    MappedGlUniformBase(GLuint program, const char* name, eUniformType type)
    {
        mGlId = glGetUniformLocation(program, name);
        mGlProgram = program;
        mType=type;
    }

    virtual ~MappedGlUniformBase()
    {}

    virtual GLint getGlId()
    { return mGlId; }


protected:
    void setGlId(GLint id)
    { mGlId = id; }

private:
    GLint mGlId;
    GLuint mGlProgram;
    eUniformType mType;
};

template<typename GLTYPE> class MappedGlUniform1;

template<> class MappedGlUniform1<GLint>:public MappedGlUniformBase
{
public:
    MappedGlUniform1(GLint program, const char* name);
    void setValue(GLint v0);
    virtual void enable();
    virtual void disable();

private:
    GLint mV0;
};

template<> class MappedGlUniform1<GLfloat>:public MappedGlUniformBase
{
public:
    MappedGlUniform1(GLint program, const char* name, GLfloat v0);
    void setValue(GLfloat v0);
    virtual void enable();
    virtual void disable();

private:
    GLfloat mV0;
};

class MappedGlTexture:public MappedGlUniform1<GLint>
{
public:
    MappedGlTexture(GLint program, const char* name, GLint textureUnitId, GLint textureGlId);
    virtual void enable();
    virtual void disable();
private:
    GLint mTextureUnit;
    GLint mTexture;
    GLint mTextureUnitId;
};

template<typename GLTYPE> class MappedGlUniform4;

template<> class MappedGlUniform4<glm::mat4*>:public MappedGlUniformBase
{
public:
    MappedGlUniform4(GLuint program, const char *name);
    void setValue(glm::mat4* pGlmMat4);
    virtual void enable();
    virtual void disable();

private:
    glm::mat4 *mpGlmMat4;
};

#endif
