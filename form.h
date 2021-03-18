#ifndef FORM_H
#define FORM_H
#include <QWidget>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
class QLabel;
QT_END_NAMESPACE

/*
 * класс служит для ввода данных через GUI
 * и первого этапа отсеивания данных
 */
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    void setDisabled(bool);

signals:
    void inputIsOk(QMap<QString, double>, int);

public slots:
    void checkInput();

private slots:
    void dynamicBlockInput(int);

private:
    QLineEdit *aLine;
    QLineEdit *bLine;
    QLineEdit *cLine;
    QLineEdit *fromLine;
    QLineEdit *toLine;
    QLineEdit *stepLine;
    QComboBox *funcComboBox;
    QList<QLineEdit *> lines;

    void setupInputValidator();
    QLabel * createLabel(const QString&);
    bool boundsIsOk(const QMap<QString, double> &);
    void setupUi();
};
#endif // FORM_H
