#pragma once 
#ifndef __VEC_H__
#define __VEC_H__

#include "Struct.h"


namespace engine
{
    namespace core
    {
        class Vec : public Struct
        {
        public: 
            float & operator[](const unsigned int index) const;
        protected:
            Vec(void){}
            ~Vec(void){}
        };
    }
}

#endif //__VEC_H__