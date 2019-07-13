
#include "view.h"
#include "scene.h"



int main(int argc, char **argv)
{
    //SceneFantacy scene;
    View::InitializeGlfw();

    Scene scene;

    View view;
    view.attach(&scene);

    view.create();
    view.exec();

    View::FreeGlfw();
}
