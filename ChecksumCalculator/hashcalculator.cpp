#include "hashcalculator.h"

HashCalculator::HashCalculator(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < 256; i++)
    {
        crc = i;
        for(int j = 0; j < 8; j++)
        {
            crc = crc & 1 ? (crc >> 1) ^ polynome : crc >> 1;
        }
        crc_table[i] = crc;
    }
}

void HashCalculator::getFileToCalculateHash(QString filename, QCryptographicHash::Algorithm algorithm)
{
    QFile f(filename);
    QCryptographicHash hash(algorithm);
    f.open(QFile::ReadOnly);
    double remainFileSize = f.size();
    while(!f.atEnd())
    {
        hash.addData(f.read(4096));
        remainFileSize -= 4096;
        emit progress((f.size() - remainFileSize) / f.size()  * 100);
    }
    emit result(hash.result().toHex());
    hash.reset();
    f.close();
}

void HashCalculator::getFileToCalculateCRC32(QString filename)
{
    emit result(countCRC32(filename));
}

void HashCalculator::compareFiles(QString file1, QString file2, QCryptographicHash::Algorithm algorithm)
{
    QFile f1(file1);
    QFile f2(file2);
    f1.open(QFile::ReadOnly);
    f2.open(QFile::ReadOnly);
    QCryptographicHash hash1(algorithm);
    QCryptographicHash hash2(algorithm);
    double remainFile1Size = f1.size();
    double remainFile2Size = f2.size();
    while(!f1.atEnd())
    {
        hash1.addData(f1.read(4096));
        remainFile1Size -= 4096;
        emit progress((f1.size() - remainFile1Size) / (f1.size() + f2.size())  * 100);
    }
    while(!f2.atEnd())
    {
        hash2.addData(f2.read(4096));
        remainFile2Size -= 4096;
        emit progress((f1.size() + f2.size() - remainFile2Size) / (f1.size() + f2.size())  * 100);
    }
    emit progress(100);
    f1.close();
    f2.close();
    if(hash1.result() == hash2.result())
        emit compareResult(true);
    else
        emit compareResult(false);
    hash1.reset();
    hash2.reset();
}

void HashCalculator::compareFilesWithCRC32(QString file1, QString file2)
{
    QFile f1(file1);
    QFile f2(file2);
    f1.open(QFile::ReadOnly);
    f2.open(QFile::ReadOnly);
    double remainFile1Size = f1.size();
    double remainFile2Size = f2.size();
    unsigned long crc1 = 0xFFFFFFFF;
    unsigned long crc2 = 0xFFFFFFFF;
    while(!f1.atEnd())
    {
        QByteArray data1 = f1.read(4096);
        remainFile1Size -= 4096;
        crc1 = crc_table[(crc ^ *data1) & 0xFF] ^ (crc >> 8);
        emit progress((f1.size() - remainFile1Size) / (f1.size() + f2.size())  * 100);
    }
    while(!f2.atEnd())
    {
        QByteArray data2 = f2.read(4096);
        remainFile2Size -= 4096;
        crc2 = crc_table[(crc ^ *data2) & 0xFF] ^ (crc >> 8);
        emit progress((f1.size() + f2.size() - remainFile2Size) / (f1.size() + f2.size())  * 100);
    }
    emit progress(100);
    f1.close();
    f2.close();
    if(crc1 == crc2)
        emit compareResult(true);
    else
        emit compareResult(false);
}

QString HashCalculator::countCRC32(QString filename)
{
    QFile f(filename);
    f.open(QFile::ReadOnly);
    crc = 0xFFFFFFFF;
    double remainFileSize = f.size();
    while (!f.atEnd())
    {
        QByteArray data = f.read(4096);
        remainFileSize -= 4096;
        crc = crc_table[(crc ^ *data) & 0xFF] ^ (crc >> 8);
        emit progress((f.size() - remainFileSize) / f.size()  * 100);
    }
    return QString::number(crc, 16);
}
