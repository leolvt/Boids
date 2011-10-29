#
# Try to find GLFW library and include path.
# Once done this will define
#
# GLFW_FOUND - true/false
# GLFW_INCLUDE_DIR - Where GL/glfw.h is located
# GLFW_LIBRARIES - libs to link if you need GLFW
# 

FIND_PATH(GLFW_INCLUDE_DIR GL/glfw.h
    ${PROJECT_SOURCE_DIR}/include
    $ENV{GLFWDIR}/include 
    /usr/local/include 
    /usr/local/X11R6/include 
    /usr/X11R6/include 
    /usr/X11/include 
    /usr/include/X11 
    /usr/include 
    /opt/X11/include 
    /opt/include
    DOC "The directory where GL/glew.h resides"
    )
FIND_LIBRARY(GLFW_LIBRARIES NAMES glfw PATHS 
    ${PROJECT_SOURCE_DIR}/lib
    $ENV{GLFWDIR}/lib 
    /usr/local/lib
    /usr/local/X11R6/lib
    /usr/X11R6/lib 
    /usr/X11/lib
    /usr/lib/X11
    /usr/lib 
    /opt/X11/lib
    /opt/lib 
    DOC "The GLFW library"
    )

IF (GLFW_INCLUDE_PATH)
    IF (GLFW_LIBRARIES)
        SET( GLFW_FOUND 1 CACHE STRING "Set to 1 if GLFW is found, 0 otherwise")
    ELSE (GLFW_LIBRARIES)
        SET( GLFW_FOUND 0 CACHE STRING "Set to 1 if GLFW is found, 0 otherwise")
    ENDIF(GLFW_LIBRARIES)
ELSE (GLFW_INCLUDE_PATH)
    SET( GLFW_FOUND 0 CACHE STRING "Set to 1 if GLFW is found, 0 otherwise")
ENDIF (GLFW_INCLUDE_PATH)

MARK_AS_ADVANCED( GLFW_FOUND )

