#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "openglwindow.h"
//! [1]
class QOpenGLShaderProgram;
class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() override;
    //void render() override;
    void render(QPainter *painter);

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // MAINWINDOW_H
