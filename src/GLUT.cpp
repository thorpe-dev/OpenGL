#include <parser.h>
#include <cgRender.h>
#include <shader_utils.h>

//global variables
static struct {
    
    vtk_file* data;
    
} globals;

void setupGlobals(void)
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
    
    setupGlobals();
    
    //globals.data = parser::parser((string)read_file(argv[1])).get_vtk_file();
}