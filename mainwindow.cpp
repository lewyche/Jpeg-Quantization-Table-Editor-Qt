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
    this->setWindowTitle("Jpeg Quantization Table Editor Qt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importPressed() {
    QString path = QFileDialog::getOpenFileName(this, "Open Jpeg", QDir::homePath());
    if(jpegEditor.verifyFile(path.toStdString())) {
        QImage image(path);
        if(image.isNull()) {
            QMessageBox::information(this, "Bad Image", "Something is wrong with the image!");
            return;
        }
        ui->imageDisplay->setPixmap(QPixmap::fromImage(image));

        //tell jpegEditor the path of the image
        jpegEditor.importJpeg(path.toStdString());

        setHexEdit(jpegEditor.getQuantTableStr(0));

        setColourSpace();

        ui->hexEditStatus->setText("Input Good");
        imageImported = true;

        ui->qualitySlider->setEnabled(true);

    } else if (path == "") {

    } else {
        QMessageBox::information(this, "Invaild File", "File Not JPEG!");
    }
}

void MainWindow::setHexEdit(std::string str) {
    ui->hexEdit->setPlainText(QString::fromStdString(str));
}

void MainWindow::setColourSpace() {
    QString currColourSpace = QString::fromStdString(jpegEditor.getColourSpaceText());
    QString colourSpaceText = "";
    if(currColourSpace != "other") {
        colourSpaceText += "Currently editing: " + currColourSpace + " table";
    }
    ui->colorSpace->setText(colourSpaceText);
}

void MainWindow::changeQuantTable(std::string table) {
    if(table != "") {
        setHexEdit(table);
        setColourSpace();
    }
}

void MainWindow::on_back_clicked()
{
    std::string table = jpegEditor.getPrevTable();
    changeQuantTable(table);
}

void MainWindow::on_next_clicked()
{
    std::string table = jpegEditor.getNextTable();
    changeQuantTable(table);
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

void MainWindow::saveImage() {
    jpegEditor.writeJpeg();
    QImage newImage;
    newImage.load("test.jpg", "JPEG");
    ui->imageDisplay->setPixmap(QPixmap::fromImage(newImage));
}

void MainWindow::on_save_clicked()
{
    saveImage();
}

void MainWindow::on_setQuality_clicked()
{
    if(imageImported) {
        int position = ui->qualitySlider->value();
        std::string newQuantString = jpegEditor.calculateQuantizationTable(position);
        setHexEdit(newQuantString);
        saveImage();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    if(imageImported) {
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save Jpeg"), "", "Images(*.jpg)");
        jpegEditor.writeJpeg(filePath.toStdString());
    } else {
        QMessageBox::information(this, "Image Not Imported", "No Image to save!");
    }

}

