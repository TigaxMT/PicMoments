/************************************************************************************************************
*    											                                                            *
*    PicYou - Is a simple program to take photos. Written on C++ and using Opencv library and QT Framework *
*											                                                                *
*    Copyright (C) 2017  Tiago Martins                        				                                *
*											                                                                *
*    This program is free software: you can redistribute it and/or modify		                            *
*    it under the terms of the GNU General Public License as published by                                   *
*    the Free Software Foundation, either version 3 of the License, or                                      *
*    (at your option) any later version. 					                                                *
*											                                                                *
*    This program is distributed in the hope that it will be useful,			                            *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of			                                *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			                                *
*    GNU General Public License for more details.					                                        *
*											                                                                *
*    You should have received a copy of the GNU General Public License			                            *
*    along with this program. If not, see <http://www.gnu.org/licenses/>.                                   *
*											                                                                *
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cap(0),
    timer(this)
{
    ui->setupUi(this);
    ui->menuBar->setStyleSheet("color: white");
    ui->stopRecBtn->setVisible(false);

    rng(12345);

    ext = ".avi";

    if(!cap.isOpened())
    {
        return;
    }

    // This vector store the type of image quality (JPEG quality from 0 to 100, 95 is the default value)

    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(95);

    // Choose the directory you want the photos/videos have saved.

    while(1)
    {
        picPath = QFileDialog::getExistingDirectory(this,"Directory to save your Pics/Vids",QDir::currentPath()).toStdString();

        if(!picPath.empty())
            break;
    }

    //when the timer times out the signal is emitted and the ontimeout() private slot is executed

    connect(&timer,SIGNAL(timeout()),this,SLOT(on_timeout()));

    //call the timer every 10 ms

    timer.start(10);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::capture()
{
    // must disconnect the pushButton signal, if not the program doesn't stop to take photos

    ui->picBtn->disconnect(ui->picBtn,&QPushButton::clicked,this,&MainWindow::capture);

    if(ui->actionCanny_Edges->isChecked())
    {
        cv::imwrite(intToString(),flpPic,compression_params);

        ui->statusBar->setStyleSheet("color: red");
        ui->statusBar->showMessage("Pic Captured",500);
    }
    else
        if(ui->actionColor_Contours->isChecked())
        {
            cv::imwrite(intToString(),flpPic,compression_params);

            ui->statusBar->setStyleSheet("color: white");
            ui->statusBar->showMessage("Pic Captured",500);
        }
        else
        {
            // Capture a frame to Mat Image

            cap >> pic;

            //Flip the frame

            cv::flip(pic,flpPic,1);

            // And save it in the path created on intToString function

            cv::imwrite(intToString(),flpPic,compression_params);

            // Show a message in the statusbar for 1/2 sec

            ui->statusBar->setStyleSheet("color: white");
            ui->statusBar->showMessage("Pic Captured",500);
        }

    pics++;
}

void MainWindow::record()
{
    if(!isRec)
    {
        ui->picBtn->disconnect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::record);
        ui->stopRecBtn->setVisible(true);
        ui->recBtn->setVisible(false);

        // Size of the frame
        cv::Size S = cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH),
                              (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT));

        // Use the MPEG-4.3 codec because he don't give problems on writing .avi file, like others codecs

        if(ui->actionCanny_Edges->isChecked())
            rec.open(intToStringRec(),CV_FOURCC('D','I','V','3'),15,S,false);
        else
            rec.open(intToStringRec(),CV_FOURCC('D','I','V','3'),15,S,true);


        if(!rec.isOpened())
            return;

        isRec=true;
    }

    rec.write(flpPic);
}

void MainWindow::stopRecord()
{
    ui->picBtn->disconnect(ui->stopRecBtn,&QPushButton::clicked,this,&MainWindow::stopRecord);
    ui->stopRecBtn->setVisible(false);
    ui->recBtn->setVisible(true);

    isRec = false;
    rec.release();
    recs++;
}

void MainWindow::about()
{
    if(!aboutExec)
    {
       aboutDlg = new About(this);

       aboutDlg->show();

       aboutExec = true;
    }

    ui->actionAbout_PicYou->disconnect(ui->actionAbout_PicYou,&QAction::triggered,this,&MainWindow::about);
}

void MainWindow::videoSettings()
{
    if(!vidDlgExec)
    {
       vidDlg = new VideoSettings(this);

       vidDlg->show();

       vidDlgExec = true;
    }

    switch(vidDlg->getExtVal())
    {
        case 0:
                ext = ".avi";
                break;
        case 1:
                ext = ".mov";
                break;
    }

    ui->actionVideo_Settings->disconnect(ui->actionVideo_Settings,&QAction::triggered,this,&MainWindow::videoSettings);
}

void MainWindow::on_timeout()
{
    // Start capture frames

    cap.read(pic);

    // Check if any effect are checked

    if(ui->actionCanny_Edges->isChecked())
        cannyEdge();
    else
        if(ui->actionColor_Contours->isChecked())
            colorContours();
        else
        {
            if(threshExec)
            {
                threshCtrl->close();

                threshExec = false;

                ui->menuBar->setStyleSheet("color: white");
            }

            noEffects();
        }

    if(isRec)
        record();

    if(ui->actionVideo_Settings->isChecked())
        if(vidDlg->dlgExecVal() == true)
            videoSettings();

    if(aboutDlg->dlgExecVal() == false)
        aboutExec = false;

    if(vidDlg->dlgExecVal() == false)
        vidDlgExec = false;

    // Signals and Slots

    connect(ui->picBtn,&QPushButton::clicked,this,&MainWindow::capture);
    connect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::record);
    connect(ui->stopRecBtn,&QPushButton::clicked,this,&MainWindow::stopRecord);
    connect(ui->actionCanny_Edges,&QAction::triggered,this,&MainWindow::cannyEdge);
    connect(ui->actionColor_Contours,&QAction::triggered,this,&MainWindow::colorContours);
    connect(ui->actionAbout_PicYou,&QAction::triggered,this,&MainWindow::about);
    connect(ui->actionVideo_Settings,&QAction::triggered,this,&MainWindow::videoSettings);
}

void MainWindow::noEffects()
{
    // Flip the frame

    cv::flip(pic,flpPic,1);

    showFrame(flpPic);
}

void MainWindow::cannyEdge()
{
    // Create and show QDialog to adjust the threshold value

    if(!threshExec)
    {
        threshCtrl = new Threshold(this);

        threshCtrl->setWindowTitle("Adjust Threshold");

        threshCtrl->show();

        threshExec = true;
    }

    threshold = threshCtrl->getSliderVal();

    ui->menuBar->setStyleSheet("color: red");
    ui->actionCanny_Edges->disconnect(ui->actionCanny_Edges,&QAction::triggered,this,&MainWindow::cannyEdge);

    // Start the algorithm to CannyEdge effect

    cv::Mat pic_gray;

    cv::flip(pic,flpPic,1);

    cv::cvtColor(flpPic,pic_gray,cv::COLOR_BGR2GRAY);

    flpPic.release();

    cv::blur(pic_gray,flpPic,cv::Size(3,3));

    cv::Canny(flpPic,flpPic,threshold,threshold*3,3);

    showFrame(flpPic);
}

void MainWindow::colorContours()
{
    if(!threshExec)
    {
        threshCtrl = new Threshold(this);

        threshCtrl->setWindowTitle("Adjust Threshold");

        threshCtrl->show();

        threshExec = true;
    }

    threshold = threshCtrl->getSliderVal();

    ui->menuBar->setStyleSheet("color: white");
    ui->actionColor_Contours->disconnect(ui->actionColor_Contours,&QAction::triggered,this,&MainWindow::colorContours);

    cv::Mat pic_gray,canny;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::flip(pic,flpPic,1);

    cv::cvtColor(flpPic,pic_gray,cv::COLOR_BGR2GRAY);

    flpPic.release();

    cv::blur(pic_gray,pic_gray,cv::Size(3,3));

    cv::Canny(pic_gray,canny,threshold,threshold*2,3);

    pic_gray.release();

    cv::findContours(canny,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point(0,0));

    flpPic = cv::Mat::zeros(canny.size(),CV_8UC3);

    canny.release();

    for(size_t i = 0;i < contours.size();i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

        cv::drawContours(flpPic,contours,(int)i,color,2,8,hierarchy,0,cv::Point());
    }

    showFrame(flpPic);
}

void MainWindow::showFrame(const cv::Mat &frame)
{
    //Convert the image to the to RGB888 format, a format Qt supports

    switch(frame.type())
    {
        case CV_8UC1: cv::cvtColor(frame,tmpMat,CV_GRAY2RGB); break;

        case CV_8UC3: cv::cvtColor(frame,tmpMat,CV_BGR2RGB); break;
    }

    //You need keep tmpMat in memory for QImage

    assert(tmpMat.isContinuous());

    // Set the QImage variable with the same size of tmpMat and copy the data of tmpMat into qimg

    qimg = QImage(tmpMat.data,tmpMat.cols,tmpMat.rows,tmpMat.cols*3,QImage::Format_RGB888);

    // Set fix the size of the video stream

    this->setFixedSize(frame.cols,frame.rows);

    repaint();
}
