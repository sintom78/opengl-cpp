#include <iostream>
#include "drawableToTexture.hpp"
#include "shader.hpp"


DrawableToTexture::DrawableToTexture(GLuint width, GLuint height):
    mFramebufferName(0), mRenderedTexture(0), mProgram(NULL)
{
    glGenFramebuffers(1, &mFramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferName);
    //GLuint renderedTexture;
    glGenTextures(1, &mRenderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, mRenderedTexture);

    // Give an empty image to OpenGL ( the last "0" means "empty" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // The depth buffer
    if ( !GLEW_ARB_framebuffer_object ){ // OpenGL 2.1 doesn't require this, 3.1+ does
        std::cout << "Your GPU does not provide framebuffer objects. Use a texture instead." << std::endl;
        throw std::exception();
    }

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderedTexture, 0);

    // Set the list of draw buffers.
    /*
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    */

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::exception();

}

void DrawableToTexture::setProgram(Program* program)
{
    mProgram = program;
}

bool DrawableToTexture::AttachVboToAttrib(GlVbo* vbo, const char* attribName)
{
    mMappedVbos.push_back(
                MappedGlVbo(
                    vbo,
                    mProgram->getGlId(),
                    attribName));
}

bool DrawableToTexture::DetachVbo(GlVbo* Vbo)
{
//TODO: implement
}

void DrawableToTexture::Draw()
{
    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferName);
//        glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mRenderedTexture);

    // Use our shader
    mProgram->use();

    glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    enableAllMappedVbos();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    disableAllMappedVbos();
}

void DrawableToTexture::enableAllMappedVbos()
{
    for( MappedGlVboContainer::iterator it=mMappedVbos.begin(); it!= mMappedVbos.end(); it++)
    {
        it->enable();
    }
}

void DrawableToTexture::disableAllMappedVbos()
{
    for( MappedGlVboContainer::iterator it=mMappedVbos.begin(); it!= mMappedVbos.end(); it++)
    {
        it->disable();
    }
}
