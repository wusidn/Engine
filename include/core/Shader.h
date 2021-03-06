#pragma once
#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Object.h"
#include "File.h"

using namespace std;


namespace engine
{
    namespace core
    {
        enum ShaderType{
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };

        class Shader : public Object
        {
        public:

            friend class ShaderProgram;
            
            static Shader & create(const enum ShaderType type);
            static Shader & create(const string & fileName, const enum ShaderType type);     
            static Shader & create(const vector<string> & shaderFiles, const enum ShaderType type);

            //编译shader
            const bool compile(void) const;
            
            //获取shaderId
            const unsigned int shaderId(void) const;
            
        protected:

            CREATEFUNC(Shader);

            Shader(void);
            virtual ~Shader(void);
            virtual const bool init(void);
            virtual const bool initWithType(const enum ShaderType type);
            // virtual const bool init(const string & fileName, const enum ShaderType type);
            virtual const bool initWithFiles(const vector<string> & shaderFiles, const enum ShaderType type);
            
        private:
            GLuint _shaderId;

            //ShaderTemplateCode
            static string vertexShaderCode(void);
            static string fragmentShaderCode(void);

            static string versionKey, globalCodeKey, mainCodeKey;
            
            //编译是否成功
            const bool compileIsSuccessful(void) const;
            
        };
    }
}

#endif //__SHADER_H__