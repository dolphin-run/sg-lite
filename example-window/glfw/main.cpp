
#include "view.h"
#include "scene.h"
#include "scenefantacy.h"



int main(int argc, char **argv)
{
    //SceneFantacy scene;
    View::InitializeGlfw();

    {
        //Force Scene&View destroyed before View::FreeGlfw();
        //Scene scene;
        SceneFantacy scene;

        View view;
        view.attach(&scene);

        view.create("view1");

#if 0
        View view1;
        view1.attach(&scene);

        view1.create("view2");
#endif

        View::exec();
    }

    View::FreeGlfw();
}
