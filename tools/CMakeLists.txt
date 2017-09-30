
link_directories(${CMAKE_BINARY_DIR}/src)
include_directories(../src)

add_executable(gpath gpath.c)
target_link_libraries(gpath libogdl.a)

add_executable(tindent tindent.c)
target_link_libraries(tindent libogdl.a)
	
add_executable(ogdl2dot ogdl2dot.c)
target_link_libraries(ogdl2dot libogdl.a)

find_package(EXPAT REQUIRED)
if(${EXPAT_FOUND})
    add_executable(xml2ogdl xml2ogdl.c)
    target_link_libraries(xml2ogdl libogdl.a ${EXPAT_LIBRARIES})
    install(TARGETS gpath tindent ogdl2dot xml2ogdl DESTINATION bin)
else()
    message(INFO " - No expat XML stream library found! Can't build xml2ogdl...")
    install(TARGETS gpath tindent ogdl2dot DESTINATION bin)
endif()   

