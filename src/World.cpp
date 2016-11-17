#include "World.h"
#include <algorithm>
#include "LogManager.h"

namespace engine
{
    using namespace tools;
    vector<World *> World::_worldPool;

    const vector<World *> & World::worldPool()
    {
        return _worldPool;
    }

    World::World(void)
    {
        _root = nullptr;
    }

    World::~World(void)
    {
        auto index = std::find(_worldPool.begin(), _worldPool.end(), this);
        if(index != _worldPool.end())
        {
            _worldPool.erase(index);
        }
        if(_root)
        {
            _root->release();
            _root = nullptr;
        }
    }

    const bool World::init()
    {
        if(!Object::init()){ return false; }
        
        _worldPool.push_back(this);
        retain();

        _root = &Node::create();
        _root->retain();
        _root->position(Vec3(.0f));

        return true;
    }
    
    Node & World::root(void) const
    {
        return *_root;
    }

    void World::append(Node & child)
    {
        _root->append(child);
    }

    void World::remove(Node & child)
    {
        _root->remove(child);
    }
    
}