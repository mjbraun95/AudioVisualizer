/*
  Names:  Ang Li  Matthew Braun
  IDs:    1550746 1497171
  CMPUT 275, Winter 2019

  Project: audio visualizer
*/
#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QFontComboBox>
#include <QLabel>
#include <QMessageBox>
#include <Q3DBars>
#include <QtDataVisualization>
#include <QItemModelBarDataProxy>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "graphmodifier.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <QElapsedTimer>
#include <QGroupBox>
#include <QRadioButton>

using namespace QtDataVisualization;

int main(int argc, char *argv[])
{
    // Initialization application
    // Found from https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html
    QApplication app(argc, argv);
    Q3DBars *widgetgraph = new Q3DBars();
    QWidget *container = QWidget::createWindowContainer(widgetgraph);

    if (!widgetgraph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }
    QSize screenSize = widgetgraph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    // Set program title
    widget->setWindowTitle(QStringLiteral("3D Magnitude-Frequency-Time Visualization"));

    // Adds camera orientation options, zoom options, bar shape options, bar select options
    // Found from https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html
    QCheckBox *smoothCheckBox = new QCheckBox(widget);
    smoothCheckBox->setText(QStringLiteral("Smooth bars"));
    smoothCheckBox->setChecked(false);
    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));
    QPushButton *zoomToSelectedButton = new QPushButton(widget);
    zoomToSelectedButton->setText(QStringLiteral("Zoom to selected bar"));
    QComboBox *selectionModeList = new QComboBox(widget);
    selectionModeList->addItem(QStringLiteral("None"),
                               int(QAbstract3DGraph::SelectionNone));
    selectionModeList->addItem(QStringLiteral("Bar"),
                               int(QAbstract3DGraph::SelectionItem));
    selectionModeList->addItem(QStringLiteral("Row"),
                               int(QAbstract3DGraph::SelectionRow));
    selectionModeList->addItem(QStringLiteral("Bar and Row"),
                               int(QAbstract3DGraph::SelectionItemAndRow));
    selectionModeList->addItem(QStringLiteral("Column"),
                               int(QAbstract3DGraph::SelectionColumn));
    selectionModeList->addItem(QStringLiteral("Bar and Column"),
                               int(QAbstract3DGraph::SelectionItemAndColumn));
    selectionModeList->addItem(QStringLiteral("Row and Column"),
                               int(QAbstract3DGraph::SelectionRowAndColumn));
    selectionModeList->addItem(QStringLiteral("Bar, Row and Column"),
                               int(QAbstract3DGraph::SelectionItemRowAndColumn));
    selectionModeList->addItem(QStringLiteral("Slice into Row"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionRow));
    selectionModeList->addItem(QStringLiteral("Slice into Row and Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow));
    selectionModeList->addItem(QStringLiteral("Slice into Column"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionColumn));
    selectionModeList->addItem(QStringLiteral("Slice into Column and Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn));
    selectionModeList->addItem(QStringLiteral("Multi: Bar, Row, Col"),
                               int(QAbstract3DGraph::SelectionItemRowAndColumn
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->addItem(QStringLiteral("Multi, Slice: Row, Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->addItem(QStringLiteral("Multi, Slice: Col, Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->setCurrentIndex(1);
    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);
    QCheckBox *reverseValueAxisCheckBox = new QCheckBox(widget);
    reverseValueAxisCheckBox->setText(QStringLiteral("Reverse value axis"));
    reverseValueAxisCheckBox->setChecked(false);
    QSlider *rotationSliderX = new QSlider(Qt::Horizontal, widget);
    rotationSliderX->setTickInterval(30);
    rotationSliderX->setTickPosition(QSlider::TicksBelow);
    rotationSliderX->setMinimum(-180);
    rotationSliderX->setValue(0);
    rotationSliderX->setMaximum(180);
    QSlider *rotationSliderY = new QSlider(Qt::Horizontal, widget);
    rotationSliderY->setTickInterval(15);
    rotationSliderY->setTickPosition(QSlider::TicksAbove);
    rotationSliderY->setMinimum(-90);
    rotationSliderY->setValue(0);
    rotationSliderY->setMaximum(90);

    // Initializes + sets parameters for Mode buttons
    QRadioButton *staticMode = new QRadioButton(widget);
    staticMode->setText("Static Mode");
    staticMode->setChecked(true);
    QRadioButton *animateMode = new QRadioButton(widget);
    animateMode->setText("Animation Mode");
    QRadioButton *buildMode = new QRadioButton(widget);
    buildMode->setText("Build Mode");

    // Initializes + sets parameters for "Animate Speed" slider
    QSlider *refreshAnimateSpeedSlider = new QSlider(Qt::Horizontal, widget);
    refreshAnimateSpeedSlider->setTickInterval(1);
    refreshAnimateSpeedSlider->setTickPosition(QSlider::TicksBelow);
    refreshAnimateSpeedSlider->setMinimum(1);
    refreshAnimateSpeedSlider->setMaximum(500);

    // Initializes + sets parameters for "Build Speed" slider
    QSlider *refreshBuildSpeedSlider = new QSlider(Qt::Horizontal, widget);
    refreshBuildSpeedSlider->setTickInterval(1);
    refreshBuildSpeedSlider->setTickPosition(QSlider::TicksBelow);
    refreshBuildSpeedSlider->setMinimum(1);
    refreshBuildSpeedSlider->setMaximum(400);

    // Initializes + sets parameters for "Bar Thickness" slider
    QSlider *thicknessSlider = new QSlider(Qt::Horizontal, widget);
    thicknessSlider->setTickInterval(1);
    thicknessSlider->setTickPosition(QSlider::TicksBelow);
    thicknessSlider->setMinimum(1);
    thicknessSlider->setValue(38);
    thicknessSlider->setMaximum(40);

    // Initializes + sets parameters for "Frame Buffer" slider
    QSlider *frameBufferSlider = new QSlider(Qt::Horizontal, widget);
    frameBufferSlider->setTickInterval(1);
    frameBufferSlider->setTickPosition(QSlider::TicksBelow);
    frameBufferSlider->setMinimum(1);
    frameBufferSlider->setValue(1);
    frameBufferSlider->setMaximum(500);

    // Found from https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html
    QCheckBox *axisTitlesVisibleCB = new QCheckBox(widget);
    axisTitlesVisibleCB->setText(QStringLiteral("Axis titles visible"));
    axisTitlesVisibleCB->setChecked(true);
    QSlider *axisLabelRotationSlider = new QSlider(Qt::Horizontal, widget);
    axisLabelRotationSlider->setTickInterval(10);
    axisLabelRotationSlider->setTickPosition(QSlider::TicksBelow);
    axisLabelRotationSlider->setMinimum(0);
    axisLabelRotationSlider->setValue(30);
    axisLabelRotationSlider->setMaximum(90);


    // Loads widgets into user interface
    vLayout->addWidget(new QLabel(QStringLiteral("Bar Thickness")));
    vLayout->addWidget(thicknessSlider, 1, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Render Modes")));
    vLayout->addWidget(staticMode);
    vLayout->addWidget(animateMode);
    vLayout->addWidget(new QLabel(QStringLiteral("Animate Speed")));
    vLayout->addWidget(refreshAnimateSpeedSlider, 1, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Buffer Length (will reduce speed)")));
    vLayout->addWidget(frameBufferSlider, 1, Qt::AlignTop);
    vLayout->addWidget(buildMode);
    vLayout->addWidget(new QLabel(QStringLiteral("Build Speed")));
    vLayout->addWidget(refreshBuildSpeedSlider, 1, Qt::AlignTop);

    // Found from https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html
    vLayout->addWidget(new QLabel(QStringLiteral("Rotate horizontally")));
    vLayout->addWidget(rotationSliderX, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Rotate vertically")));
    vLayout->addWidget(rotationSliderY, 0, Qt::AlignTop);
    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(zoomToSelectedButton, 0, Qt::AlignTop);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(smoothCheckBox);
    vLayout->addWidget(reverseValueAxisCheckBox);
    vLayout->addWidget(axisTitlesVisibleCB);
    vLayout->addWidget(new QLabel(QStringLiteral("Change selection mode")));
    vLayout->addWidget(selectionModeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Axis label rotation")));
    vLayout->addWidget(axisLabelRotationSlider, 1, Qt::AlignTop);

    GraphModifier *modifier = new GraphModifier(widgetgraph);

    // Connects "Static Mode" button to relevant functions
    QObject::connect(staticMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::stopAnimate);
    QObject::connect(staticMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::stopBuild);
    QObject::connect(staticMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::staticBuild);

    // Connects "Animate Mode" button to relevant functions
    QObject::connect(animateMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::stopBuild);
    QObject::connect(animateMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::startAnimate);

    // Connects "Build Mode" button to relevant functions
    QObject::connect(buildMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::stopAnimate);
    QObject::connect(buildMode, &QRadioButton::clicked, modifier,
                      &GraphModifier::startBuild);

    // Connects "Speed" sliders to relevant functions
    QObject::connect(refreshAnimateSpeedSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeAnimateSpeed);
    QObject::connect(refreshBuildSpeedSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeBuildSpeed);

    // Connects "Bar Thickness" slider to relevant function
    QObject::connect(thicknessSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeThickness);

    // Connects "Frame Buffer" slider to relevant function
    QObject::connect(frameBufferSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeFrameBuffer);

    // Connects other objects to their relevant functions
    // Found from https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html
    QObject::connect(rotationSliderX, &QSlider::valueChanged, modifier, &GraphModifier::rotateX);
    QObject::connect(rotationSliderY, &QSlider::valueChanged, modifier, &GraphModifier::rotateY);
    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
                     &GraphModifier::changePresetCamera);
    QObject::connect(zoomToSelectedButton, &QPushButton::clicked, modifier,
                     &GraphModifier::zoomToSelectedBar);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setGridEnabled);
    QObject::connect(smoothCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setSmoothBars);
    QObject::connect(reverseValueAxisCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setReverseValueAxis);
    QObject::connect(modifier, &GraphModifier::gridEnabledChanged,
                     gridCheckBox, &QCheckBox::setChecked);
    QObject::connect(axisTitlesVisibleCB, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setAxisTitleVisibility);
    QObject::connect(axisLabelRotationSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeLabelRotation);
    widget->show();
    return app.exec();
}
