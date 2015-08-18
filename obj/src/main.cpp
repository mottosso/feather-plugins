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
            std::cout << "import obj\n";
            std::string filename;
            bool selection=false;
            bool p=false;
            p = params.getParameterValue<std::string>("filename",filename);
            if(!p)
                return status(FAILED,"filename parameter failed");
            p = params.getParameterValue<bool>("selection",selection);
            if(!p)
                return status(FAILED,"selection parameter failed");
 
            return status();
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
