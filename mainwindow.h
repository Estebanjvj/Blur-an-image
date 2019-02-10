#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnAbrir_clicked();

    void on_btnAction_clicked();

private:
    Ui::MainWindow *ui;
    const QString DEFAULT_PATH = "C:/Users/zaory/Documents/Los dataset/C1T2";
    cv::Mat inMat, outMat;

    cv::Mat getBlurEffect(cv::Mat inMat, QSize qp);
    void showImageOnce(cv::Mat theMat, std::string nombre);
    void customIntegral();
};

#endif // MAINWINDOW_H
