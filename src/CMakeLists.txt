find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)

set(DEPS ${CONAN_LIBS} ${OPENGL_LIBRARIES} ${OpenCL_LIBRARY})

file(GLOB examples ${CMAKE_CURRENT_LIST_DIR}/*)
list(REMOVE_ITEM examples ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt)

foreach(example ${examples})
    get_filename_component(exe ${example} NAME_WE)
    add_executable(${exe} ${exe}.cc)
    target_link_libraries(${exe} ${DEPS})
endforeach()
