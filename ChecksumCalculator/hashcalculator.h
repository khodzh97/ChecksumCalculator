#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QDebug>

#define polynome 0x0218a7a392

class HashCalculator : public QObject
{
    Q_OBJECT
public:
    explicit HashCalculator(QObject *parent = nullptr);

public slots:

    void getFileToCalculateHash(QString filename, QCryptographicHash::Algorithm algorithm);

    void getFileToCalculateCRC32(QString filename);

    void compareFiles(QString file1, QString file2, QCryptographicHash::Algorithm algorithm);

    void compareFilesWithCRC32(QString file1, QString file2);

    QString countCRC32(QString filename);

signals:

    void progress(int percent);

    void result(QString hashResult);

    void compareResult(bool result);

private:


    unsigned long crc;
    unsigned long crc_table[256];
};

#endif // HASHCALCULATOR_H
