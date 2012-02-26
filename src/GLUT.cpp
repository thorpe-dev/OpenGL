#include <cgRender.h>

//global variables
static struct {

    vector<glm::vec3>* vertex;
    vector<glm::vec3>* normals;
    vector<glm::vec3>* polygons;
    vector<glm::vec2>* tex;

    int width,height;
    vector<float>* cam;
    vector<float>* centre;
} global;

void setupGlobal(void)
{
    global.width = 512;
    global.height = 512;
    float cam[3] = {4.3f,4.2f,-5.1f};
    float centre[3] = {0.06f,-0.13f,-0.08f};
    global.cam = new vector<float>(cam, cam + sizeof(cam)/sizeof(float));
    global.centre = new vector<float>(centre, centre + sizeof(centre)/sizeof(float));
}

void display()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 poly;

    for (int i = 0; i < (int)global.polygons->size(); i++)
    {
        poly = (*global.polygons)[i];

        glBegin(GL_POLYGON);
        
        for (int j = 0; j < 3; j++)
        {
            glNormal3fv(glm::value_ptr((*global.normals)[poly[j]]));
            //glVertex3fv(glm::value_ptr((*global.vertex)[poly[j]]));
            glVertex3fv(glm::value_ptr((*global.vertex)[poly[j]]));
        }
        glEnd();
    }

    glFlush();

    glutPostRedisplay();
}

void reload()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt((*global.cam)[0],(*global.cam)[1],(*global.cam)[2],
            (*global.centre)[0],(*global.centre)[1],(*global.centre)[2],
            0, 1, 0
            );
}

inline float to_radians(float degrees)
{
    return (4 * atan(1) * degrees / 180);
}

void reshape (int x, int y)
{
    glViewport (0, 0, x, y); 

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(to_radians(90), 1, 0.1, 100);

    reload();
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

    cout << "Allocated vectors" << endl << endl;
    
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

glm::vec3 findNormal(vector<glm::vec3> polyPoints)
{
    return (glm::normalize(glm::cross(polyPoints[2] - polyPoints[0], 
                polyPoints[1] - polyPoints[0])));
}



void buildNormals(void)
{
    vector<glm::vec3> points;
    glm::vec3 norm;
    vector<glm::vec3> polygons = *global.polygons;

    BOOST_FOREACH(glm::vec3 poly, polygons)
    {
        points = vector<glm::vec3>(3);
        for (int i = 0; i < 3; i++)
        {
            cout << poly[i];
            points[i] = (*global.vertex)[poly[i]];
        }

        norm = findNormal(points);

        for (int i = 0; i < 3; i++)
        {
            (*global.normals)[(int)poly[i]] += norm;
        }
    }

    for (int i = 0; i < (int)global.vertex->size();i++)
    {
        (*global.normals)[i] = glm::normalize((*global.normals)[i]);
    }
}

void init()
{
    cout << "Initialising" << endl;
    glClearColor(0.0,0.0,0.0,0.0);

    glShadeModel(GL_FLAT);
    
//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
//     glEnable (GL_TEXTURE_2D);
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

    vtk_file* vtk = parser::parser((char*)read_file(argv[1]).c_str(),true).get_vtk_file();
    ppm_file* ppm = parser::parser(argv[2],false).get_ppm_file();
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_SINGLE|GLUT_DEPTH|GLUT_RGB);

    glutInitWindowSize(global.width,global.height);
    
    glutInitWindowPosition(256,256);

    glutCreateWindow(argv[0]);

    cout << "Building vectors" << endl;
    buildVectors(vtk);

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    exit(EXIT_SUCCESS);
}
