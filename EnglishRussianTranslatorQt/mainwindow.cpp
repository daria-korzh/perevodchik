#include "mainwindow.h"
#include <QVBoxLayout>
#include <QToolTip>
#include <QCursor>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    input = new QLineEdit();
    input->setPlaceholderText("Введите английское предложение...");
    result = new QLabel("Перевод появится здесь");

    layout->addWidget(input);
    layout->addWidget(result);
    setCentralWidget(central);

    loadDictionary();

    connect(input, &QLineEdit::textChanged,
            this, &MainWindow::textChanged);

    resize(500,150);
    setWindowTitle("Англо-русский словарь с подсказками");
}

void MainWindow::loadDictionary()
{
    QFile file("translate.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Не удалось открыть файл translate.txt";
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.trimmed().isEmpty())
            continue;

        QStringList parts = line.split(" ");

        if (parts.size() >= 2)
        {
            QString eng = parts[0].toLower();
            QString rus= parts.mid(1).join(" ");

            eng = eng.trimmed();
            rus = rus.trimmed();

            dictionary[eng]=rus;

            qDebug() << "Загружено:" << eng << "->" << rus;
        }
    }

    file.close();
    qDebug() << "Всего загружено слов:" << dictionary.size();
}

void MainWindow::textChanged(const QString &text)
{
    QString word = text.split(" ").last().toLower();
    if (word=="the"|| word=="a"|| word=="an")
    {
        QToolTip::hideText();
        return;
    }
    if(word.length()>=1)
    {
        QStringList variants;
        for(auto it = dictionary.begin(); it!=dictionary.end(); ++it)
        {
            if(it.key().startsWith(word))
                variants <<it.key()+ "->" + it.value();
        }
        if(!variants.isEmpty())
        {
            QToolTip::showText(
                QCursor::pos(),
            "Возможно: " + variants.join("\n")
                );
        }
    }

    QStringList words = text.split(" ", Qt::SkipEmptyParts);
    QString translation;
    for(QString w : words)
    {
        QString clean =w.toLower();
        if(dictionary.contains(clean)&& !dictionary[clean].isEmpty())
            translation += dictionary[clean]+ " ";
    }

    if(!translation.isEmpty())
        result->setText("Перевод: " + translation);
}