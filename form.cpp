#include "form.h"
#include <QDoubleValidator>
#include <QStringList>
#include <QGridLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

Form::Form(QWidget *parent) :
      QWidget(parent),
      aLine(new QLineEdit()),
      bLine(new QLineEdit()),
      cLine(new QLineEdit()),
      fromLine(new QLineEdit()),
      toLine(new QLineEdit()),
      stepLine(new QLineEdit()),
      funcComboBox(new QComboBox())
{
    setupUi();

    lines = findChildren<QLineEdit *>();

    setupInputValidator();

    connect(funcComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Form::dynamicBlockInput);

}

void Form::setDisabled(bool b)
{
    foreach (auto *line, lines)
    {
        line->setDisabled(b);
    }
    funcComboBox->setDisabled(b);
}

void Form::dynamicBlockInput(int func)
{
   switch(func)
   {
     case 2: case 3:
         cLine->setDisabled(true);
         break;
     default:
         cLine->setDisabled(false);
         break;
   }
}

void Form::checkInput()
{
    /*
     * данные не должны быть пустыми
     * иначе сообщим об этом юзеру
     * и закончим проверку
     */
    foreach (auto *line, lines)
    {
        if (line->isEnabled() &&
            line->text().isEmpty())
        {
            QMessageBox::information(this, "info",
                           "Please, provide data");
            return;
        }
    }

    QMap<QString, double> data;
    data["a"] = aLine->text().toDouble();
    data["b"] = bLine->text().toDouble();
    data["c"] = cLine->text().toDouble();
    data["from"] = fromLine->text().toDouble();
    data["to"] = toLine->text().toDouble();
    data["step"] = stepLine->text().toDouble();

    /*
     * если нет логических ошибок в диапазоне
     * то данные можно отправлять дальше
     */
    if (boundsIsOk(data))
    {
        emit inputIsOk(data,
                       funcComboBox->currentIndex());
    }
}

void Form::setupInputValidator()
{
    /*
     * выставим для теста небольшой диапазон
     * допустимых значений double.
     * расширить диапазон до предела double
     * можно будет только после реализации
     * соответсующих проверок в class Processing
     */
    double minNeg = -99.;
    double maxPos = 99.;
    double minPos = 0.;
    int decimals = 2;
    QDoubleValidator *wideVld = new QDoubleValidator(
                minNeg, maxPos, decimals, this);
    QDoubleValidator *narrowVld = new QDoubleValidator(
                minPos, maxPos, decimals, this);

    aLine->setValidator(wideVld);
    bLine->setValidator(wideVld);
    cLine->setValidator(wideVld);
    fromLine->setValidator(wideVld);
    toLine->setValidator(wideVld);
    stepLine->setValidator(narrowVld);

    int length = 5;
    aLine->setMaxLength(length);
    bLine->setMaxLength(length);
    cLine->setMaxLength(length);
    fromLine->setMaxLength(length);
    toLine->setMaxLength(length);
    stepLine->setMaxLength(length);
}

QLabel* Form::createLabel(const QString& name)
{
    QLabel *label = new QLabel(name);
    label->setAlignment(Qt::AlignRight);
    return label;
}

bool Form::boundsIsOk(const QMap<QString, double>
                         &data)
{
    if (data["from"] > data["to"])
    {
        QMessageBox::information(this, "info",
                         "From is greater than To");
        return false;
    }
    else if (data["from"] == data["to"])
    {
        QMessageBox::information(this, "info",
                         "From is equal to To");
        return false;
    }

    if (data["to"] - data["from"]
                < data["step"])
    {
        QMessageBox::information(this, "info",
                         "Step is too big");
        return false;
    }

    if (data["step"] == 0)
    {
        QMessageBox::information(this, "info",
                         "Step cannot be zero");
        return false;
    }

    return true;
}
void Form::setupUi()
{
    funcComboBox->insertItems(0, QStringList(
            {"f(x) = A*(x*x) + B*x + C",
            "f(x) = A*sin(x) + B*cos(C*x)",
            "f(x) = A*log(B*x)",
            "f(x) = A / (sin(x*x) * B)"}));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(funcComboBox, 0, 0, 1, 4);
    layout->addWidget(createLabel("A="), 1, 0);
    layout->addWidget(createLabel("B="), 2, 0);
    layout->addWidget(createLabel("C="), 3, 0);
    layout->addWidget(aLine, 1, 1);
    layout->addWidget(bLine, 2, 1);
    layout->addWidget(cLine, 3, 1);
    layout->addWidget(createLabel("From"), 1, 2);
    layout->addWidget(createLabel("To"), 2, 2);
    layout->addWidget(createLabel("Step"), 3, 2);
    layout->addWidget(fromLine, 1, 3);
    layout->addWidget(toLine, 2, 3);
    layout->addWidget(stepLine, 3, 3);
    setLayout(layout);
}
