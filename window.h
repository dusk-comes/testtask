#ifndef WINDOW_H
#define WINDOW_H
#include "chart.h"
#include "form.h"
#include "processing.h"
#include <QWidget>

QT_BEGIN_HEADER
class QPushButton;
QT_END_HEADER

/*
 * класс для реализации логики программы
 * связывает интерфейсы между различными
 * модулями программы
 */
class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void clickOnStart();
    void clickOnPause();
    void clickOnBreak();
    void showPercentage(int);
    void showError(QString);
    void blockInput();

private:
    Processing *oper;
    Form *form;
    Chart *chart;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *breakButton;

    void setupUi();
};

#endif // WINDOW_H
