//#include "cgRender.h"
#include <vtkGenericDataObjectReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataObject.h>
#include <vtkTexture.h>
#include <vtkImageData.h>
#include <vtkPNMReader.h>
#include <vtkTransform.h>

int main(int argc, char** argv) 
{
    // Check input arguments
    // and print a usage on failure
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << "and then a .vtk file, followed by a .ppm file" << endl;
        exit(EXIT_FAILURE);
    }

    vtkSmartPointer<vtkGenericDataObjectReader> inputReader 
        = vtkSmartPointer<vtkGenericDataObjectReader>::New();
    inputReader->SetFileName(argv[1]);
    inputReader->Update();
    
    vtkSmartPointer<vtkPNMReader> ppm 
        = vtkSmartPointer<vtkPNMReader>::New();
    ppm->SetFileName(argv[2]);
    
    vtkSmartPointer<vtkTexture> tex
        = vtkSmartPointer<vtkTexture>::New();
    tex->SetInput(ppm->GetOutput());
    tex->InterpolateOn();
    
    vtkSmartPointer<vtkTransform> trans
        = vtkSmartPointer<vtkTransform>::New();
    trans->Translate(0.0,0.05,0);
    
    tex->SetTransform(trans);
    
    // Lets visualise this stuff!!!!
    
    // Make a new Mapper and set its input to the readers output
    vtkSmartPointer<vtkPolyDataMapper> map 
        = vtkSmartPointer<vtkPolyDataMapper>::New();
    map->SetInputConnection(inputReader->GetOutputPort());

    // Make an actor and set its mapper to the mapper
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(map);
    actor->SetTexture(tex);

    // Make a new render and render window
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    
    // And add the renderer to the render window
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->AddActor(actor);
    renderer->SetBackground(0, 0, 0); // Background color black

    renderWindow->SetSize(256,256);
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
    
}
