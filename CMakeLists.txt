cmake_minimum_required (VERSION 2.8)
project (Projeto)
set(PROG progprinc)

set(SOURCES src/client/main.c src/client/myf.c src/client/callbacks.c)

add_executable(${PROG} ${SOURCES})

#Next line to force the compilation with C++ because of the openCV library which is in C++ after version 3
SET_SOURCE_FILES_PROPERTIES( ${SOURCES} PROPERTIES LANGUAGE CXX )

#-----------------------------------------------

#Cf. http://docs.opencv.org/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html
#Add an external library like OpenCV
#find_package( OpenCV 2.4.9.1 EXACT )  #for a very specifi version
find_package( OpenCV 3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#The recommended way to find packages automatically using PkgConfig
#See more on http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

target_link_libraries(${PROG} m ${GTK_LIBRARIES} ${OpenCV_LIBS} )


# add a target to generate API documentation with Doxygen
# The project version number. Used in documentation generation and similar
set(VERSION_MAJOR   1   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

#-- add a target to generate API documentation with Doxygensudo mysqladmin -p -u root version
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install doc (requires Doxygen)" ${DOXYGEN_FOUND})
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in" )
    set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(doxy_main_page "${CMAKE_CURRENT_SOURCE_DIR}/docs/mainpage.dox")
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

#-----------------------------------------------------
# --- add custom mtargets (and dependencies)
add_custom_target( cpfiles
                COMMAND cp "${PROJECT_SOURCE_DIR}/src/client/*.glade" "${PROJECT_BINARY_DIR}"
                COMMAND cp "${PROJECT_SOURCE_DIR}/src/client/*.jpg" "${PROJECT_BINARY_DIR}"
		COMMAND cp "${PROJECT_SOURCE_DIR}/src/client/*.png" "${PROJECT_BINARY_DIR}"                 )

#Add dependencies
add_dependencies(${PROG} cpfiles)

#-------------------------------------

#Next lines generate the prototypes but it is not automatic.
#You must force a make prototypes.h to generate them
#it is better this way to avoid unnecessary recompilations.
#If you got an error during compilation and if it is likely due to a
#bad or missing prototype, just redo a make prototypes.h
#This follows the commom use of defining the prototypes manually.

add_custom_target( prototypes.h
        DEPENDS ${SOURCES}
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                COMMAND echo "Rebuild prototypes.h"
                COMMAND echo '/*File generated automatically. Do not edit*/'        > "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo '/*Generated on' `hostname` ',' `date` '*/'            >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo '\#ifdef __cplusplus'                                  >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo 'extern \"C\" {'                                       >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo '\#endif'                                              >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND cproto `pkg-config --cflags gtk+-3.0|awk '{$$1="" \; print $$0}'` `pkg-config --cflags opencv` -q -O /dev/null ${SOURCES}  >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo '\#ifdef __cplusplus'                                  >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo '}'                                                    >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                COMMAND echo '\#endif'                                              >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
                 )


install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/client/" DESTINATION bin FILES_MATCHING PATTERN "*.glade")

