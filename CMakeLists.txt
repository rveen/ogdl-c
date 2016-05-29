cmake_minimum_required (VERSION 3.0)

project (ogdl-c VERSION 1.0.0 LANGUAGES C)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools")

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen.conf"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Building documentation..."
   )
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/api/html DESTINATION share/doc/ogdl)
endif()
