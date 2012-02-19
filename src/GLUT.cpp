#include <parser.h>
#include <cgRender.h>
#include <shader_utils.h>

#include <iostream>

static struct {
    int screen_width;
    int screen_height;
    
    GLint rotate;
    GLuint program;
    GLint attribute_coord3d;
    GLint uniform_mvp;
    vtk_file* data;
    
    char* vertex_shader;
    char* fragment_shader;
    GLuint vbo_vertices;
    GLuint ibo_poly;
    
    GLint location;
} singleton_var;

void setup_globals(void)
{
    singleton_var.screen_width = 256;
    singleton_var.screen_height = 256;
    singleton_var.rotate = GL_FALSE;
}

GLfloat* calc_normals(void)
{
    GLfloat* ret = new GLfloat[singleton_var.data->polygons->size()];
    
    return ret;   
}

static bool init_resources(void)
{
    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    
    if ((vs = shader_create(singleton_var.vertex_shader, GL_VERTEX_SHADER)) == 0)
        return false;
    
    if ((fs = shader_create(singleton_var.fragment_shader, GL_FRAGMENT_SHADER)) == 0)
        return false;
    
    GLfloat* vertices = &(*singleton_var.data->points)[0];
    
    // Set up vertices
    glGenBuffers(1, &singleton_var.vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER,singleton_var.vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER,
                 (singleton_var.data->point_count) * 3 * sizeof(GLfloat),
                 vertices, 
                 GL_STATIC_DRAW);
    
    GLshort* poly = &(*singleton_var.data->polygons)[0];
    
    // Set up polygons
    glGenBuffers(1, &singleton_var.ibo_poly);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, singleton_var.ibo_poly);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 sizeof(GLshort) * singleton_var.data->polygons->size(), 
                 poly, 
                 GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    
    
    
    /*
    glGenBuffers(1, &singleton_var.ibo_poly);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,singleton_var.ibo_poly);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (singleton_var.data->polygons->size())*sizeof(GLfloat), 
                 poly,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);*/
    
    // ATTACH ALL OF THE SHADERS!
    singleton_var.program = glCreateProgram();
    glAttachShader(singleton_var.program,vs);
    glAttachShader(singleton_var.program,fs);
    glLinkProgram(singleton_var.program);
    glGetProgramiv(singleton_var.program,GL_LINK_STATUS, &link_ok);
    
    if (!link_ok)
    {
        cout << "glLinkProgram: ";
        print_log(singleton_var.program);
        return false;
    }
    
    const char* attribute_name;
    attribute_name = "coord3d";
    singleton_var.attribute_coord3d = 
        glGetAttribLocation(singleton_var.program, attribute_name);
    if (singleton_var.attribute_coord3d == -1)
    {
        cout << "Could not bind attribute" << attribute_name << endl;
        return false;
    }
    
    const char* uniform_name;
    uniform_name = "mvp";
    singleton_var.uniform_mvp = glGetUniformLocation(singleton_var.program, uniform_name);
    if (singleton_var.uniform_mvp == -1)
    {
        cout << "Could not bind uniform " << uniform_name << endl;
        return false;
    }
    
    /*const char* light_name;
    light_name = "light0Color";
    singleton_var.location = glGetUniformLocation(singleton_var.program,light_name);
    if (singleton_var.location == -1)
    {
        cout << "Could not bind light " << light_name << endl;
        return false;
    }
    
    GLfloat colour[4] = {0.4f,0,1,1};
    
    glUniform4fARB(singleton_var.location,0.4f,0,1,1);*/
    return true;    
}

static void idle_func(void)
{
    //TODO: make this not be crazy
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45° per second
    glm::vec3 axis_y(0, 1, 0);
    //glm::mat4 anim = glm::rotate(glm::mat4(0.4f), angle, axis_y);
    
    glm::mat4 model = glm::translate(glm::mat4(0.4f), glm::vec3(0.0,0.0,-4.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0,1.0,0.0),glm::vec3(0.0,0.0,-4.0), glm::vec3(0.0,1.0,0.0));
    glm::mat4 projection = glm::perspective(7.0f,1.0f*singleton_var.screen_width/singleton_var.screen_height, 0.1f,10.0f);
    
    glm::mat4 mvp = projection * model;
    
    glUseProgram(singleton_var.program);
    
    glUniformMatrix4fv(singleton_var.uniform_mvp,1, GL_FALSE, glm::value_ptr(mvp));
    
    glutPostRedisplay();
}

static void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    
    glUseProgram(singleton_var.program);
    
    glEnableVertexAttribArray(singleton_var.attribute_coord3d);    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, singleton_var.vbo_vertices);
    
    glVertexAttribPointer(
        singleton_var.attribute_coord3d,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, singleton_var.ibo_poly);
    
    int size;
    
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort),GL_UNSIGNED_SHORT,0);
    
    glDisableVertexAttribArray(singleton_var.attribute_coord3d);
    
    glFlush();
    
    glutSwapBuffers();
}

static void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 'q' : case 'Q' :
            exit(EXIT_SUCCESS);
            break;
        case 'r' : case 'R' :
            singleton_var.rotate = GL_TRUE;
            break;
    }
}

static void onReshape(int width, int height)
{
    singleton_var.screen_width = width;
    singleton_var.screen_height = height;
    glViewport(0,0,width, height);
}

static void free_resources(void)
{
    glDeleteProgram(singleton_var.program);
    glDeleteBuffers(1, &singleton_var.ibo_poly);
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
    
    setup_globals();
    
    singleton_var.data = parser::parser((string)read_file(argv[1])).get_vtk_file();
    singleton_var.vertex_shader = argv[2];
    singleton_var.fragment_shader = argv[3];
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(singleton_var.screen_width,singleton_var.screen_height);
    glutCreateWindow(argv[0]);

    

    GLenum glew_status = glewInit();

    if (!GLEW_VERSION_2_0 || glew_status != GLEW_OK)
    {
        cout << "Error is " << glewGetErrorString(glew_status) << endl;
        exit(EXIT_FAILURE);
    }

    if (init_resources())
    {   
        glutIdleFunc(idle_func);
        glutReshapeFunc(onReshape);
        glutKeyboardFunc(keyboard);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_RESCALE_NORMAL);
        glEnable (GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE  );
        

        glutDisplayFunc(render);
        glutMainLoop();
    }
    else
    {
        cout << "Failed to load resources" << endl;
        exit(EXIT_FAILURE);
    }

    free_resources();
    return EXIT_SUCCESS;
}