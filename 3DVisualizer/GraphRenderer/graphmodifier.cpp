#include "graphmodifier.h"
#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/QTime>
#include <QtWidgets/QComboBox>
#include <QtCore/qmath.h>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <QEventLoop>
#include <QTime>
#include <QTimer>
#include <QDir>
#include <string>
#include <linux/limits.h>
#include <unistd.h>

//reference: https://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
//std::string getexepath()
//{
//  char result[ PATH_MAX ];
//  ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
//  return std::string( result, (count > 0) ? count : 0 );
//}

using namespace QtDataVisualization;

float tempFloats[6304][74];

GraphModifier::GraphModifier(Q3DBars *bargraph)
    : m_graph(bargraph),
      m_xRotation(0.0f),
      m_yRotation(0.0f),
      m_fontSize(1),
      m_segments(4),
      m_subSegments(3),
      m_minval(0.0f),
      m_maxval(200.0f),
      m_magnitudeAxis(new QValue3DAxis),
      m_timeAxis(new QCategory3DAxis),
      m_freqAxis(new QCategory3DAxis),
      m_primarySeries(new QBar3DSeries),
      m_barMesh(QAbstract3DSeries::MeshBevelBar),
      m_smooth(false)
{
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftMedium);
    m_graph->activeTheme()->setBackgroundEnabled(false);
    m_graph->activeTheme()->setFont(QFont("Times New Roman", m_fontSize));
    m_graph->activeTheme()->setLabelBackgroundEnabled(true);
    m_graph->setMultiSeriesUniform(true);

    // Adjust spaces between bars
    QSizeF barSize(0,0);
    m_graph->setBarSpacing(barSize);
    m_graph->setBarThickness(10);
    QDir::current();
    std::ifstream fin("/home/matt/Desktop/Link to Courses/CMPUT 275/Assignments/Final Project 2/CMPUT275_Final_Project/3DVisualizer/FileDecoder/output.txt");
    std::string line;
    getline(fin, line);
    timeIndexLen = std::stoi(line);
    getline(fin, line);
    freqIndexLen = std::stoi(line);

    int at, freqIndex,timeIndex;
    for (timeIndex=0; timeIndex<timeIndexLen; timeIndex++)
    {
        getline(fin, line);
        if (line == "NEXT_TIME_INTERVAL")
        {
//            std::cout << "line: " << line << std::endl;
            getline(fin, line);
//            std::cout << "line: " << line << std::endl;
            QString time = QString::fromStdString(line);
            m_timeBuckets << time;
        }
        for (freqIndex=0; freqIndex<freqIndexLen; freqIndex++)
        {
            getline(fin, line);
            std::string p[2];
            at = 0;
            for (auto c : line)
            {
                if (c == ':')
                {
                    QString temp = QString::fromStdString(p[0]);
                    ++at;
                }
                else
                {
                    p[at] += c;
                }
            }
            tempFloats[timeIndex][freqIndex] = std::stof(p[1]);
        }
        timeIndex += 1;

    }
    //Maps frequency searches to musical notes
    m_freqBuckets << "A0" << "A#0" << "B0" << "C0" << "C#0" << "D0" << "D#0" << "E0" << "F0" << "F#0" << "G0"<< "G#0"
                  << "A1" << "A#1" << "B1" << "C1" << "C#1" << "D1" << "D#1" << "E1" << "F1" << "F#1" << "G1"<< "G#1"
                  << "A2" << "A#2" << "B2" << "C2" << "C#2" << "D2" << "D#2" << "E2" << "F2" << "F#2" << "G2"<< "G#2"
                  << "A3" << "A#3" << "B3" << "C3" << "C#3" << "D3" << "D#3" << "E3" << "F3" << "F#3" << "G3"<< "G#3"
                  << "A4" << "A#4" << "B4" << "C4" << "C#4" << "D4" << "D#4" << "E4" << "F4" << "F#4" << "G4"<< "G#4"
                  << "A5" << "A#5" << "B5" << "C5" << "C#5" << "D5" << "D#5" << "E5" << "F5" << "F#5" << "G5"<< "G#5"
                  << "A6" << "A#6" << "B6" << "C6" << "C#6" << "D6" << "D#6" << "E6" << "F6" << "F#6" << "G6"<< "G#6";

    m_magnitudeAxis->setTitle("Magnitude");
//    std::cout << "getexepath(): " << getexepath() << std::endl;
    m_magnitudeAxis->setSegmentCount(m_segments);
    m_magnitudeAxis->setSubSegmentCount(m_subSegments);
    m_magnitudeAxis->setRange(m_minval, m_maxval);
    m_magnitudeAxis->setLabelFormat(QString(QStringLiteral("%.1f ")));
    m_magnitudeAxis->setLabelAutoRotation(30.0f);
    m_magnitudeAxis->setTitleVisible(true);

    m_timeAxis->setTitle("Time");
    m_timeAxis->setLabelAutoRotation(30.0f);
    m_timeAxis->setTitleVisible(true);
    m_freqAxis->setTitle("Frequency");
    m_freqAxis->setLabelAutoRotation(30.0f);
    m_freqAxis->setTitleVisible(true);

    m_graph->setValueAxis(m_magnitudeAxis);
    m_graph->setRowAxis(m_timeAxis);
    m_graph->setColumnAxis(m_freqAxis);

    m_primarySeries->setItemLabelFormat(QStringLiteral("Oulu - @colLabel @rowLabel: @valueLabel"));
    m_primarySeries->setMesh(QAbstract3DSeries::MeshBevelBar);
    m_primarySeries->setMeshSmooth(false);

    m_graph->addSeries(m_primarySeries);

    changePresetCamera();

    resetTemperatureData();

    // Set up property animations for zooming to the selected bar
    Q3DCamera *camera = m_graph->scene()->activeCamera();
    m_defaultAngleX = camera->xRotation();
    m_defaultAngleY = camera->yRotation();
    m_defaultZoom = camera->zoomLevel();
    m_defaultTarget = camera->target();

    m_animationCameraX.setTargetObject(camera);
    m_animationCameraY.setTargetObject(camera);
    m_animationCameraZoom.setTargetObject(camera);
    m_animationCameraTarget.setTargetObject(camera);

    m_animationCameraX.setPropertyName("xRotation");
    m_animationCameraY.setPropertyName("yRotation");
    m_animationCameraZoom.setPropertyName("zoomLevel");
    m_animationCameraTarget.setPropertyName("target");

    int duration = 1700;
    m_animationCameraX.setDuration(duration);
    m_animationCameraY.setDuration(duration);
    m_animationCameraZoom.setDuration(duration);
    m_animationCameraTarget.setDuration(duration);

    // The zoom always first zooms out above the graph and then zooms in
    qreal zoomOutFraction = 0.3;
    m_animationCameraX.setKeyValueAt(zoomOutFraction, QVariant::fromValue(0.0f));
    m_animationCameraY.setKeyValueAt(zoomOutFraction, QVariant::fromValue(90.0f));
    m_animationCameraZoom.setKeyValueAt(zoomOutFraction, QVariant::fromValue(50.0f));
    m_animationCameraTarget.setKeyValueAt(zoomOutFraction,
                                          QVariant::fromValue(QVector3D(0.0f, 0.0f, 0.0f)));

}

GraphModifier::~GraphModifier()
{
    delete m_graph;
}

void GraphModifier::resetTemperatureData()
{
    // Set up data

    // Create data arrays
    QBarDataArray *dataSet = new QBarDataArray;
    QBarDataRow *dataRow;

    dataSet->reserve(m_timeBuckets.size());
    for (int time = 0; time < m_timeBuckets.size(); time++) {
        // Create a data row
        dataRow = new QBarDataRow(m_freqBuckets.size());
        for (int freq = 0; freq < m_freqBuckets.size(); freq++) {
            // Add data to the row
            (*dataRow)[freq].setValue(tempFloats[time][freq]);
        }
        // Add the row to the set
        dataSet->append(dataRow);
    }

    // Add data to the data proxy (the data proxy assumes ownership of it)
    m_primarySeries->dataProxy()->resetArray(dataSet, m_timeBuckets, m_freqBuckets);
//    m_secondarySeries->dataProxy()->resetArray(dataSet2, m_timeBuckets, m_freqBuckets);
}

void GraphModifier::changeRange(int range)
{
    std::cout << "Changing range= " << range << std::endl;
    if (range >= m_timeBuckets.count())
    {
        std::cout << "timebuckets.count: " << m_timeBuckets.count() << std::endl;
        m_timeAxis->setRange(0, m_timeBuckets.count() - 1);
    }
    else
        m_timeAxis->setRange(range, range);
}

//####mymethods
void GraphModifier::startAnimate(QTimer *timer)
{
    timer->start(46);
}

int frame = 0;
void GraphModifier::animate()
{
    if (frame >=6303)
        frame=0;
    frame += 1;
    m_timeAxis->setRange(frame, frame+1);
}

void GraphModifier::build()
{
    if (frame >=6303)
        frame=0;
    frame += 1;
    m_timeAxis->setRange(0, frame+1);
}

void GraphModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_barMesh = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        m_primarySeries->setMesh(m_barMesh);
    }
}

void GraphModifier::changePresetCamera()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    // Restore camera target in case animation has changed it
    m_graph->scene()->activeCamera()->setTarget(QVector3D(0.0f, 0.0f, 0.0f));

    static int preset = Q3DCamera::CameraPresetFront;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void GraphModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
    emit fontSizeChanged(currentTheme->font().pointSize());
}

void GraphModifier::changeLabelBackground()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(!m_graph->activeTheme()->isLabelBackgroundEnabled());
}

void GraphModifier::changeSelectionMode(int selectionMode)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        int flags = comboBox->itemData(selectionMode).toInt();
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionFlags(flags));
    }
}

void GraphModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    m_graph->activeTheme()->setFont(newFont);
}

void GraphModifier::changeFontSize(int fontsize)
{
    m_fontSize = fontsize;
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    std::cout << "fontsize: " << m_fontSize << std::endl;
}

void GraphModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    // Updates the UI component to show correct shadow quality
    emit shadowQualityChanged(quality);
}

void GraphModifier::changeLabelRotation(int rotation)
{
    m_magnitudeAxis->setLabelAutoRotation(float(rotation));
    m_freqAxis->setLabelAutoRotation(float(rotation));
    m_timeAxis->setLabelAutoRotation(float(rotation));
}

void GraphModifier::setAxisTitleVisibility(bool enabled)
{
    m_magnitudeAxis->setTitleVisible(enabled);
    m_freqAxis->setTitleVisible(enabled);
    m_timeAxis->setTitleVisible(enabled);
}

void GraphModifier::setAxisTitleFixed(bool enabled)
{
    m_magnitudeAxis->setTitleFixed(enabled);
    m_freqAxis->setTitleFixed(enabled);
    m_timeAxis->setTitleFixed(enabled);
}

void GraphModifier::zoomToSelectedBar()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    Q3DCamera *camera = m_graph->scene()->activeCamera();
    float currentX = camera->xRotation();
    float currentY = camera->yRotation();
    float currentZoom = camera->zoomLevel();
    QVector3D currentTarget = camera->target();

    m_animationCameraX.setStartValue(QVariant::fromValue(currentX));
    m_animationCameraY.setStartValue(QVariant::fromValue(currentY));
    m_animationCameraZoom.setStartValue(QVariant::fromValue(currentZoom));
    m_animationCameraTarget.setStartValue(QVariant::fromValue(currentTarget));

    QPoint selectedBar = m_graph->selectedSeries()
            ? m_graph->selectedSeries()->selectedBar()
            : QBar3DSeries::invalidSelectionPosition();

    if (selectedBar != QBar3DSeries::invalidSelectionPosition()) {
        // Normalize selected bar position within axis range to determine target coordinates
        QVector3D endTarget;
        float xMin = m_graph->columnAxis()->min();
        float xRange = m_graph->columnAxis()->max() - xMin;
        float zMin = m_graph->rowAxis()->min();
        float zRange = m_graph->rowAxis()->max() - zMin;
        endTarget.setX((selectedBar.y() - xMin) / xRange * 2.0f - 1.0f);
        endTarget.setZ((selectedBar.x() - zMin) / zRange * 2.0f - 1.0f);

        // Rotate the camera so that it always points approximately to the graph center
        qreal endAngleX = 90.0 - qRadiansToDegrees(qAtan(qreal(endTarget.z() / endTarget.x())));
        if (endTarget.x() > 0.0f)
            endAngleX -= 180.0f;
        float barValue = m_graph->selectedSeries()->dataProxy()->itemAt(selectedBar.x(),
                                                                        selectedBar.y())->value();
        float endAngleY = barValue >= 0.0f ? 30.0f : -30.0f;
        if (m_graph->valueAxis()->reversed())
            endAngleY *= -1.0f;

        m_animationCameraX.setEndValue(QVariant::fromValue(float(endAngleX)));
        m_animationCameraY.setEndValue(QVariant::fromValue(endAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(250));
        m_animationCameraTarget.setEndValue(QVariant::fromValue(endTarget));
    } else {
        // No selected bar, so return to the default view
        m_animationCameraX.setEndValue(QVariant::fromValue(m_defaultAngleX));
        m_animationCameraY.setEndValue(QVariant::fromValue(m_defaultAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(m_defaultZoom));
        m_animationCameraTarget.setEndValue(QVariant::fromValue(m_defaultTarget));
    }

    m_animationCameraX.start();
    m_animationCameraY.start();
    m_animationCameraZoom.start();
    m_animationCameraTarget.start();
}

void GraphModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
    emit shadowQualityChanged(quality);
}

void GraphModifier::rotateX(int rotation)
{
    m_xRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

void GraphModifier::rotateY(int rotation)
{
    m_yRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

void GraphModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled(bool(enabled));
}

void GraphModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled(bool(enabled));
}

void GraphModifier::setSmoothBars(int smooth)
{
    m_smooth = bool(smooth);
    m_primarySeries->setMeshSmooth(m_smooth);
//    m_secondarySeries->setMeshSmooth(m_smooth);
}

void GraphModifier::setSeriesVisibility(int enabled)
{
//    m_secondarySeries->setVisible(bool(enabled));
}

void GraphModifier::setReverseValueAxis(int enabled)
{
    m_graph->valueAxis()->setReversed(enabled);
}

void GraphModifier::setReflection(bool enabled)
{
    m_graph->setReflection(enabled);
}
