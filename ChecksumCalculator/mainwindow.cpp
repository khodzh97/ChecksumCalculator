#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getFileToCalculate(QString path)
{
    ui->fileName->setText(path);
    filename = path;
    on_calcButton_clicked();
}

void MainWindow::getCountedHash(QString result)
{
    ui->progressBar->setValue(100);
    ui->checksum->setText(result);
    setEnableControls(true);
}

void MainWindow::getProgress(int percent)
{
    ui->progressBar->setValue(percent);
}

void MainWindow::getCompareResult(bool result)
{
    QMessageBox msgbox;
    msgbox.setWindowTitle("Результат");
    if(result)
    {
        msgbox.setText("Файлы идентичны");
    }
    else
    {
        msgbox.setText("Файлы разные");
    }
    msgbox.exec();
    setEnableControls(true);
}

void MainWindow::on_browseButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this);
    ui->fileName->setText(filename);
}

void MainWindow::on_calcButton_clicked()
{
    if(ui->fileName->text() != "")
    {
        setEnableControls(false);
        if(ui->comboBox->currentText() == "CRC32")
            emit calculateCRC32(filename);
        else
            emit calculateHash(filename, currentAlgorithm);
        ui->checksum->clear();
        ui->progressBar->setValue(0);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:{
        currentAlgorithm = QCryptographicHash::Md5;
        break;
    }
    case 1:{
        currentAlgorithm = QCryptographicHash::Sha1;
        break;
    }
    case 2:{
        currentAlgorithm = QCryptographicHash::Sha256;
        break;
    }
    case 3:{
        currentAlgorithm = QCryptographicHash::Sha512;
        break;
    }
    case 4:{
        currentAlgorithm = QCryptographicHash::Keccak_256;
        break;
    }
    case 5:{
        currentAlgorithm = QCryptographicHash::Keccak_512;
        break;
    }
    default: break;
    }
}

void MainWindow::on_compareButton_clicked()
{
    setEnableControls(false);
    QString fileToCompare = QFileDialog::getOpenFileName(this);
    if(ui->comboBox->currentText() == "CRC32")
        emit compareFilesWithCRC32(filename, fileToCompare);
    else
        emit compareFiles(filename, fileToCompare, currentAlgorithm);
}

void MainWindow::setEnableControls(bool state)
{
    ui->browseButton->setEnabled(state);
    ui->comboBox->setEnabled(state);
    ui->calcButton->setEnabled(state);
    ui->compareButton->setEnabled(state);
}
