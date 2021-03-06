#include "Triangle.h"
#include "LogManager.h"
#include "Materia.h"

namespace engine
{

    using namespace tools;
    Triangle & Triangle::create(const Vec3 & vertex_1, const Vec3 & vertex_2, const Vec3 & vertex_3)
    {
        Triangle & result = create();
        
        bool triangleInit = result.init(vertex_1, vertex_2, vertex_3);

        assert(triangleInit);

        if(!triangleInit){ result.initializeError(1); }
        return result;
    }

    const bool Triangle::init(void)
    {
        if(!Geometry::init()){ return false; }

        vertexsCount(3);
        return true;
    }

    const bool Triangle::init(const Vec3 & vertex_1, const Vec3 & vertex_2, const Vec3 & vertex_3)
    {

        Materia & materia = Materia::create();

        // ColorRGBA tempColors[3] = {ColorRGBA(.2f, 1.0f, .4f, 1.0f), ColorRGBA(.2f, 1.0f, .4f, 1.0f), ColorRGBA(.2f, 1.0f, .4f, 1.0f)};
        // materia.colors(tempColors, sizeof(tempColors) / sizeof(ColorRGBA));
        materia.color(ColorRGBA(1.0f, 0.0f, 0.0f, 1.0f));

        bindMateria(materia);
        Vec3 tempVertexs[3] = {vertex_1, vertex_2, vertex_3};

        vertexs(tempVertexs);

        return true;
    }

     const bool Triangle::render(const int dp)
     {
         if(!Geometry::render(dp)){ return false; }

         return true;
     }

     const bool Triangle::draw(const Matrix4 & projection)
     {
         if(!Geometry::draw(projection)){ return false; }

        glBindVertexArray(vertexArrayObject());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        return true;
     }
}