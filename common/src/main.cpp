/***********************************************************************
 *
 * Filename: main.cpp
 *
 * Description: Holds common nodes and commands. 
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


/*
 ***************************************
 *           C PLUGIN WRAPPER          *
 ***************************************
 *
 * C_PLUGIN_WRAPPER()
 * 
 * This macro is needed to make it callable
 * by the core.
 * Every plugin needs it and it's always
 * the same so you should never have to
 * edit the below code.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif
    C_PLUGIN_WRAPPER()
#ifdef __cplusplus
}
#endif

using namespace feather;

// Define all of our nodes
// with id's here.
#define EMPTY 1
#define SHAPE 2
#define TRANSFORM 3

PLUGIN_INIT("Common","Commonly used nodes and commands","Richard Layman",EMPTY,TRANSFORM)

/*
 ***************************************
 *               NONE                  *
 ***************************************
*/

// parent
ADD_FIELD_TO_NODE(EMPTY,FNode,field::Node,field::connection::In,FNode(),1)
// child
ADD_FIELD_TO_NODE(EMPTY,FNode,field::Node,field::connection::Out,FNode(),2)


namespace feather
{
    DO_IT(EMPTY)
    { 
        return status();
    };
} // namespace feather

NODE_INIT(EMPTY,node::Empty,"node_empty.svg")


/*
 ***************************************
 *               SHAPE                 *
 ***************************************
*/

ADD_FIELD_TO_NODE(SHAPE,FMesh,field::Mesh,field::connection::In,FMesh(),200)
ADD_FIELD_TO_NODE(SHAPE,FMesh,field::Mesh,field::connection::Out,FMesh(),201)


namespace feather
{
    DO_IT(SHAPE)
    { 
        //ShapeFields* shape = static_cast<ShapeFields*>(fields);
        //std::cout << "shape: meshIn:" << shape->meshIn << std::endl;

        return status();
    };
} // namespace feather

NODE_INIT(SHAPE,node::Object,"")


/*
 ***************************************
 *              TRANSFORM              *
 ***************************************
*/

ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,1)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,2)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,3)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,4)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,5)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,6)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,7)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,8)
ADD_FIELD_TO_NODE(TRANSFORM,FDouble,field::Double,field::connection::Out,0.0,9)

namespace feather
{
    DO_IT(TRANSFORM) 
    {
        //TransformFields* xform = static_cast<TransformFields*>(fields);
        //std::cout << "xform: tx:" << xform->tx << std::endl;
        return status();
    };
} // namespace feather

NODE_INIT(TRANSFORM,node::Manipulator,"")


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
