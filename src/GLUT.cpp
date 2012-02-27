#include <cgRender.h>

//global variables
static struct {

    vector<glm::vec3> vertex;
    vector<glm::vec3> normals;
    vector<glm::vec3> polygons;
    vector<glm::vec2> tex;
    
    ppm_file* ppm;

    int width,height;
    float* cam;
    float* centre;
} global;

void setupGlobal(void)
{
    global.width = 512;
    global.height = 512;
    float cam[3] = {0.36,-0.1448,-0.14};
    float centre[3] = {0.05,-0.105,-0.06};
    global.cam = cam;
    global.centre = centre;
}

void display()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 poly;

    for (int i = 0; i < (int)global.polygons.size(); i++)
    {
        poly = (global.polygons)[i];

        glBegin(GL_POLYGON);
        
        for (int j = 0; j < 3; j++)
        {
//             glNormal3fv(glm::value_ptr((global.normals)[poly[j]]));
//             glVertex3fv(glm::value_ptr((*global.vertex)[poly[j]]));
            glVertex3fv(glm::value_ptr((global.vertex)[poly[j]]));
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
    gluLookAt(global.cam[0],global.cam[1],global.cam[2],
            global.centre[0],global.centre[1],global.centre[2],
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
    gluPerspective(100, (float) x / (float) y, 0.1, 1.2);
//     gluPerspective(to_radians(90), 1, 0.1, 100);

    reload();
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 27: // ESC
        exit(0);
        break;
        case 'q':
            global.cam[0] -= 0.1;
            break;
        case 'w':
            global.cam[0] += 0.1;
            break;
        case 'a':
            global.cam[1] -= 0.1;
            break;
        case 's':
            global.cam[1] += 0.1;
            break;
        case 'z':
            global.cam[2] -= 0.1;
            break;
        case 'x':
            global.cam[2] += 0.1;
            break;
        case 'e':
            global.centre[0] -= 0.01;
            break;
        case 'r':
            global.centre[0] += 0.01;
            break;
        case 'd':
            global.centre[1] -= 0.01;
            break;
        case 'f':
            global.centre[1] += 0.01;
            break;
        case 'c':
            global.centre[2] -= 0.01;
            break;
        case 'v':
            global.centre[2] += 0.01;
            break;
    }

}

void buildVectors(vtk_file* data)
{
    vector<glm::vec3> vertex = vector<glm::vec3>(data->point_count);
    vector<glm::vec3> normals = vector<glm::vec3>(data->point_count);
    vector<glm::vec3> polygons = vector<glm::vec3>(data->polygon_count);
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

    cout << data->textures->size() << endl;
    cout << data->texture_count << endl;
    for(j = 0; j < (int)data->texture_count; j+=2)
    {
        t = glm::vec2(
                (*data->textures)[j],
                (*data->textures)[j + 1]
                );
        tex[j] = t;
    }

    cout << "Texture data added to vector" << endl;

    global.vertex = vertex;
    global.normals = normals;
    global.polygons = polygons;
    global.tex = tex;
}

glm::vec3 findNormal(glm::mat3 polyPoints)
{
    return (glm::normalize(glm::cross(polyPoints[2] - polyPoints[0], 
                polyPoints[1] - polyPoints[0])));
}



void buildNormals(void)
{
    cout << "Building normals" << endl;
    glm::mat3 points;
    cout << "1" << endl;
    glm::vec3 norm;
    cout << "2" << endl;
    
    cout << global.polygons[0][0] << endl;
    cout << "Allocating done" << endl;

    BOOST_FOREACH(glm::vec3 poly, global.polygons)
    {
        for (int i = 0; i < 3; i++)
        {
            points[i] = (global.vertex)[poly[i]];
        }

        norm = findNormal(points);

        for (int i = 0; i < 3; i++)
        {
            (global.normals)[(int)poly[i]] += norm;
        }
    }

    for (int i = 0; i < (int)global.vertex.size();i++)
    {
        (global.normals)[i] = glm::normalize((global.normals)[i]);
    }
}

void init()
{
    cout << "Initialising" << endl;
    glClearColor(0.0,0.0,0.0,0.0);
    
    cout << "Generating textures" << endl;
    glGenTextures(1, (GLuint*)global.ppm->texture);
    cout << "Binding textures" << endl;
    glBindTexture(GL_TEXTURE_2D,*global.ppm->texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    cout << "Set repeat paramters" << endl;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,global.ppm->height,global.ppm->width,
                 0,GL_RGB,GL_UNSIGNED_BYTE,global.ppm->texture);

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
    glEnable (GL_TEXTURE_2D);
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

    vtk_file* vtk = parser::parser(argv[1],true).get_vtk_file();
    global.ppm = parser::parser(argv[2],false).get_ppm_file();
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_SINGLE|GLUT_DEPTH|GLUT_RGB);

    glutInitWindowSize(global.width,global.height);
    
    glutInitWindowPosition(256,256);

    glutCreateWindow(argv[0]);

    cout << "Building vectors" << endl;
    buildVectors(vtk);
    buildNormals();

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    exit(EXIT_SUCCESS);
}
