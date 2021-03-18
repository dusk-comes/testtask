#ifndef MYCHART_H
#define MYCHART_H
#include <QtCharts/QChart>
#include <QQueue>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

/*
 * класс для динамической отрисовки данных
 * по запросу очищает график от предыдущих
 * значений
 */
class Chart: public QChart
{
    Q_OBJECT

public:
    explicit Chart(QGraphicsItem *parent = nullptr,
                   Qt::WindowFlags wFlags = {});

    void flush();

public slots:
    void drawPoint(const QPointF &points);

private:
    QSplineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QQueue<QPointF> points;
};
#endif // MYCHART_H
