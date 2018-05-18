#include "mainview.h"
#include "ui_mainview.h"
#include "node/sgrectanglenode.h"

MainView::MainView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    m_textNode = new SGTextNode();
    m_scene.addItem(m_textNode);
    m_textNode->setX(500);
    m_textNode->setY(700);
    m_textNode->setZ(3);
    m_textNode->setText(L"ABÖÐ¹úgij.\"\nÒ»ÑÔ¾Å¶¦");
    m_textNode->setColor(0xff0000);

    auto txtr = new SGRectangleNode();
    txtr->setX(500);
    txtr->setY(700);
    txtr->setZ(2);
    txtr->setColor(0x00ff00);
    txtr->setSize(m_textNode->width(), m_textNode->height());
    m_scene.addItem(txtr);

    auto r1 = new SGRectangleNode();
    r1->setX(400);
    r1->setY(100);
    r1->setZ(1);
    r1->setColor(0xffff00);
    r1->setSize(400, 400);
    m_scene.addItem(r1);

    auto r2 = new SGRectangleNode(r1);
    r2->setX(0);
    r2->setY(0);
    r2->setColor(0x5f0000);
    r2->setSize(100, 100);

    auto r3 = new SGRectangleNode(r1);
    r3->setX(100);
    r3->setY(100);
    r3->setColor(0xff0000);
    r3->setSize(100, 100);
    r3->setRotation(-3.1416f / 6.f);

    //
    auto r11 = new SGRectangleNode();
    r11->setX(400);
    r11->setY(600);
    r11->setColor(0xffffff);
    r11->setSize(400, 400);
    m_scene.addItem(r11);
    r11->setRotation(-3.1416f / 6.f);
    r11->setScale(1.5f);

    auto r21 = new SGRectangleNode(r11);
    r21->setX(0);
    r21->setY(0);
    r21->setColor(0x5f0000);
    r21->setSize(100, 100);

    auto r31 = new SGRectangleNode(r11);
    r31->setX(100);
    r31->setY(100);
    r31->setColor(0xff0000);
    r31->setSize(100, 100);
    r31->setRotation(-3.1416f/6.f);
    //r31->setScale(2.0);
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
