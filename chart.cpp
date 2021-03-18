#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

Chart::Chart(QGraphicsItem *parent,
             Qt::WindowFlags wFlags) :
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    series(0),
    axisX(new QValueAxis()),
    axisY(new QValueAxis())
{

    series = new QSplineSeries(this);
    QPen red(Qt::red);
    red.setWidth(3);
    series->setPen(red);

    addSeries(series);

    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void Chart::flush()
{
    series->clear();
    axisX->setRange(0, 1);
    axisY->setRange(0, 1);
}

void Chart::drawPoint(const QPointF &point)
{
    if (point.x() > axisX->max())
    {
        axisX->setMax(point.x() * 2);
    }
    if (point.x() < axisX->min())
    {
        axisX->setMin(point.x() * 2);
    }
    if (point.y() > axisY->max())
    {
        axisY->setMax(point.y() * 2);
    }
    if (point.y() < axisY->min())
    {
        axisY->setMin(point.y() * 2);
    }
    series->append(point);
}

