#include "processing.h"
#include <QtMath>
#include <stdexcept>

Processing::Processing(QObject *parrent) :
    QObject(parrent)
{
    connect(&timer, &QTimer::timeout,
            this, &Processing::execute);
    timer.setInterval(100);
}

void Processing::pauseExe()
{
    if (current_x <= TO)
    {
        switch(STATE)
        {
            case State::PAUSE:
                STATE = State::PROCESS;
                timer.start();
                break;

            case State::PROCESS:
                STATE = State::PAUSE;
                timer.stop();
                break;

            default: break;
        }
    }
}

void Processing::stopExe()
{
    STATE = State::END;
    timer.stop();
}

State Processing::state()
{
    return STATE;
}

void Processing::processData(
        QMap<QString, double> inputs,
        int func)
{
    A = inputs["a"];
    B = inputs["b"];
    C = inputs["c"];
    FROM = inputs["from"];
    TO = inputs["to"];
    STEP = inputs["step"];
    FUNC = func;

    TOTAL = (TO - FROM) / STEP;
    current_x = FROM;

    COUNTER = 0;
    startExe();
}

void Processing::startExe()
{
    STATE = State::PROCESS;
    timer.start();
}

void Processing::execute()
{
    double y;
    switch(FUNC)
    {
         case 0:
            y = A * qPow(current_x, 2) + B * current_x + C;
            break;

         case 1:
             y = A * qSin(current_x) + B * qCos(C * current_x);
             break;

         case 2:
            try{ y = A * m_log(B * current_x); }
            catch (std::logic_error er)
            {
                stopExe();
                emit error(er.what());
                return;
            }
            break;

         case 3:
            try { y = division(A, qSin(pow(current_x, 2)) * B); }
            catch (std::logic_error er)
            {
                stopExe();
                emit error(er.what());
                return;
            }

            break;
    }

    emit sendPoint({current_x, y});
    emit tickTack((COUNTER / TOTAL) * 100);

    ++COUNTER;
    current_x += STEP;

    if (current_x > TO)
    {
        emit tickTack(100);
        stopExe();
    }
}

inline
double Processing::division(const double &lhs,
                            const double &rhs)
{
    if (rhs == 0)
        throw std::logic_error("zero devision");
    return lhs / rhs;
}

inline
double Processing::m_log(const double &exp)
{
    if (exp <= 0)
        throw std::logic_error(
                    "negative or zero exponenta");
    return qLn(exp);
}
