add_executable(cgRender OpenGL_cgRender.cpp parser.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(cgRender vtkHybrid)

target_link_libraries(cgRender ${OPENGL_LIBRARY})
target_link_libraries(cgRender ${GLUT_LIBRARY})
