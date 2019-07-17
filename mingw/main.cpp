
#include "view.h"
#include "scene.h"



int main(int argc, char **argv)
{
    //SceneFantacy scene;
    View::InitializeGlfw();

    {
        //Force Scene&View destroyed before View::FreeGlfw();
        Scene scene;

        View view;
        view.attach(&scene);

        view.create("view1");


        View view1;
        view1.attach(&scene);

        view1.create("view2");

        View::exec();
    }

    View::FreeGlfw();
}
