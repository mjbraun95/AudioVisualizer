/*
  Names:  Ang Li  Matthew Braun
  IDs:    1550746 1497171
  CMPUT 275, Winter 2019

  Project: audio visualizer
*/
#ifndef GRAPHMODIFIER_H
#define GRAPHMODIFIER_H

#include <QtDataVisualization/q3dbars.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>

#include <QtGui/QFont>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtCore/QPropertyAnimation>

using namespace QtDataVisualization;

// Found and modified from https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html
class GraphModifier : public QObject
{
    Q_OBJECT
public:
    explicit GraphModifier(Q3DBars *bargraph);
    ~GraphModifier();

    void resetData();
    void changePresetCamera();
    void rotateX(int rotation);
    void rotateY(int rotation);
    void setGridEnabled(int enabled);
    void setSmoothBars(int smooth);
    void setSeriesVisibility(int enabled);
    void setReverseValueAxis(int enabled);

public Q_SLOTS:
    void changeRange(int range);
    void startAnimate();
    void stopAnimate();
    void startBuild();
    void stopBuild();
    void staticBuild();
    void animate();
    void build();
    void changeLabelRotation(int rotation);
    void changeThickness(int thickness);
    void changeAnimateSpeed(int animateSpeed);
    void changeBuildSpeed(int buildSpeed);
    void changeFrameBuffer(int buffer);
    void setAxisTitleVisibility(bool enabled);
    void zoomToSelectedBar();

Q_SIGNALS:
    void gridEnabledChanged(bool enabled);

private:
//    ifstream fin(string filename);
    Q3DBars *m_graph;
    float m_xRotation;
    float m_yRotation;
    int m_fontSize;
    int m_segments;
    int m_subSegments;
    int timeIndexLen;
    int freqIndexLen;
    float m_minval;
    float m_maxval;
    QStringList m_freqBuckets;
    QStringList m_timeBuckets;
    QValue3DAxis *m_magnitudeAxis;
    QCategory3DAxis *m_timeAxis;
    QCategory3DAxis *m_freqAxis;
    QBar3DSeries *m_primarySeries;
    QAbstract3DSeries::Mesh m_barMesh;
    bool m_smooth;
    QPropertyAnimation m_animationCameraX;
    QPropertyAnimation m_animationCameraY;
    QPropertyAnimation m_animationCameraZoom;
    QPropertyAnimation m_animationCameraTarget;
    float m_defaultAngleX;
    float m_defaultAngleY;
    float m_defaultZoom;
    QVector3D m_defaultTarget;
};

#endif
