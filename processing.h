#ifndef PROCESSING_H
#define PROCESSING_H
#include <QString>
#include <QQueue>
#include <QMap>
#include <QPointF>
#include <QObject>
#include <QtCore/QTimer>

enum class State {PROCESS, PAUSE, END};

/*
 * класс для вычисления ординаты;
 * запоминает своё состояние;
 * останавливает вычисления в случае ошибки;
 * по таймауту отправляет данные для отрисовки
 * может быть использован для обработки
 * данных хранящихся в файловой системе.
 */
class Processing : public QObject
{
    Q_OBJECT

public:
    explicit Processing(QObject *parrent = nullptr);
    void pauseExe();
    void stopExe();
    State state();

public slots:
    void processData(QMap<QString, double>, int);

private slots:
    void execute();

signals:
    void sendPoint(QPointF);
    void tickTack(int);
    void error(QString);

private:
    QQueue<QPointF> points;
    QTimer timer;
    double A, B, C, FROM, TO, STEP, TOTAL, current_x;
    int COUNTER, FUNC;
    State STATE;

    void startExe();
    double division(const double&, const double&);
    double m_log(const double&);
};
#endif // PROCESSING_H
