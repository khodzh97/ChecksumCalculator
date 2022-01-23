#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void getFileToCalculate(QString path);


public slots:

    void getCountedHash(QString result);

    void getProgress(int percent);

    void getCompareResult(bool result);

private slots:
    void on_browseButton_clicked();

    void on_calcButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_compareButton_clicked();

signals:

    void calculateHash(QString filename, QCryptographicHash::Algorithm algorithm);

    void calculateCRC32(QString filename);

    void compareFiles(QString file1, QString file2, QCryptographicHash::Algorithm algorithm);

    void compareFilesWithCRC32(QString file1, QString file2);

private:
    Ui::MainWindow *ui;
    QString filename = "";
    QCryptographicHash::Algorithm currentAlgorithm = QCryptographicHash::Md5;

    void setEnableControls(bool state);
};
#endif // MAINWINDOW_H
