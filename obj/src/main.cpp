/***********************************************************************
 *
 * Filename: main.cpp
 *
 * Description: Plugin for importing and exporting obj files.
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

#include "obj.hpp"

#ifdef __cplusplus
extern "C" {
#endif
    C_PLUGIN_WRAPPER()
#ifdef __cplusplus
}
#endif

using namespace feather;

PLUGIN_INIT("Obj","Import and export obj format file data.","Richard Layman",0,0)


/*
 ***************************************
 *              COMMANDS               *
 ***************************************
 */

namespace feather
{
    namespace command
    {
        enum Command { N=0, IMPORT_OBJ, EXPORT_OBJ };

        status import_obj(parameter::ParameterList params) {
            std::string filename;
            bool selection=false;
            bool p=false;
            p = params.getParameterValue<std::string>("filename",filename);
            if(!p)
                return status(FAILED,"filename parameter failed");
            p = params.getParameterValue<bool>("selection",selection);
            if(!p)
                return status(FAILED,"selection parameter failed");

            // load the mesh
            //mesh_t mesh;
            //p = obj::io::load_mesh(mesh,filename);
            //if(!p)
            //    return status(FAILED,"obj file failed to parse");

            //return status();
            obj_data_t data;
            feather::status s = obj::io::file<obj::io::IMPORT,obj::io::OBJ>(data,filename);

            // add the nodes to the scenegraph
            int uid = feather::scenegraph::add_node(feather::node::Polygon,320,data.object.at(0).o);            

            // for now I'm just going to connect the node to the root
            feather::scenegraph::connect(0,4,uid,1);
            //feather::scenegraph::update();

            // get the mesh from the node and fill in it's values
            typedef field::Field<feather::FMesh,field::connection::In>* sourcefield;
            sourcefield sf=NULL;
           
            // NOTE: you can't call feather::sg[uid] from here - you will get a seg fault 
            sf = static_cast<sourcefield>(feather::scenegraph::get_fieldBase(uid,320,1));
            if(sf){
                // only going to do the first object as a test
                // fill in the mesh
                sf->value.assign_v(data.object.at(0).mesh.v);
                sf->value.assign_st(data.object.at(0).mesh.st);
                sf->value.assign_vn(data.object.at(0).mesh.vn);
                // obj v indexes are 1 based and need to be converted to 0 based
                for_each(data.object.at(0).grp.at(0).sg.at(0).f.begin(), data.object.at(0).grp.at(0).sg.at(0).f.end(), [](feather::FFace& f){
                    for_each(f.begin(), f.end(),[](feather::FFacePoint& fp){ fp.v=fp.v-1; fp.vn=fp.vn-1; });
                });
                sf->value.assign_f(data.object.at(0).grp.at(0).sg.at(0).f);
            }
            else
                std::cout << "NULL SOURCE FIELD\n";
    
            // build the gl mesh for the viewport
            sf->value.build_gl();
                 
            feather::scenegraph::update();

            return s;
        };

        status export_obj(parameter::ParameterList params) {
            std::cout << "running export_obj command" << std::endl;
            return status();
        };

    } // namespace command

} // namespace feather

// Import Obj Command
ADD_COMMAND("import_obj",IMPORT_OBJ,import_obj)

ADD_PARAMETER(command::IMPORT_OBJ,1,parameter::String,"filename")

ADD_PARAMETER(command::IMPORT_OBJ,2,parameter::Bool,"selection")

// Export Obj Command
ADD_COMMAND("export_obj",EXPORT_OBJ,export_obj)

INIT_COMMAND_CALLS(EXPORT_OBJ)
