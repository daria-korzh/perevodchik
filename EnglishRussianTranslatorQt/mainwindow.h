#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void textChanged(const QString &text);

private:
    QLineEdit *input;
    QLabel *result;
    QMap<QString, QString> dictionary;

    void loadDictionary();
};

#endif