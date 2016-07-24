#include <exception>
#include <iostream>
#include "uniforms.hpp"


MappedGlUniform1<GLint>::MappedGlUniform1(GLint program, const char *name)
    :MappedGlUniformBase(program,name,UNIFORMTYPE_INT1),mV0(0)
{
}

void MappedGlUniform1<GLint>::setValue(GLint v0)
{
    mV0=v0;
}

void MappedGlUniform1<GLint>::enable()
{
    glUniform1i(getGlId(), mV0);
}

void MappedGlUniform1<GLint>::disable()
{

}

MappedGlTexture::MappedGlTexture(GLint program, const char* name, GLint textureUnit, GLint textureGlId)
    :MappedGlUniform1(program, name), mTextureUnit(textureUnit), mTexture(textureGlId)
{
    if ((mTextureUnit<GL_TEXTURE0) || (mTextureUnit>GL_TEXTURE31))
        throw std::exception();

    mTextureUnitId = mTextureUnit - GL_TEXTURE0;
}

void MappedGlTexture::enable()
{
    glActiveTexture(mTextureUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glUniform1i(getGlId(),mTextureUnitId);
}


void MappedGlTexture::disable()
{

}

MappedGlUniform1<GLfloat>::MappedGlUniform1(GLint program, const char* name, GLfloat v0)
    :MappedGlUniformBase(program,name,UNIFORMTYPE_FLOAT1),mV0(v0)
{
}

void MappedGlUniform1<GLfloat>::setValue(GLfloat v0)
{
    mV0=v0;
}

void MappedGlUniform1<GLfloat>::enable()
{
    glUniform1f(getGlId(), mV0);
}

void MappedGlUniform1<GLfloat>::disable()
{

}

MappedGlUniform4<glm::mat4*>::MappedGlUniform4(GLuint program, const char *name)
    :MappedGlUniformBase(program,name,UNIFORMTYPE_FLOAT1),mpGlmMat4(NULL)
{}


void MappedGlUniform4<glm::mat4*>::setValue(glm::mat4* pGlmMat4)
{
    mpGlmMat4=pGlmMat4;
}

void MappedGlUniform4<glm::mat4*>::enable()
{
    glUniformMatrix4fv(getGlId(), 1, GL_FALSE, &(*mpGlmMat4)[0][0]);
}

void MappedGlUniform4<glm::mat4*>::disable()
{

}
