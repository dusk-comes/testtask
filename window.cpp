#include "window.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QtCharts/QChartView>

Window::Window(QWidget *parent) :
    QWidget(parent),
    oper(new Processing()),
    form(new Form()),
    chart(new Chart),
    startButton(new QPushButton("Start", this)),
    pauseButton(new QPushButton("Pause", this)),
    breakButton(new QPushButton("Break", this))
{
    setupUi();

    /*
     * вся логика реализованна с помощью
     * signals & slots.
     * действия по кликам реализованы через
     * приватные слоты.
     */
    connect(startButton, &QPushButton::clicked,
            this, &Window::clickOnStart);

    connect(pauseButton, &QPushButton::clicked,
            this, &Window::clickOnPause);

    connect(breakButton, &QPushButton::clicked,
            this, &Window::clickOnBreak);

    connect(form, &Form::inputIsOk,
            oper, &Processing::processData);

    connect(form, &Form::inputIsOk,
            this, &Window::blockInput);

    connect(oper, &Processing::sendPoint,
            chart, &Chart::drawPoint);

    connect(oper, &Processing::tickTack,
            this, &Window::showPercentage);

    connect(oper, &Processing::error,
            this, &Window::showError);
}

void Window::clickOnStart()
{
    /*
     * кнопка потенциально может запустить
     * вычисления. ввод данных заблокируется
     * вместе с началом вычислений после
     * валидации значений в class Form.
     */
    QString statusOfButton = startButton->text();
    if (statusOfButton == "Start")
    {
        emit form->checkInput();
    }
    else if (statusOfButton == "New")
    {
        startButton->setText("Start");

        /*
         * обновим график перед новой итерацией
         */
        chart->flush();

        /*
         * остаётся разблокировать только Форму
         * ввода данных
         */
        form->setDisabled(false);
    }
}

void Window::clickOnPause()
{
    /*
     * должна тригерить вычисления только
     * в состояних PAUSE или PROCESS
     */
    State state = oper->state();
    switch(state)
    {
        case State::PROCESS:
            pauseButton->setText("Continue");
            break;

        case State::PAUSE:
            pauseButton->setText("Pause");
            break;

        default: break;
    }
    oper->pauseExe();
}

void Window::clickOnBreak()
{
    /*
     * в любом состоянии должна быть возможность
     * прервать текущие вычисления И(ИЛИ)
     * разблокировать кнопку старт
     */
    oper->stopExe();
    startButton->setText("New");
    startButton->setDisabled(false);
}

void Window::showPercentage(int p)
{
    startButton->setText(
                QString("Progres %1%").arg(p));
}

void Window::showError(QString e)
{
    QMessageBox::critical(this, "error", e);
}

void Window::blockInput()
{
    startButton->setDisabled(true);
    form->setDisabled(true);
}

void Window::setupUi()
{
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    form->setSizePolicy(QSizePolicy::Fixed,
                        QSizePolicy::Fixed);

    QHBoxLayout *buttomLayout = new QHBoxLayout();
    buttomLayout->addWidget(startButton);
    buttomLayout->addWidget(pauseButton);
    buttomLayout->addWidget(breakButton);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(form);
    formLayout->addRow(buttomLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(chartView);

    setLayout(mainLayout);
}
