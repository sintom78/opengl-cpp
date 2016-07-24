#include "glvbo.hpp"

MappedGlVbo::MappedGlVbo(GlVbo* vbo, GLuint programID, const char* attribName): mVbo(vbo), mIsEnabled(false)
{
    mAttribId = glGetAttribLocation(programID, attribName);
}

void MappedGlVbo::enable()
{
    glBindBuffer(GL_ARRAY_BUFFER, mVbo->GetglID());
    glVertexAttribPointer(
        mAttribId,  // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );
    glEnableVertexAttribArray(mAttribId);
    mIsEnabled = true;
}

void MappedGlVbo::disable()
{
    glDisableVertexAttribArray(mAttribId);
    mIsEnabled = false;
}


GlVbo::GlVbo(GLsizeiptr size,
             const GLvoid *data,
             GLenum usage):mGlBufferId(0)
{
    glGenBuffers(1, &mGlBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, mGlBufferId);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
