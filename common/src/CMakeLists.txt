PROJECT(feather_common)

SET(feather_common_SRCS
    main.cpp
)

ADD_LIBRARY(feather_common SHARED ${feather_common_SRCS})

QT5_USE_MODULES(feather_common OpenGL)

SET_TARGET_PROPERTIES(feather_common
    PROPERTIES
    VERSION 0.01
    SOVERSION 1)

INSTALL(TARGETS feather_common
    LIBRARY DESTINATION /usr/local/feather/plugins)
