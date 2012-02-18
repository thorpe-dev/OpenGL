add_executable(cgRender GLUT.cpp parser.cpp shader_utils.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(GLEW REQUIRED)

target_link_libraries(cgRender vtkHybrid)

target_link_libraries(cgRender ${OPENGL_LIBRARY} ${GLUT_LIBRARY} GLEW)