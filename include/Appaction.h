#pragma once
#ifndef __APPACTION_H__
#define __APPACTION_H__

#include "Object.h"
#include "World.h"
#include <functional>

using namespace std;

namespace engine
{
    using namespace core;
    enum runningState{
        stop, running 
    };

    class Appaction : public Object
    {
    public: 
    //function

        virtual const bool init(void);

        //运行程序
        void run(int argc, char ** argv);

        //获取程序路径
        static string appactionPath(void);

        //获取屏幕
        World & screen(void);

    protected:

        //程序开始
        virtual void start(void) = 0;

        //每一帧
        virtual void update(const int dt) = 0;

        virtual ~Appaction(void);
    private:

        static string _appactionPath;        
    };
}


#endif //__APPACTION_H__