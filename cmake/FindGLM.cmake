#
# Try to find GLM library and include path.
# Once done this will define
#
# GLM_FOUND - true/false
# GLM_INCLUDE_DIR - Where GLM.h is located
# GLM_LIBRARIES - libs to link if you need GLM
# 

FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp
    ${PROJECT_SOURCE_DIR}/include
    $ENV{GLM_PACKAGE_DIR}/include 
    /usr/local/include
    /usr/local/X11R6/include 
    /usr/X11R6/include 
    /usr/X11/include 
    /usr/include/X11 
    /usr/include 
    /opt/X11/include 
    /opt/include
    DOC "The directory where glm/glm.hpp resides"
    )

set(GLM_LIBRARIES)

IF (GLM_INCLUDE_PATH)
    IF (GLM_LIBRARIES)
        SET( GLM_FOUND 1 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
    ELSE (GLM_LIBRARIES)
        SET( GLM_FOUND 0 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
    ENDIF(GLM_LIBRARIES)
ELSE (GLM_INCLUDE_PATH)
    SET( GLM_FOUND 0 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
ENDIF (GLM_INCLUDE_PATH)

MARK_AS_ADVANCED( GLM_FOUND )

