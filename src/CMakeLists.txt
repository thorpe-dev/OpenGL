add_executable(cgRender parser.cpp)

find_package(VTK REQUIRED)
find_package(GLUT REQUIRED)

include(${VTK_USE_FILE})

target_link_libraries(cgRender vtkHybrid)

target_link_libraries(cgRender ${OPENGL_LIBRARY})
target_link_libraries(cgRender ${GLUT_LIBRARY})
