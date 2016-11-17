#include "CameraOutput.h"
#include "LogManager.h"

namespace engine
{
    using namespace tools;
    const bool CameraOutput::init(void)
    {
        if(!Node::init()){ return false; }
        return true;
    }

    Camera & CameraOutput::camera(void) const
    {
        return *_camera;
    }
    
    void CameraOutput::camera(Camera & linkCamera)
    {
        if(_camera)
        {
            _camera->release();
            _camera = nullptr;
        }
        _camera = &linkCamera;
        _camera->retain();
    }

    const bool CameraOutput::render(const int dt)
    {
        if(!Node::render(dt)){ return false; }

        if(!_camera){ return false; }

        return camera().preparatory(dt);
    }

    const bool CameraOutput::draw(const Matrix4 & projection)
    {
        if(!Node::draw(projection)){ return false; }

        if(!_camera){ return false; }

        //设置摄像机投影到gl空间的位置

        //拍照
        return camera().photograph();
    }

    CameraOutput::CameraOutput(void)
    {
        _camera = nullptr;
    }

    CameraOutput::~CameraOutput(void)
    {
        if(_camera)
        {
            _camera->release();
            _camera = nullptr;
        }
    }
}