aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} REACTOR_SRC_FILES)

add_library(reactor SHARED ${REACTOR_SRC_FILES})

#find_package(PkgConfig REQUIRED)
#
#pkg_check_modules(liburing REQUIRED liburing)
#
#target_include_directories(reactor
#    PRIVATE
#    ${liburing_INCLUDE_DIRS})
#
#target_link_directories(reactor
#    PRIVATE
#    ${liburing_LIBRARIES})

add_library(uring SHARED IMPORTED)
set_target_properties(uring PROPERTIES IMPORTED_LOCATION /usr/lib/liburing.so)

target_link_libraries(reactor
    PRIVATE
    uring
)