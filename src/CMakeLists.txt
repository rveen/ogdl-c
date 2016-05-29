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

install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)
