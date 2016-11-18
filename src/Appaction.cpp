#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Appaction.h"
#include "Gc.h"
#include "File.h"
#include "Node.h"
#include "Vec4.h"
#include "LogManager.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Size2.h"
#include "World.h"
#include "ScreenWorld.h"
#include "CameraOutput.h"

using namespace std;


GLuint vertexArrayObject;
GLuint vertexBufferObject;
GLuint indiesBufferObject;

namespace engine
{
    using namespace tools;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        std::cout << key << std::endl;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    const bool Appaction::init(void)
    {
        retain();
        return true;
    }

    string Appaction::_appactionPath = "";

    void Appaction::run(int argc, char ** argv)
    {
        static bool running = false;
        if(running) return;
        running = true;

        
        static regex pathRegex("^([^/\\\\]*[/\\\\])+");
        smatch searchResult;
        string sourceAppactionPath = string(argv[0]);
        if(!regex_search(sourceAppactionPath, searchResult, pathRegex))
        {
            Log.info("Matching appactionPath Error");
        }

        _appactionPath = string(searchResult[0]);
        
        //初始化glfw
        glfwInit();

        //设置参数
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        //创建窗口
        GLFWwindow* window = glfwCreateWindow(800, 600, "engine", nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glfwSetKeyCallback(window, key_callback);

        

        
        glClearColor(0.0, 0.0, 0.0, 1.0);

        start();

        while(!glfwWindowShouldClose(window))
        {

            static int prevDisplayTime = 0;
            int currDisplayTime = glfwGetTime();

            glfwPollEvents();

            //动画系统、计时器执行

            //执行用户代码逻辑
            update(currDisplayTime - prevDisplayTime);

            //确定所有元素的位置后计算所有元素的位置
            ScreenWorld::instance().render(currDisplayTime - prevDisplayTime);

            //清空画布
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //以每个视口绘制
            ScreenWorld::instance().draw();

            prevDisplayTime = currDisplayTime;

            //垃圾回收
            Gc::getInstance().clean();

            //渲染
            glfwSwapBuffers(window);
        };
        glfwTerminate();
    }

    string Appaction::appactionPath(void)
    {
        return _appactionPath;
    }

    World & Appaction::screen(void)
    {
        return ScreenWorld::instance();
    }


    Appaction::~Appaction(void)
    {

    }

}
