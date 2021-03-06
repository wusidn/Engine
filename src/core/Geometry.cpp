#include "Geometry.h"
#include <iostream>
#include <cstring>
#include <vector>

#include "LogManager.h"
#include "Matrix4.h"
#include "ShaderProgram.h"


using namespace std;

namespace engine
{
    namespace core
    {
        using namespace tools;

        const bool Geometry::bindMateria(Materia & m)
        {
            _materia = &m;
            m.retain();

            //更新着色器
            if(!updateShaderProgram()){ return false; }

            return true;
        }

        Geometry::Geometry(void)
        {
            _vertexs = nullptr;
            _indies = nullptr;
            _materia = nullptr;
            _shaderProgram = nullptr;

            _vertexArrayObject = _vertexBufferObject = _indiesBufferObject = 0;
            _vertexsCount = _indiesCount = 0;
        }

        const bool Geometry::init(void)
        {
            if(!Node::init()){ return false; }

            _materia = (engine::Materia*)&(Materia::defaultMateria());
            updateShaderProgram();

            return true;
        }

        void Geometry::vertexsCount(const unsigned short count)
        {
            
            _vertexsCount = 0;
            if(_vertexs) { delete[] _vertexs; }

            if(!count)
            {
                _vertexs = nullptr;
                return;
            }

            _vertexs = new Vec3[count];
            if(!_vertexs){ return; }


            //申请顶点数组对象
            while(!_vertexArrayObject)
            {
                glGenVertexArrays(1, &_vertexArrayObject);
            }

            //申请缓存对象
            while(!_vertexBufferObject)
            {
                glGenBuffers(1, &_vertexBufferObject);
            }

            _vertexsCount = count;
        }

        void Geometry::indiesCount(const unsigned short count)
        {
            _indiesCount = 0;

            if(_indies){ delete[] _indies; }

            if(!count){ _indies = nullptr; }

            _indies = new unsigned short[count];
            if(!_indies) return;

            //申请缓存对象
            while(!_indiesBufferObject)
            {
                glGenBuffers(1, &_indiesBufferObject);
            }

            _indiesCount = count;
            memset(_indies, 0, sizeof(unsigned short) * _indiesCount);
        }

        const unsigned short Geometry::vertexsCount(void) const
        {
            return _vertexsCount;
        }

        const unsigned short Geometry::indiesCount(void) const
        {
            return _indiesCount;
        }

        const GLuint Geometry::vertexArrayObject(void) const
        {
            return _vertexArrayObject;
        }
        const GLuint Geometry::vertexBufferObject(void) const
        {
            return _vertexBufferObject;
        }
        const GLuint Geometry::indiesBufferObject(void) const
        {
            return _indiesBufferObject;
        }




        void Geometry::vertex(const unsigned short index, const Vec3 & data)
        {
            if(index >= _vertexsCount)
            {
                //错误信息
                return;
            }
            *(_vertexs + index) = data;
        }

        void Geometry::indie(const unsigned short index, const unsigned short data)
        {
            if(index >= _indiesCount)
            {
                //错误信息
                return;
            }
            *(_indies + index) = data;
        }


        void Geometry::vertexs(const Vec3 * data)
        {
            vertexs(data, _vertexsCount, 0);
        }
        
        void Geometry::vertexs(const Vec3 * data, const unsigned short count, const unsigned short startIndex)
        {
            memcpy(_vertexs + startIndex, data, (_vertexsCount - startIndex) * sizeof(Vec3));
        }

        void Geometry::indies(const unsigned short * data, const unsigned short count, const unsigned short startIndex)
        {
            memcpy(_indies + startIndex, data, (_indiesCount - startIndex) * sizeof(unsigned short));
        }

        void Geometry::indies(const unsigned short * data)
        {
            indies(data, _indiesCount, 0);
        }

        const Vec3 * Geometry::vertexs(void) const 
        {
            return _vertexs;
        }

        const unsigned short * Geometry::indies(void) const 
        {
            return _indies;
        }

        const bool Geometry::render(const int dp)
        {
            if(!Node::render(dp)){ return false; }
            return true;
        }

        const bool Geometry::draw(const Matrix4 & projection)
        {

            //如果没有顶点数据直接返回
            if(!_vertexsCount){ return true; }

            //如果没有bind着色器程序
            if(!_shaderProgram)
            { 
                Log.error("not bind ShaderProgram!");
                return false; 
            }

            //材质
            if(!_materia) { return false; }

            //调用Node的draw
            if(!Node::draw(projection)){ return false; }

            if(_indiesCount)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiesBufferObject);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * _indiesCount, _indies, GL_STATIC_DRAW);
            }

            glBindVertexArray(_vertexArrayObject);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);

            MateriaType materiaType = _materia->materiaType();

            switch(materiaType)
            {
                case MateriaType::Purity:

                    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * _vertexsCount, _vertexs, GL_STATIC_DRAW);
                    _shaderProgram->uniformSet("fColor", _materia->color().rgba());
                break;

                case MateriaType::Multicolor:

                    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * _vertexsCount + sizeof(ColorRGBA) * _materia->colorsCount(), nullptr, GL_STATIC_DRAW);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * _vertexsCount, _vertexs);
                    glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vec3) * _vertexsCount, sizeof(ColorRGBA) * _materia->colorsCount(), _materia->colors());
                break;

                default:
                    Log.error("materiaType({0}) is ndefine!", materiaType);
                return false;
            }


            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void *)(sizeof(Vec3) * _vertexsCount));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

            //启用着色器程序
            _shaderProgram->use();
            

            //创建旋转  缩放  平移到世界坐标 矩阵
            //旋转
            Matrix4 rotationMatrix = Matrix4::createRotationMatrix(rotate() + accumulativeRotateOffset());
            // Log.info("rotationMatrix: {0}", rotationMatrix);
            //缩放
            Matrix4 scaleMatrix = Matrix4::createScaleMatrix(scale() * (Vec3(0) + accumulativeScaleOffset()));
            // Log.info("scaleMatrix: {0}", scaleMatrix);
            //平移
            Matrix4 translationMatrix = Matrix4::createTranslationMatrix(position().convertToSize3() + worldCoordinateOffset());
            // Log.info("translationMatrix: {0}", translationMatrix);

            //m
            Matrix4 modelMatrix(1.0f); // = rotationMatrix * scaleMatrix * translationMatrix;

            //v
            Matrix4 viewMatrix(1.0f);

            //p
            Matrix4 projectionMatrix(1.0f);
            // projectionMatrix[3][3] = 0.0f;
            // projectionMatrix[3][2] = 1.0f;

            // Log.info("modelMatrix: {0}; viewMatrix: {1}; projectionMatrix: {2}", modelMatrix, viewMatrix, projectionMatrix);


            _shaderProgram->uniformSet("modelMatrix", modelMatrix);
            _shaderProgram->uniformSet("viewMatrix", viewMatrix);
            _shaderProgram->uniformSet("projectionMatrix", projectionMatrix);

            return true;
        }

        const bool Geometry::updateShaderProgram(void)
        {
            vector<string> vShaderFiles, fShaderFiles;

            if(_materia)
            {
                switch(_materia->materiaType())
                {
                    case MateriaType::Purity:
                        vShaderFiles.push_back("MPurity.vert");
                        fShaderFiles.push_back("MPurity.frag");
                    break;
                    default:
                        return false;
                }
            }

            //创建新的着色器
            ShaderProgram & newShaderProgram = ShaderProgram::create(vShaderFiles, fShaderFiles);
            if(!newShaderProgram.ready()){ return false; }
            
            
            //解除链接
            if(_shaderProgram != nullptr)
            {
                _shaderProgram->release();
                _shaderProgram = nullptr;
            }

            //建立新的链接
            _shaderProgram = &newShaderProgram;
            newShaderProgram.autoRelease();
            newShaderProgram.retain();

            return true;
        }

        Geometry::~Geometry(void)
        {
            if(_vertexs){ delete[] _vertexs; }
            
            if(_indies){ delete[] _indies; }

            if(_vertexBufferObject){ glDeleteBuffers(1, &_vertexBufferObject); }

            if(_vertexBufferObject){ glDeleteBuffers(1, &_indiesBufferObject); }

            if(_vertexBufferObject){ glDeleteVertexArrays(1, &_vertexArrayObject); }
        }
    }
}