#include <cgRender.h>

void init() 
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    cout << "init" << endl;

    /*
       glShadeModel (GL_SMOOTH);

    // Enable lighting
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);

    // Set material parameters
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  MaterialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
    */
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cout << "display" << endl;

    /*
       for (all polygons)
       glBegin(GL_POLYGON);
       for (all vertices of polygon)
    // Define texture coordinates of vertex
    glTexCoord2f(...);
    // Define normal of vertex
    glNormal3f(...);
    // Define coordinates of vertex
    glVertex3f(...);
    }
    glEnd();
    }
    glFlush ();
    //  or, if double buffering is used,
    //  glutSwapBuffers();
    */
    }

void reshape (int w, int h)
{
    cout << "reshape" << endl;

    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    /*
       glMatrixMode (GL_PROJECTION);
       glLoadIdentity();
       gluPerspective(fovy, aspect, near, far);
       glMatrixMode (GL_MODELVIEW);
       glLoadIdentity();
       gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
       */
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
    }
}

int main(int argc, char** argv)
{

    // Check input arguments
    // and print a usage on failure
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << "and then a .vtk file, followed by 1 (meaning Gourand rendered "
            << "or 2 (meaning texture mapped render)" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize graphics window
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
    //  Or, can use double buffering
    //  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

    glutInitWindowSize (256, 256); 
    glutInitWindowPosition (0, 0);
    glutCreateWindow (argv[0]);

    // Initialize OpenGL
    init();

    // Initialize callback functions
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Start rendering 
    glutMainLoop();
}

