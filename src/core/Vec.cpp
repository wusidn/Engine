#include "Vec.h"

namespace engine
{
    namespace core
    {
        float & Vec::operator[](const unsigned int index) const
        {
            return *((float *)this + index);
        }
    }
}