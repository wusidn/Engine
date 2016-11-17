#pragma once
#ifndef __SCREEN_WORLD_H__
#define __SCREEN_WORLD_H__
#include "World.h"
#include "Camera.h"

namespace engine
{
    using namespace core;
    class ScreenWorld : public World
    {
        friend class Appaction;
    public:
        static ScreenWorld & instance(void);

        const Camera & screenCamera(void) const;
    protected:
        ScreenWorld();
        virtual ~ScreenWorld();
        virtual const bool init(void);

        virtual const bool render(const int dt);
        virtual const bool draw(void);
        
    private:
        CREATEFUNC(ScreenWorld);
        static ScreenWorld * _instance;

        Camera * _screenCamera;
    };
}

#endif //__SCREEN_WORLD_H__