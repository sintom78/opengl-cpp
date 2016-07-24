#include <iostream>
#include "drawableToScreen.hpp"
#include "glm/glm.hpp"
#include "shader.hpp"


DrawableToScreen::DrawableToScreen():
    mProgram(NULL)
{

}

void DrawableToScreen::setProgram(Program* program)
{
    mProgram = program;
}

bool DrawableToScreen::AttachGlObjToAttrib(MappedGlObject* unif/*, const char* attribName*/)
{
    mMappedObjects.push_back(unif);
}

bool DrawableToScreen::DetachGlObj(MappedGlObject* unif)
{
//TODO: implement
}

void DrawableToScreen::Draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //draw to screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (mProgram)
        mProgram->use();

    enableAllMappedVbos();
    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    disableAllMappedVbos();
}


void DrawableToScreen::enableAllMappedVbos()
{
    for( MappedGlObjectContainer::iterator it=mMappedObjects.begin(); it!= mMappedObjects.end(); it++)
    {
        (*it)->enable();
    }
}

void DrawableToScreen::disableAllMappedVbos()
{
    for( MappedGlObjectContainer::iterator it=mMappedObjects.begin(); it!= mMappedObjects.end(); it++)
    {
        (*it)->disable();
    }
}
