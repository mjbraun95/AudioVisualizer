#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphmodifier.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::startAnimation()
//{
//    GraphModifier *modifier;
//    QTimer *timer = new QTimer(this);
//    QObject::connect(timer, &QTimer::timeout, modifier, &GraphModifier::animate);
//    timer->start(1);
//}
