#include <parser.h>
#include <cgRender.h>
#include <string>
#include <iostream>
#include <fstream>


static bool make_resources(void)
{
    return true;    
}

static void idle_func(void)
{
}

static void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

vtk_file* read_file(char* c)
{
    //int length;
    ifstream inputFile(c);


    // get length of file:
    inputFile.seekg (0, std::ios::end);
    int length = inputFile.tellg();
    inputFile.seekg (0, std::ios::beg);

    // allocate memory for the string
    char* buffer = new char[length];
    
    //Read file into string
    inputFile.read (buffer,length);
    inputFile.close();
    std::string vtk_input = buffer;
    
    //Parse the file
    cout << "Parsing vtk file: " << c << endl;
    
    return parser::parser((string)buffer).get_vtk_file();
}

int main(int argc, char** argv)
{

    // Check input arguments
    // and print a usage on failure
   using namespace std;
    // Check input arguments
    // and print a usage on failure
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << "and then a .vtk file, "
            << "followed by 1 (meaning Gourand rendered "
            << "or 2 (meaning texture mapped render)" << endl;
        exit(EXIT_FAILURE);
    }    
    vtk_file* file = read_file(argv[1]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(256,256);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(&render);
    glutIdleFunc(&idle_func);
    
    glewInit();
    
    if (!GLEW_VERSION_2_0)
    {
        cout << "OpenGL version is too low" << endl;
        exit(EXIT_FAILURE);
    }
    
    if (!make_resources())
    {
        cout << "Failed to load resources" << endl;
        exit(EXIT_FAILURE);
    }
    
    glutMainLoop();
    
    return EXIT_SUCCESS;
    
    
    
}