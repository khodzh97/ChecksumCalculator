#include "mainwindow.h"
#include "hashcalculator.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    HashCalculator hash;

    qRegisterMetaType<QCryptographicHash::Algorithm>();
    QThread *computingThread = new QThread;

    QObject::connect(&w, SIGNAL(calculateHash(QString, QCryptographicHash::Algorithm)), &hash, SLOT(getFileToCalculateHash(QString, QCryptographicHash::Algorithm)));
    QObject::connect(&w, SIGNAL(calculateCRC32(QString)), &hash, SLOT(getFileToCalculateCRC32(QString)));
    QObject::connect(&w, SIGNAL(compareFiles(QString, QString, QCryptographicHash::Algorithm)), &hash, SLOT(compareFiles(QString, QString, QCryptographicHash::Algorithm)));
    QObject::connect(&w, SIGNAL(compareFilesWithCRC32(QString, QString)), &hash, SLOT(compareFilesWithCRC32(QString, QString)));
    QObject::connect(&hash, SIGNAL(progress(int)), &w, SLOT(getProgress(int)));
    QObject::connect(&hash, SIGNAL(result(QString)), &w, SLOT(getCountedHash(QString)));
    QObject::connect(&hash, SIGNAL(compareResult(bool)), &w, SLOT(getCompareResult(bool)));

    hash.moveToThread(computingThread);

    computingThread->start(QThread::TimeCriticalPriority);

    w.show();
    w.setWindowTitle("Калькулятор хеш-сумм");

    w.getFileToCalculate(argv[1]);

    return a.exec();
}

