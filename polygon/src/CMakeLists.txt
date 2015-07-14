PROJECT(feather_polygon)

FIND_PACKAGE(OpenGL REQUIRED)

SET(feather_polygon_SRCS
    main.cpp
)

ADD_DEFINITIONS("-D GL_GLEXT_PROTOTYPES")

ADD_LIBRARY(feather_polygon SHARED ${feather_polygon_SRCS})

QT5_USE_MODULES(feather_polygon OpenGL)

SET_TARGET_PROPERTIES(feather_polygon
    PROPERTIES
    VERSION 0.01
    SOVERSION 1)

INSTALL(TARGETS feather_polygon
    LIBRARY DESTINATION /usr/local/feather/plugins)
