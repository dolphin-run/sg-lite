#include "mainview.h"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    m_textNode = new SGTextNode();
    m_scene.addItem(m_textNode);
    m_textNode->setX(500);
    m_textNode->setY(200);
    m_textNode->setZ(3);
    m_textNode->setText(L"ABCDEFGHIj\n107");
}

MainView::~MainView()
{
    delete ui;
}

void MainView::on_btn_align_bottom_clicked()
{
    m_textNode->setVAlignment(VAlignment::AlignBottom);
}

void MainView::on_btn_align_vcenter_clicked()
{
    m_textNode->setVAlignment(VAlignment::AlignVCenter);
}

void MainView::on_btn_align_top_clicked()
{
    m_textNode->setVAlignment(VAlignment::AlignTop);
}

void MainView::on_btn_align_right_clicked()
{
    m_textNode->setHAlignment(HAlignment::AlignRight);
}

void MainView::on_btn_align_hcenter_clicked()
{
    m_textNode->setHAlignment(HAlignment::AlignHCenter);
}

void MainView::on_btn_align_left_clicked()
{
    m_textNode->setHAlignment(HAlignment::AlignLeft);
}

void MainView::on_btn_add_view_clicked()
{
    View *view = new View(this);
    view->attach(&m_scene);
    //view->viewCamera()->setFillMode(true);

    ui->verticalLayout->addWidget(view);

}

void MainView::on_btn_text_color_clicked()
{
    m_textNode->setColor(Color::random());
}
