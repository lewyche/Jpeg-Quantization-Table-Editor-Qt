#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editjpeg.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void importPressed();

private slots:
    void on_back_clicked();
    void on_next_clicked();

    void on_hexEdit_textChanged();

    void on_save_clicked();

private:
    void setHexEdit(std::string str);

    bool imageImported;

    editJpeg jpegEditor;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
