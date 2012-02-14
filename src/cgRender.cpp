//#include "cgRender.h"
#include <vtkGenericDataObjectReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataObject.h>

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

    std::string inputFile = argv[1];

    vtkSmartPointer<vtkGenericDataObjectReader> inputReader 
        = vtkSmartPointer<vtkGenericDataObjectReader>::New();
    inputReader->SetFileName(inputFile.c_str());

    inputReader->Update();
    
    // Lets visualise this stuff!!!!
    
    // Make a new Mapper and set its input to the readers output
    vtkSmartPointer<vtkPolyDataMapper> map 
        = vtkSmartPointer<vtkPolyDataMapper>::New();
    map->SetInputConnection(inputReader->GetOutputPort());

    // Make an actor and set its mapper to the mapper
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(map);

    // Make a new render and render window
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    
    // And the renderer to the render window
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->AddActor(actor);
    renderer->SetBackground(0, 0, 0); // Background color green

    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
    
}
