
# sg-lite

## Introduction

A lite and high performance render engine with scene&graph model, written with c++11 purely.
OpenGL ES2.0 currently used backend but maybe others in the future.

As a lite engine, the library only build the scene and view with some common nodes within it, 
You should integrate it with some OpenGL Window Framework.
Qt5 and GLFW have been used for examples.

sg-lite can be used cross-platform, Windows and MacOS have been test, users can migrate it to other OS easily.

Below is a simple screen-shot which renders an fantacy binary tree recurcively with multiview.
<image src="https://www.evernote.com/shard/s364/sh/0bf01f6e-6cf6-44a7-9338-9e28b5a8c5dc/a7e15d4ed0148f3d/res/c30849f1-bc7e-4bf1-a01d-aaa371ed2eb6/demo.gif">


## Quick Start
Windows as example
### build library
go to `src` and build it with Qt or makefile.
1. with Qt, you can open the `sgcore.pro` file to manipulate the project. or with msvc-qt just cmd like this:
```
qmake
nmake release
nmake install
```
2. with mingw, 
```
make mingw
make install
```

`install` will install the library to parent direcory, which copies header files to include folder and dll to lib folder

### run example
go to `examples` and start with Qt or makefile.
1. copy the `sgcore.dll` `assets` to app output directory, eg. `example/bin/x64`
2. glfw can be build and run with mingw
3. qt5* can be build with *.pro file

more details here <a href="#Examples">Examples</a>

### write yourself

sg-lite is designed as a scene&view model, perpare a scene and bind it to a view or more than one, then it will make drawing automatically. 
User can edit the scene or the node during drawing,
such as add node to the scene, modify the properties of some node, to make animations.

as opengl only defines how to rendering, you need OpenGL API from system and some Window with opengl context to display it.
sg-lite use GLAD or GLEW to call OpenGL functions, use GLFW or Qt as Window container, 
For quick starter can go to example folder to get how it working.

If start with the library step by step, following these:
1. extend from SGView to implement A UserView according to the windows system. the key point is to perpare the Window and OpenGL Context for rendering later.
```
class UserView : public SGView, public SystemWindow
{
public:
    bool createWindow();//perpare the window and setup OpenGL context
    void destroy();//exit with free all system resource.

    void exec();//running the rendering loop.
private:
    Window* m_window;//System window.
};
```
2. extend from SGScene to create A UserScene and add some nodes.
```
class UserScene : public SGScene
{
public:
    void buildScene();//init the scene
    
    void addNode(...);//add some node
    void removeNode(...);//remove node
    void setNode(SGNode *node, ...);//edit properties
private:
    std::list<SGNode*> m_nodes;//for indexing
};

void UserScene::buildScene()
{
    for (int i = 0; i < 20; i++)
    {
        //add some solid rectangle
        int row = i / 10;
        int col = i % 10;
        SGRectangleNode *sn = new SGRectangleNode();
        sn->setPos(col * 200, row * 200);
        sn->setSize(150, 100);
        sn->setColor(Color::random());

        this->addItem(sn);
    }
};
```
3. bind UserScene to UserView
`window->attach(scene);`
4. start the render loop, each period with some code like this:

```
while(not stop)
{
    MakeContextCurrent(window);
    SGView::renderFrame();
    SwapBuffers(window);
}
```

Currently Scene is equipped with some common nodes, 
eg. rectangle node with solid color, image rectangle node, text node, video node, 
each node maybe be defined with some unique properties.
User can extend UserNode from SGNode with it like.

more details here <a href="#Node Type">Node Type</a>

## Examples
examples with sg-lite, which can be build&run in multi-platform with Qt or GLFW.

+ example/glfw  
example with glfw, which can be build with makefile

+ example/qt5quick
+ example/qt5widget
+ example/qt5window  
examples for Qt5(for modern and classic code, QtQuick&QtWidget), which can be all build easily with qt5example.pro

+ example/assets  
image and video resources, which are used in examples.

+ example/example-scene  
scenes will be shared with all examples.
    - *scene.h* render some basic nodes like rectangles and rings, some animation added for interesting.
    - *scenefantacy.h* render a binary tree recursively.
    - *scene video.h* render a video with yuv420. 

## Project Directories

Dir        | Description
-----------------|---------------------------------------------------
include |  sg-lite header files
lib     | sg-lite library pre-built.
src     | sg-lite library source code.
example        | examples with sg-lite, which can be build&run in multi-platform with Qt or GLFW.
 
## Node Type

Node        |  properties           |   Description
-----------------|---------------------------------------------------|-----------------------
SGNode          |   position(x, y, z-index)<br> size(width, height)<br> transformation(scale, rotation)<br> visibility<br> opacity    |   basic node
SGRectangleNode |  color(rgba)            | solid color rectagle
SGImageNode     | src(image path)           | a jpg/png image
SGYuvVideoNode  | frame(yuv buffer)         | play video by setting yuv frame buffer
SGRgbVideoNode  | frame(rgb buffer)         | play video by setting rgb frame buffer
SGCircularNode  | innerRadius<br> color(colorInside, colorOutside) | A ring or a circle
SGTextNode      | string<br> color<br> alignment    | a wide-string, support line separator

## References
### OpenGL Wrapper
[GLAD](https://glad.dav1d.de/) can generate a portable GL Loader hpp/cpp file, OpenGL functions can be called after you include the glad.h without linking any library.

[GLEW](http://glew.sourceforge.net/) works almost like GLAD except some libraries, eg. glew32 opengl32, maybe need linking.

### Window Framework
[Qt](https://www.qt.io/) can be used to develop OpenGL application in various ways.

+ [QOpenGLWidget](https://doc.qt.io/qt-5/qopenglwidget.html) can be used like Qt Widget but make painting with OpenGL.

+ [QWindow](https://doc.qt.io/qt-5/qwindow.html) can be set to OpenGL surface, rendering with OpenGL with an QOpenGLContext.

+ [QQuickFramebufferObject](https://doc.qt.io/qt-5/qquickframebufferobject.html) is used in QtQuick, which comes to the modern qt style with qml.

[GLFW](https://www.glfw.org/) is a library for OpenGL application development, it provides OS dependent utilites, such as Windows, Context, IO, which OpenGL application need. 

### Utilites
[stb](https://github.com/nothings/stb) single-file public domain (or MIT licensed) libraries for C/C++. 

+ *stb_image.h* is used to load image files;
+ *stb_truetype.h* is used to load font files, it also supports calculating SDF when,
which helps a lot when sg-lite do text rendering.

[freetype](https://www.freetype.org/) is a freely available software library to render fonts.
it also helps to render a string with its perfect documents although removed from the source code.

[glm](https://github.com/g-truc/glm) sg-lite use it to do vector&matrix transformation.

### Nice Projects
[ejoy2d](https://github.com/ejoy/ejoy2d) a lite game engine.

[Qt Quick Scene Graph](https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#scene-graph-and-rendering) Qt Quick rendering engine background. sg-lite rendering more or less comes from it.

## Warning
+ Some api maybe thread-safe some maybe not, For safety, changes to scene&nodes should be synchronized to rendering(SGView::renderFrame()).
+ SGView::setSize() and SGView::setSizePx() should be called with new size when System Window size changed. 
Why not supply an virtual interface for UserView to Override, for effecient simply.
+ SGView rendering can be diffrent for each one, this can be important when use one scene with multi-view, if you want to 
view it in diffrent aspect. this will be done in the near future.
+ Opacity of Node has not implement currently.
+ Text point size and bounding rect are not support.
