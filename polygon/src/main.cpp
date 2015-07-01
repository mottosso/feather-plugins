/***********************************************************************
 *
 * Filename: main.cpp
 *
 * Description: Plugin for creating and working with polygon objects.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include <feather/deps.hpp>
#include <feather/pluginmanager.hpp>
#include <feather/field.hpp>
#include <feather/node.hpp>
#include <feather/parameter.hpp>
#include <feather/command.hpp>
#include <feather/scenegraph.hpp>
#include <QColor>

#ifdef __cplusplus
extern "C" {
#endif
    C_PLUGIN_WRAPPER()
#ifdef __cplusplus
}
#endif

using namespace feather;

#define POLYGON_SHAPE 320
#define POLYGON_PLANE 321
#define POLYGON_CUBE 322


PLUGIN_INIT("Polygon","Polygon objects and tools","Richard Layman",POLYGON_SHAPE,POLYGON_CUBE)


/*
 ***************************************
 *            POLYGON SHAPE            *
 ***************************************
*/

// meshIn
ADD_FIELD_TO_NODE(POLYGON_SHAPE,FMesh,field::Mesh,field::connection::In,FMesh(),1)
// testIn
ADD_FIELD_TO_NODE(POLYGON_SHAPE,int,field::Int,field::connection::In,10,2)

namespace feather
{

    DO_IT(POLYGON_SHAPE)
    { 
        //PolygonShapeFields* shape = static_cast<PolygonShapeFields*>(fields);
        //typedef field::Field<int,field::connection::In>* fielddata;
        //fielddata f = static_cast<fielddata>(fields.at(0));

        //std::cout << "value from polycube field:" << f->value << std::endl;
        return status();
    };

    GL_INIT(POLYGON_SHAPE)
    {
        QOpenGLShader* frag = new QOpenGLShader(QOpenGLShader::Fragment);
        frag->compileSourceFile("shaders/frag/lambert.glsl");
        QOpenGLShader* vert = new QOpenGLShader(QOpenGLShader::Vertex);
        vert->compileSourceFile("shaders/vert/mesh.glsl");

        info.program->addShader(frag);
        info.program->addShader(vert);

        info.program->link();

        node.glVertex = info.program->attributeLocation("vertex");
        node.glColor = info.program->attributeLocation("color");
        node.glMatrix = info.program->uniformLocation("matrix");
        node.glNormal = info.program->attributeLocation("normal");
        node.glLightPosition = info.program->attributeLocation("lightposition");
        node.glShaderDiffuse = info.program->attributeLocation("shader_diffuse");
    }; 

    GL_DRAW(POLYGON_SHAPE)
    {
        typedef field::Field<FMesh,field::connection::In>* fielddata;
        fielddata f = static_cast<fielddata>(node.fields.at(1));

        typedef field::Field<FMesh,field::connection::Out>* targetdata;
        targetdata tf;

        if(f->connected) {
            tf = static_cast<targetdata>(scenegraph::get_fieldBase(f->puid,f->pn,f->pf));
        }

        
        if(tf!=NULL)
        { 
            if(tf->value.v.size() >= 4)
            {
                QVector3D lpos;
                lpos.setX(4);
                lpos.setY(20);
                lpos.setZ(10);
 
                info.program->bind();
                info.program->setAttributeValue(node.glLightPosition, lpos);

                info.program->setUniformValue(node.glMatrix, *info.view);
                info.program->enableAttributeArray(node.glVertex);
                info.program->enableAttributeArray(node.glColor);
                info.program->enableAttributeArray(node.glNormal);
                info.program->setAttributeArray(node.glVertex, GL_FLOAT, &tf->value.v[0], 3);
                info.program->setAttributeArray(node.glColor, GL_UNSIGNED_INT, &tf->value.c[0], 3);
                info.program->setAttributeArray(node.glNormal, GL_FLOAT, &tf->value.vn[0],3);

                /*
                QColor color;

                color.setRgb(0,0,100);
                info.program->setAttributeValue(node.glShaderDiffuse, color);
                */
                //info.program->setAttributeValue(node.glColor, color);
 
                glPolygonMode(GL_FRONT, GL_FILL);
                glPolygonMode(GL_BACK, GL_LINE);
                glDrawElements(GL_TRIANGLES, tf->value.i.size(), GL_UNSIGNED_INT, &tf->value.i[0]);

                //color.setRgb(0,0,0);
                //info.program->setAttributeValue(node.glShaderDiffuse, color);
                glLineWidth(4.5);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDrawElements(GL_TRIANGLES, tf->value.i.size(), GL_UNSIGNED_INT, &tf->value.i[0]);

                info.program->disableAttributeArray(node.glVertex);
                info.program->disableAttributeArray(node.glColor);
                info.program->disableAttributeArray(node.glNormal);
                info.program->release();
            }
        }

    }; 

} // namespace feather

NODE_INIT(POLYGON_SHAPE,node::Polygon,"polyshape.svg")


/*
 ***************************************
 *            POLYGON PLANE            *
 ***************************************
*/

ADD_FIELD_TO_NODE(POLYGON_PLANE,int,field::Int,field::connection::In,2,1)
ADD_FIELD_TO_NODE(POLYGON_PLANE,int,field::Int,field::connection::In,2,2)



namespace feather
{

    DO_IT(POLYGON_PLANE)
    { 
        return status();
    };

} // namespace feather

NODE_INIT(POLYGON_PLANE,node::Polygon,"")


/*
 ***************************************
 *            POLYGON CUBE             *
 ***************************************
*/

ADD_FIELD_TO_NODE(POLYGON_CUBE,int,field::Int,field::connection::In,0,1)
ADD_FIELD_TO_NODE(POLYGON_CUBE,int,field::Int,field::connection::In,0,2)
ADD_FIELD_TO_NODE(POLYGON_CUBE,int,field::Int,field::connection::In,0,3)
ADD_FIELD_TO_NODE(POLYGON_CUBE,FMesh,field::Mesh,field::connection::Out,FMesh(),4)

namespace feather
{

    DO_IT(POLYGON_CUBE) 
    {
        typedef field::Field<FMesh,field::connection::Out>* MeshOut;
        typedef field::Field<int,field::connection::In>* SubIn;

        MeshOut meshOut = static_cast<MeshOut>(fields.at(0));
        SubIn subX = static_cast<SubIn>(fields.at(1));
        SubIn subY = static_cast<SubIn>(fields.at(2));
        SubIn subZ = static_cast<SubIn>(fields.at(3));

        int fcount = (subX->value+1)*2 + (subY->value+1)*2 + (subZ->value+1)*2;
        int vcount = fcount + 2;

        if(subX->update || subY->update || subZ->update)
        {
            // clear the mesh
            meshOut->value.v.clear();
            meshOut->value.vn.clear();
            meshOut->value.i.clear();


            // there are 3 side, each will be subdiv by it's sub axis value
            // the front and back of each axis are created
            int side = 0;
            float maxV = 1.0;
            float minV = -1.0;
            float spanX = 2.0;
            float spanY = 2.0;

            /* 
            while(side < 3) {
                // front
                // create each face
                meshOut->value.v.push_back(FVertex3D(1.0,1.0,1.0);    
            }
            */

            if(!meshOut->value.v.size())
            {
                // VERTICS
                // Front 
                meshOut->value.v.push_back(FVertex3D(1.0,1.0,1.0));
                meshOut->value.v.push_back(FVertex3D(1.0,-1.0,1.0));
                meshOut->value.v.push_back(FVertex3D(-1.0,-1.0,1.0));
                meshOut->value.v.push_back(FVertex3D(-1.0,1.0,1.0));
                // Back 
                meshOut->value.v.push_back(FVertex3D(1.0,1.0,-1.0));
                meshOut->value.v.push_back(FVertex3D(-1.0,1.0,-1.0));
                meshOut->value.v.push_back(FVertex3D(-1.0,-1.0,-1.0));
                meshOut->value.v.push_back(FVertex3D(1.0,-1.0,-1.0));

                // NORMALS
                // Front
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,1.0));
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,1.0));
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,1.0));
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,1.0));
                // Back 
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,-1.0));
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,-1.0));
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,-1.0));
                meshOut->value.vn.push_back(FVertex3D(0.0,0.0,-1.0));

                FFace f;           
                // front face 
                f.push_back(FFacePoint(0,0,0));
                f.push_back(FFacePoint(1,0,1));
                f.push_back(FFacePoint(2,0,2));
                f.push_back(FFacePoint(3,0,3));
                meshOut->value.add_face(f);
                f.clear();
                // back face 
                f.push_back(FFacePoint(4,0,4));
                f.push_back(FFacePoint(5,0,5));
                f.push_back(FFacePoint(6,0,6));
                f.push_back(FFacePoint(7,0,7));
                meshOut->value.add_face(f);
                f.clear();
                // top face 
                f.push_back(FFacePoint(4,0,4));
                f.push_back(FFacePoint(5,0,5));
                f.push_back(FFacePoint(6,0,6));
                f.push_back(FFacePoint(7,0,7));
                meshOut->value.add_face(f);
                f.clear();
                // bottom face 
                f.push_back(FFacePoint(1,0,1));
                f.push_back(FFacePoint(7,0,7));
                f.push_back(FFacePoint(6,0,6));
                f.push_back(FFacePoint(2,0,2));
                meshOut->value.add_face(f);
                f.clear();
                // left face 
                f.push_back(FFacePoint(6,0,6));
                f.push_back(FFacePoint(5,0,5));
                f.push_back(FFacePoint(3,0,3));
                f.push_back(FFacePoint(2,0,2));
                meshOut->value.add_face(f);
                f.clear();
                // right face 
                f.push_back(FFacePoint(1,0,1));
                f.push_back(FFacePoint(0,0,0));
                f.push_back(FFacePoint(4,0,4));
                f.push_back(FFacePoint(7,0,7));
                meshOut->value.add_face(f);
                f.clear();
            }

            subX->update = false;
            subY->update = false;
            subZ->update = false;
        }

        meshOut->value.build_gl();
        // testing split
        meshOut->value.split_face(2,5,7);

        return status();
    };

} // namespace feather

NODE_INIT(POLYGON_CUBE,node::Polygon,"polycube.svg")


/*
 ***************************************
 *              COMMANDS               *
 ***************************************
*/

namespace feather
{
    namespace command
    {
        enum Command { N=0 };
    } // namespace command

} // namespace feather

INIT_COMMAND_CALLS(N)
