#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QPixmap>
#include <QByteArrayView>
#include <QImage>
#include <QImageWriter>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::importPressed);
    imageImported = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importPressed() {
    QString path = QFileDialog::getOpenFileName(this, "Open Jpeg", QDir::homePath());
    if(jpegEditor.verifyFile(path.toStdString())) {
        QImage image(path);
        ui->imageDisplay->setPixmap(QPixmap::fromImage(image));

        //tell jpegEditor the path of the image
        jpegEditor.importJpeg(path.toStdString());

        setHexEdit(jpegEditor.getQuantTableStr(0));

        ui->hexEditStatus->setText("Input Good");
        imageImported = true;

    } else if (path == "") {

    } else {
        QMessageBox::information(this, "Invaild File", "File Not JPEG!");
    }
}

void MainWindow::setHexEdit(std::string str) {
    ui->hexEdit->setPlainText(QString::fromStdString(str));
}

void MainWindow::on_back_clicked()
{
    std::string table = jpegEditor.getPrevTable();
    if(table != "") {
        setHexEdit(table);
    }
}

void MainWindow::on_next_clicked()
{
    std::string table = jpegEditor.getNextTable();
    if(table != "") {
        setHexEdit(table);
    }
}

void MainWindow::on_hexEdit_textChanged()
{
    if(imageImported) {
        bool result = jpegEditor.tableChanged(ui->hexEdit->toPlainText().toStdString());
        if(result) {
            ui->hexEditStatus->setText("Input Good");
        } else {
            ui->hexEditStatus->setText("Input Invalid!");
        }
    }
}

void MainWindow::on_save_clicked()
{
/*
    std::vector<unsigned char>imageBytes = jpegEditor.getImageBytesChar();

    unsigned char arr[imageBytes.size()];
    for(int i = 0; i < imageBytes.size(); ++i) {
        arr[i] = imageBytes[i];
    }

    QImage newImage;
    bool isLoaded = newImage.loadFromData(arr, imageBytes.size(), "JPEG");
    //QImageWriter qWriter(QDir::currentPath());
    //qWriter.write(newImage);
    ui->imageDisplay->setPixmap(QPixmap::fromImage(newImage));
*/

    jpegEditor.writeJpeg();
    QImage newImage;
    bool valid = newImage.load("test.jpg", "JPEG");
    ui->imageDisplay->setPixmap(QPixmap::fromImage(newImage));

}

