#####################################################
# FILE NAME CMakeLists.txt                          #
#                                                   #
# WRITTEN BY Michael Brockus.                       #
#                                                   #
# PURPOSE contains CMake statements.                #
#                                                   #
#####################################################
cmake_minimum_required(VERSION 3.13.2.0 FATAL_ERROR)



#
#  CMake: Declare project
#
project(cexception LANGUAGES C DESCRIPTION "C Exception library.")



#
#  CMake: Creation of library   
#
add_library("cexception" STATIC)



#
#  CMake: Adding source to target
#
target_sources("cexception" PRIVATE "lib/CException.c")



#
#  CMake: Including directories to target
#
target_include_directories(cexception 
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>"
        "$<INSTALL_INTERFACE:lib>"

    PRIVATE "lib"
)



#
#  CMake: Give target an alias 
#
add_library("cexception::framework" ALIAS "cexception")



#
#  CMake: export project
#
install(TARGETS "cexception" EXPORT "cexceptionConfig"
    ARCHIVE DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_BINDIR}"

    INCLUDES DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(DIRECTORY lib/ DESTINATION lib)

install(EXPORT cexceptionConfig DESTINATION share/cexceptionConfig/cmake)

# This makes the project importable from the build directory
export(TARGETS cexception FILE cexceptionConfig.cmake)