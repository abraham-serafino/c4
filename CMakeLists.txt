cmake_minimum_required(VERSION 3.10)

project(C4 C)

set(CMAKE_C_STANDARD 23)

include_directories(src)

set(POINTER_SOURCES
    src/throwException.c
    src/pointer.c
    src/c4defaults.c
    src/pointer.test.c
)

add_executable(pointer-test ${POINTER_SOURCES})

#set(CMAKE_C_EXTENSIONS OFF)

#add_subdirectory(vendor/libatomic_ops)
#add_subdirectory(vendor/bdwgc)

#target_link_libraries(pointer-test PRIVATE gc)
#target_include_directories(pointer-test PRIVATE vendor/bdwgc/include)

#set(ARRAY_SOURCES
#    src/throwException.c
#    src/pointer.c
#    src/Array.c
#    src/c4defaults.c
#    src/Array.test.c
#)

#add_executable(array-test ${ARRAY_SOURCES})

#target_link_libraries(array-test PRIVATE gc)
#target_include_directories(array-test PRIVATE vendor/bdwgc/include)

#set(PROJECT_SOURCES
#    src/types.c
#    src/throwException.c
#    src/pointer.c
#    src/Array.c
#    src/c4defaults.c
#    src/Person.c
#    test.c
#)

#add_executable(c4_test ${PROJECT_SOURCES})
#target_link_libraries(c4_test PRIVATE gc)
#target_include_directories(c4_test PRIVATE vendor/bdwgc/include)
