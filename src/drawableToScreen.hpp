#ifndef DRAWBLETOSCREEN_HPP
#define DRAWBLETOSCREEN_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glvbo.hpp"
#include "uniforms.hpp"

class Program;

class DrawableToScreen
{
public:
    DrawableToScreen();
    void setProgram(Program* program);
    bool AttachGlObjToAttrib(MappedGlObject* uniform/*, const char* attribName*/);
    bool DetachGlObj(MappedGlObject* uniform);

    void Draw();

private:
    std::string mAttribName;

    typedef std::vector<MappedGlObject*> MappedGlObjectContainer;
    MappedGlObjectContainer mMappedObjects;

    Program* mProgram;

    void enableAllMappedVbos();
    void disableAllMappedVbos();
};

#endif
