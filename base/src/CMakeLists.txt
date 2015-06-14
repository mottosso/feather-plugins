PROJECT(feather_plugin)

SET(feather_plugin_SRCS
    main.cpp
)

ADD_LIBRARY(feather_plugin SHARED ${feather_plugin_SRCS})

QT5_USE_MODULES(feather_plugin OpenGL)

SET_TARGET_PROPERTIES(feather_plugin
    PROPERTIES
    VERSION 0.01
    SOVERSION 1)

INSTALL(TARGETS feather_plugin
    LIBRARY DESTINATION /usr/local/feather/plugins)
