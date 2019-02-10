
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAbrir_clicked()
{
    //abrir
    QString nameFileInput = QFileDialog::getOpenFileName(this,       //path QString
         tr("Open Image"),
         DEFAULT_PATH,
         tr("Image Files (*.png *.jpg *.hdr *.jpeg)"));

    if (nameFileInput == NULL)
        return;
    ui->lbName->setText(nameFileInput);
    string pathFileInput = nameFileInput.toUtf8().constData();  //same path but std string
    inMat = imread(pathFileInput);
}

void MainWindow::on_btnAction_clicked()
{
    Mat theMat = getBlurEffect(inMat, QSize(ui->horizontalSlider->value(),ui->verticalSlider->value()));
    showImageOnce(theMat, "Resultado");
    Mat integralMat;
    integral(theMat, integralMat);
    showImageOnce(integralMat, "Integral");
    customIntegral();
}

cv::Mat MainWindow::getBlurEffect(cv::Mat inMat, QSize qp)
{
    Mat3b iMat = (Mat3f) inMat;
    Mat3b oMat = iMat;

    int max = ((inMat.cols-qp.width())*(inMat.rows-qp.height()))-4;
    ui->progressBar->setMaximum(max);
    int p=0;

    for (int x = 0; x < iMat.rows-qp.width(); ++x) {
        for (int y = 0; y < iMat.cols-qp.height(); ++y) {
            Vec3f ssd_temp;
            Vec3b real;
            ssd_temp[0]=0; ssd_temp[1] =0; ssd_temp[2]=0;
            for (int w = 0; w < qp.width(); ++w) {
                for (int h = 0; h < qp.height(); ++h) {

                    Vec3f f_temp = iMat(x+w,y+h);

                    ssd_temp[0] += f_temp[0];
                    ssd_temp[1] += f_temp[1];
                    ssd_temp[2] += f_temp[2];

                }
            }
            real[0] = ssd_temp[0]/(qp.width()*qp.height());
            real[1] = ssd_temp[1]/(qp.width()*qp.height());
            real[2] = ssd_temp[2]/(qp.width()*qp.height());
            oMat(x,y)=real;
            //qDebug() << "X" << x << "Y" << y;
            ui->progressBar->setValue(p++);
        }
    }
    imwrite(DEFAULT_PATH.toStdString()+"blured.png", oMat);
    //oMat = oMat(Rect(0,0,oMat.cols-qp.width(),oMat.rows-qp.height()));
    return oMat;
}

void MainWindow::showImageOnce(Mat theMat, string nombre)
{
    cv::namedWindow(nombre);                                    // create image window named "My Image"
    cv::imshow(nombre, theMat);                                 // show the image on window
}

void MainWindow::customIntegral()
{
    Mat croppedImage = this->inMat;
    Mat integral=Mat::zeros(croppedImage.rows+1,croppedImage.cols+1,CV_8UC3);

    for(int x=1;x<integral.rows;x++){
        for(int y=1;y<integral.cols;y++){
            integral.at<Vec3b>(x,y)=croppedImage.at<Vec3b>(x-1,y-1);
        }
    }

    //showImageOnce(integral, "xdddd");
    croppedImage = integral;
    for(int x=1;x<integral.rows;x++){
        for(int y=1;y<integral.cols;y++){

          Vec3f t =integral.at<Vec3f>  (x ,y );

          Vec3b A =integral.at<Vec3b>(x  ,y  );
          Vec3b C =integral.at<Vec3b>(x-1,y  );
          Vec3b D =integral.at<Vec3b>(x  ,y-1);
          Vec3b B =integral.at<Vec3b>(x-1,y-1);

          t[0]=(A[0]+B[0]+C[0]+D[0]);
          t[1]=(A[1]+B[1]+C[1]+D[1]);
          t[2]=(A[2]+B[2]+C[2]+D[2]);

          //derivada de la imagen
          /*t[0]=(int)(c0[0]-c1[0]-c2[0]+c3[0]);
          t[1]=(int)(c0[1]-c1[1]-c2[1]+c3[1]);
          t[2]=(int)(c0[2]-c1[2]-c2[2]+c3[2]);*/

          croppedImage.at<Vec3b>(x,y) = t;
          //croppedImage.at<Vec3b>(x,y) = t;
          qDebug() <<"0("<<t[0]<<")1("<<t[1]<<")2("<<t[2]<<")";
        }
    }
    showImageOnce(croppedImage, "da wae");
    imwrite(DEFAULT_PATH.toStdString()+"integral.png", integral);
}
