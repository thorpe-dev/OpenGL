#include <parser.h>
#include <cgRender.h>
#include <shader_utils.h>

//global variables
static struct {
    
    vector<glm::vec3>* vertex;
    vector<glm::vec3>* normals;
    vector<glm::vec3>* polygons;
    vector<glm::vec2>* tex;
    
    int width,height;
    
    
} global;

void setupGlobal(void)
{
    global.width = 256;
    global.height = 256;
}

void display()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glm::vec3 poly;
    
    for (int i = 0; i < (int)global.polygons->size(); i++)
    {
        poly = (*global.polygons)[i];
        
        glBegin(GL_TRIANGLES);
        
        glVertex3fv(&(*global.vertex)[poly[0]][0]);
        glVertex3fv(&(*global.vertex)[poly[1]][0]);
        glVertex3fv(&(*global.vertex)[poly[2]][0]);
        
        glEnd();
    }
    
    glFlush();
    
    glutPostRedisplay();
}

void reshape (int x, int y)
{
  glViewport (0, 0, x, y); 
  
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(1.57, 1, 0.1, 100); /* 1.57 is ~90 degrees */

  //refreshPositions();
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 'q' : case 'Q':
            exit(EXIT_SUCCESS);
            break;
    }
}

void buildVectors(vtk_file* data)
{
    vector<glm::vec3> vertex = vector<glm::vec3>(data->point_count);
    vector<glm::vec3> normals = vector<glm::vec3>(data->point_count);
    vector<glm::vec3> polygons = vector<glm::vec3>(data->polygon_no);
    vector<glm::vec2> tex = vector<glm::vec2>(data->texture_count);
    
    cout << "Allocated vectors" << endl;
    glm::vec3 vert;
    glm::vec3 norm;
    glm::vec3 poly;
    glm::vec2 t;
    
    int i,j = 0;
    while( j < (int)data->points->size())
    {
        //cout << i << "," << j << endl;
        vert = glm::vec3(
            (*data->points)[j++],
            (*data->points)[j++],
            (*data->points)[j++]
        );
        
        vertex[i] = vert;
        
        norm = glm::vec3(0.0,0.0,0.0);
        normals[i] = norm;
        ++i;
    }
    
    cout << "Points data addded to vector" << endl;
    
    i = 0;
    while (j < (int)data->polygons->size())
    {
        poly = glm::vec3(
            (*data->polygons)[j++],
            (*data->polygons)[j++],
            (*data->polygons)[j++]
        );
        
        polygons[i] = poly;
        
        i++;
    }
    cout << "Polygon data added to vector" << endl;
    i = 0;
    
    for(j = 0; j < (int)data->point_data->size(); j+=2)
    {
        t = glm::vec2(
            (*data->point_data)[j*2],
            (*data->point_data)[j*2 + 1]
        );
    }
    
    cout << "Texture data added to vector" << endl;
    
    global.vertex = &vertex;
    global.normals = &normals;
    global.polygons = &polygons;
    global.tex = &tex;
    
    delete(data);
}

void buildNormals(void)
{
}

void init()
{
    cout << "Initialising" << endl;
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    glShadeModel(GL_FLAT);
    
    GLfloat position[4] = {-20.0f, 20.0f, 100.0f, 0.0f};
    const GLfloat ambient[4] = {0.1f,0.1f,0.1f,1.0f};
    const GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    const GLfloat spec[4] = {1.0f,1.0f,1.0f,1.0f};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,spec);
    
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv)
{
    // Check input arguments
    // and print a usage on failure
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << "and then a .vtk file, "
            << "then a texture file" << endl;
        exit(EXIT_FAILURE);
    }
    
    setupGlobal();
    
    vtk_file* data = parser::parser((string)read_file(argv[1])).get_vtk_file();    
    glutInit(&argc,argv);
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    
    glutInitWindowSize(global.width,global.height);
    
    glutCreateWindow(argv[0]);
    
    cout << "Building vectors" << endl;
    buildVectors(data);
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    exit(EXIT_SUCCESS);
}