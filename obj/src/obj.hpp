/***********************************************************************
 *
 * Filename: obj.hpp
 *
 * Description: Code to import and export obj files.
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

#ifndef OBJ_HPP
#define OBJ_HPP

#include <feather/types.hpp>
#include <feather/deps.hpp>
#include <feather/status.hpp>

// Mesh Components

typedef struct {
    int s;
    std::vector<feather::FFace> f;
} smoothing_group_t;

typedef struct {
    std::string usemtl;
    std::vector<smoothing_group_t> sg; 
} group_t;

typedef struct{
    feather::FVertex3DArray v;
    feather::FTextureCoordArray st;
    feather::FNormal3DArray vn;
} mesh_t;

typedef struct {
    std::string o;
    mesh_t mesh;
    std::string g;
    std::vector<group_t> grp;
} object_t;

typedef struct {
    std::vector<std::string> mtllib;
    std::vector<object_t> object;
} obj_data_t;


// OBJ DATA

// vertex point
BOOST_FUSION_ADAPT_STRUCT(
        feather::FVertex3D,
        (double, x)
        (double, y)
        (double, z)
        )

// texture point
BOOST_FUSION_ADAPT_STRUCT(
        feather::FTextureCoord,
        (double, s)
        (double, t)
        )

// normal point
BOOST_FUSION_ADAPT_STRUCT(
        feather::FNormal3D,
        (double, x)
        (double, y)
        (double, z)
        )

// face point
BOOST_FUSION_ADAPT_STRUCT(
        feather::FFacePoint,
        (feather::FUInt, v)
        (feather::FUInt, vt)
        (feather::FUInt, vn)
        )

// smoothing group
BOOST_FUSION_ADAPT_STRUCT(
        smoothing_group_t,
        (int, s)
        (std::vector<feather::FFace>, f)
        )

// group
BOOST_FUSION_ADAPT_STRUCT(
        group_t,
        (std::string, usemtl)
        (std::vector<smoothing_group_t>, sg)
        )

// mesh
BOOST_FUSION_ADAPT_STRUCT(
        mesh_t,
        (feather::FVertex3DArray, v)
        (feather::FTextureCoordArray, st)
        (feather::FNormal3DArray, vn)
        )

// object
BOOST_FUSION_ADAPT_STRUCT(
        object_t,
        (std::string, o)
        (mesh_t, mesh)
        (std::string, g)
        (std::vector<group_t>, grp)
        )

// data
BOOST_FUSION_ADAPT_STRUCT(
        obj_data_t,
        (std::vector<std::string>, mtllib)
        (std::vector<object_t>, object)
        )


namespace obj 
{

    namespace io
    {
        enum FileType { Mesh, Shader, Group, Texture, Light, Camera, Global };
        enum Action { IMPORT, EXPORT };
        enum Format { OBJ };


        namespace parsing 
        {

            namespace fusion = boost::fusion;
            namespace phoenix = boost::phoenix;
            namespace qi = boost::spirit::qi;
            namespace ascii = boost::spirit::ascii;
            namespace lambda = boost::lambda;

            template <typename InputIterator>
                struct skipper_grammar : qi::grammar<InputIterator>
            {
                typedef skipper_grammar<InputIterator> type;
                skipper_grammar()
                    : skipper_grammar::base_type( skip_on ),
                    comment_char( '#' )
                {
                    using ascii::char_;
                    using ascii::space;
                    using ascii::no_case;
                    using qi::lit;
                    using qi::eol;
                    using phoenix::ref;
                    using namespace qi::labels;

                    comment =
                        //char_( ref( comment_char ) )
                        char_ ( '#' )
                        >> *( char_ - eol ) >> eol
                        ;

                    comment_charset =
                        char_( "!\"#$%&'()*,:;<>?@\\^`{|}~" )
                        ;

                    skip_on =
                        space  // tab/space/cr/lf
                        | comment
                        ;
                }

                char comment_char;
                qi::rule<InputIterator> comment;
                qi::rule<InputIterator, char()> comment_charset;
                qi::rule<InputIterator> comment_directive;
                qi::rule<InputIterator> skip_on;
            };


            template<typename Iterator, typename Skipper>
                struct obj_parser : qi::grammar<Iterator, obj_data_t(), Skipper>
            {
                obj_parser() : obj_parser::base_type(data ,"data")
                {
                    using qi::lit;
                    using qi::lexeme;
                    using qi::on_error;
                    using qi::fail;
                    using qi::int_;
                    using qi::uint_;
                    using qi::double_;
                    //using qi::eol;
                    using ascii::char_;
                    using ascii::string;
                    using namespace qi::labels;
                    using phoenix::construct;
                    using phoenix::val;
                    using phoenix::if_;
                    using phoenix::arg_names::arg1;
                    
                    v %= 'v' >> double_ >> double_ >> double_ ;
                    vt %= lit("vt") >> double_ >> double_ ;
                    vn %= lit("vn") >> double_ >> double_ >> double_ ;
                    facepoint %= uint_ >> '/' >> -uint_ >> '/' >> uint_;
                    o %= 'o' >> lexeme[+(char_ - qi::eol)] ;
                    mtllib %= lit("mtllib") >> lexeme[+(char_ - qi::eol)] ;
                    usemtl %= lit("usemtl") >> lexeme[+(char_ - qi::eol)] ;
                    s %= 's' >> int_ ;
                    f %= 'f' >> *facepoint ;
                    sgrp %= s >> *f ;
                    grp %= usemtl >> *sgrp ;
                    g %= 'g' >> lexeme[+(char_ - qi::eol)];
                    // notice the '>' instead of '>>', this is for enables error backtracing
                    mesh %= *v >> -(*vt) >> *vn;
                    object %= o >> mesh >> g >> *grp;
                    data %= -(*mtllib) >> *object;

                    // names for rule errors
                    v.name("v");
                    vt.name("vt");
                    vn.name("vn");
                    facepoint.name("fp");
                    f.name("f");
                    g.name("g");
                    s.name("s");
                    o.name("o");
                    usemtl.name("usemtl");
                    mtllib.name("mtllib");
                    sgrp.name("smoothing_group");
                    grp.name("group");
                    mesh.name("mesh");
                    object.name("object");
                    data.name("data");

                    // error handling
                    on_error<fail>
                        (
                         data,
                         std::cout
                         << val("Error! Expecting ")
                         << boost::spirit::_4   // what failed
                         << val(" here: \"")
                         << construct<std::string>(boost::spirit::_3, boost::spirit::_2) // interators to error pos, end
                         << val("\"")
                         << std::endl
                        );
                }

                qi::rule<Iterator, obj_data_t(), Skipper> data;
                qi::rule<Iterator, std::string(), Skipper> o;
                qi::rule<Iterator, feather::FVertex3D(), Skipper> v;
                qi::rule<Iterator, feather::FTextureCoord(), Skipper> vt;
                qi::rule<Iterator, feather::FNormal3D(), Skipper> vn;
                qi::rule<Iterator, feather::FFacePoint(), Skipper> facepoint;
                qi::rule<Iterator, std::string(), Skipper> g;
                qi::rule<Iterator, std::string(), Skipper> usemtl;
                qi::rule<Iterator, std::string(), Skipper> mtllib;
                qi::rule<Iterator, int(), Skipper> s;
                qi::rule<Iterator, feather::FFace(), Skipper> f;
                qi::rule<Iterator, smoothing_group_t(), Skipper> sgrp;
                qi::rule<Iterator, group_t(), Skipper> grp;
                qi::rule<Iterator, mesh_t(), Skipper> mesh;
                qi::rule<Iterator, object_t(), Skipper> object;
            }; 

        }

        bool load_mesh(mesh_t& mesh, std::string path);
        bool write_mesh(obj_data_t& data);
        bool write_obj(std::string filename, obj_data_t& data);


        template <int Action, int Format>
        feather::status file(obj_data_t& data, std::string filename="") { return feather::status(feather::FAILED,"unknown action or format"); };

        // specialization
        template <> feather::status file<IMPORT,OBJ>(obj_data_t& data, std::string filename);

    } // namespace io

} // namespace obj 


#endif
