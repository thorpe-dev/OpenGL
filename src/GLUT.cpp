#include <parser.h>
#include <cgRender.h>
#include <shader_utils.h>

//global variables
static struct {
    
    vtk_file* data;
    
    int width,height;
    
    GLuint program;
    GLuint vertexBuffer;
    
    
} global;

void setupGlobal(void)
{
    global.data = NULL;
    global.width = 256;
    global.height = 256;
}

void display()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(global.program);
    
    glUniform2f(uniform_off,0.5f,0.25f);
    
    glBindBuffer(GL_ARRAY_BUFFER, global.vertexBuffer);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,(void*)color);
    
    glDrawArrays
}

void reshape()
{
    
}

void keyboard();
{
    
}








int main(int argc, char** argv)
{
    // Check input arguments
    // and print a usage on failure
    if (argc != 5)
    {
        cout << "Usage: " << argv[0] << "and then a .vtk file, "
            << "followed by  the path to the vertex shader and fragment_shader "
            << " lastly 1 (meaning Gourand rendered "
            << "or 2 (meaning texture mapped render)" << endl;
        exit(EXIT_FAILURE);
    }
    
    setupGlobal();
    
    global.data = parser::parser((string)read_file(argv[1])).get_vtk_file();
    
    glutInit(&argc,argv);
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_ALPHA|GLUT_DEPTH);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    
    glutInitWindowSize(global.width,global.height);
    
    glutCreateWindow(argv[0]);
    
    LoadFunctions();
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    exit(EXIT_SUCCESS);
}