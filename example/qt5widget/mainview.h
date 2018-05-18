#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include "view.h"
#include "sgviewcamera.h"
#include "scene.h"
#include "scenefantacy.h"
#include "scenevideo.h"
#include "node/sgtextnode.h"

namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private slots:
    void on_btn_add_view_clicked();

    void on_btn_align_bottom_clicked();

    void on_btn_align_vcenter_clicked();

    void on_btn_align_top_clicked();

    void on_btn_align_right_clicked();

    void on_btn_align_hcenter_clicked();

    void on_btn_align_left_clicked();

    void on_btn_text_color_clicked();

private:
    Ui::MainView *ui;

    SceneFantacy m_scene;
    SGTextNode *m_textNode;
};

#endif // MAINVIEW_H
