set(LIBRARY_NAME ogdl)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -c -Wmissing-prototypes -Wstrict-prototypes")

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/graph.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ogdlbin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ogdllog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ogdlparser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/path.c
)

set(INCLUDE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ogdl.h
)

add_library(
    ${LIBRARY_NAME}
    STATIC
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

option(SWIG_PYTHON "ON to generate python code via swig" OFF)


if(SWIG_PYTHON)
    #SWIG
    #load the cmake SWIG package
    find_package(SWIG)
    if(SWIG_FOUND)
	    include(${SWIG_USE_FILE})

	    #load the package that SWIG uses to generate Python
	    find_package(PythonLibs)

	    #point to python headers and ogdl header
	    include_directories(${PYTHON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

	    #tell SWIG to create a new module, called mongopyAPI, 
	    #in Python and point to the SWIG interface file (the .i file)
	    swig_add_module(ogdlpy python ${CMAKE_CURRENT_SOURCE_DIR}/ogdl.i)

	    #link the above module to the API (the shared object) we just created and to Python
	    swig_link_libraries(ogdlpy ${LIBRARY_NAME} ${PYTHON_LIBRARIES})
    endif()
endif()

install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)

