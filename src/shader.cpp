#include <exception>
#include <iostream>
#include <vector>
#include <fstream>
#include "shader.hpp"

Shader::Shader(const char *src_file_path, GLint shaderType)
{
    std::string shaderSrc;
    loadsrc(src_file_path,shaderSrc);
    compile(src_file_path,shaderSrc, shaderType);
    checkCompilation();
}


void Shader::loadsrc(const char *src_file_path, std::string& out_shaderCode)
{
    std::ifstream shaderStream(src_file_path, std::ios::in);
    if(shaderStream.is_open()){
        std::string line = "";
        while(getline(shaderStream, line))
            out_shaderCode += "\n" + line;
        //TODO: get attributes/uniforms from line?
        shaderStream.close();
    }else{
        std::cout << "Impossible to open "
                  << src_file_path
                  << std::endl;
        throw std::exception();
    }
}

void Shader::compile(const char *src_file_path, const std::string& shaderCode, GLint shaderType)
{
    std::cout << "Compiling shader : "
              << src_file_path
              << std::endl;
    char const * sourcePointer = shaderCode.c_str();
    mShaderId = glCreateShader(shaderType);
    glShaderSource(mShaderId, 1, &sourcePointer , NULL);
    glCompileShader(mShaderId);
}

void Shader::checkCompilation()
{
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> shaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(mShaderId, infoLogLength, NULL, &shaderErrorMessage[0]);
        std::cout << &shaderErrorMessage[0] << std::endl;
        if (result == GL_FALSE)
        {
            throw std::exception();
        }
    }
}

Program::Program()
    :mHasShaders(false), mIsLinked(false)
{
    mProgramId = glCreateProgram();
}

void Program::attachShader(const Shader& shader)
{
    glAttachShader(mProgramId, shader.getGlId());
    mHasShaders=true;
    mIsLinked=false;
}

void Program::attachShader(const char *src_file, GLint shaderType)
{
    Shader shader(src_file,shaderType);
    attachShader(shader);
}

void Program::link()
{
    std::cout << "Linking program" << std::endl;
    glLinkProgram(mProgramId);

    GLint result = GL_FALSE;
    int infoLogLength;

    glGetProgramiv(mProgramId, GL_LINK_STATUS, &result);
    glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> programErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(mProgramId, infoLogLength, NULL, &programErrorMessage[0]);
        std::cout << &programErrorMessage[0] << std::endl;
    }
    mIsLinked=(GL_TRUE==result);
}
