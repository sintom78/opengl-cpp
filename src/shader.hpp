#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

class Shader
{
public:
    Shader(const char *src_file, GLint shaderType);
    GLuint getGlId() const { return mShaderId; }
private:
    GLuint mShaderId;

    void loadsrc(const char *src_file_path, std::string& out_shaderCode);
    void compile(const char *src_file_path, const std::string& shaderCode, GLint shaderType);
    void checkCompilation();
};

class Program
{
public:
    Program();
    void attachShader(const Shader& shader);
    void attachShader(const char *src_file, GLint shaderType);
    void link();
    void use() { glUseProgram(mProgramId); }
    GLuint getGlId() const { return mProgramId; }
private:
    bool mHasShaders;
    bool mIsLinked;
    GLuint mProgramId;
};

#endif // SHADER_HPP
