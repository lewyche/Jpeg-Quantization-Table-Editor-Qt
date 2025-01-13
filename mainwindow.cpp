#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QPixmap>

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
        jpegEditor.importJpeg(path.toStdString());
        setHexEdit(jpegEditor.getQuantTable(0));
        imageImported = true;
        ui->hexEditStatus->setText("Input Good");
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

}

