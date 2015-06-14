PROJECT(feather_obj)

SET(feather_obj_SRCS
    obj.cpp
    main.cpp
)

ADD_LIBRARY(feather_obj SHARED ${feather_obj_SRCS})

QT5_USE_MODULES(feather_obj OpenGL)

SET_TARGET_PROPERTIES(feather_obj
    PROPERTIES
    VERSION 0.01
    SOVERSION 1)

INSTALL(TARGETS feather_obj
    LIBRARY DESTINATION /usr/local/feather/plugins)
