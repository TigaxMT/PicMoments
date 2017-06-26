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
    isRec(false),
    timer(this)
{
    ui->setupUi(this);
    ui->menuBar->setStyleSheet("color: white");

    if(!cap.isOpened())
    {
        return;
    }

    // This vector store the type of image quality (JPEG quality from 0 to 100, 95 is the default value)

    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(95);

    // Choose the directory you want the photos/videos have saved

    picPath = QFileDialog::getExistingDirectory(this,"Directory to save your Pics/Vids",QDir::currentPath()).toStdString();

    //when the timer times out the signal is emitted and the ontimeout() private slot is executed

    connect(&timer,SIGNAL(timeout()),this,SLOT(on_timeout()));

    //call the timer every 10 ms

    timer.start(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::capture()
{
    // must disconnect the pushButton signal, if not the program doesn't stop to take photos

    ui->picBtn->disconnect(ui->picBtn,&QPushButton::clicked,this,&MainWindow::capture);

    // Capture a frame to Mat Image

    cap >> pic;

    //Flip the frame

    cv::flip(pic,flpPic,1);

    // And save it in the path created on intToString function

    cv::imwrite(intToString(),flpPic,compression_params);

    // Show a message in the statusbar for 1/2 sec

    ui->statusBar->setStyleSheet("color: white");
    ui->statusBar->showMessage("Pic Captured",500);

    pics++;
}

void MainWindow::record()
{
    if(!isRec)
    {
        // Size of the frame
        cv::Size S = cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH),
                              (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT));

        // Codec for the video

        int ex = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));

        rec.open(intToStringRec(),ex,cap.get(cv::CAP_PROP_FPS),S,true);

        if(!rec.isOpened())
            return;

        isRec=true;
    }

    rec << flpPic;

    connect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::stopRecord);
}

void MainWindow::stopRecord()
{
    ui->picBtn->disconnect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::record);
    ui->picBtn->disconnect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::stopRecord);

    isRec = false;
    recs++;
}

void MainWindow::on_timeout()
{
    // Start capture frames

    cap.read(pic);

    // Flip the frame

    cv::flip(pic,flpPic,1);

    showFrame(flpPic);

    // If the pushButton has been pressed a signal is emitted and initializes your private slot capture()

    if(isRec)
        record();

    connect(ui->picBtn,&QPushButton::clicked,this,&MainWindow::capture);
    connect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::record);
}

void MainWindow::showFrame(const cv::Mat &frame)
{
    //Convert the image to the to RGB888 format, a format Qt supports

    cv::cvtColor(frame,tmpMat,CV_BGR2RGB);

    //You need keep tmpMat in memory for QImage

    assert(tmpMat.isContinuous());

    // Set the QImage variable with the same size of tmpMat and copy the data of tmpMat into qimg

    qimg = QImage(tmpMat.data,tmpMat.cols,tmpMat.rows,tmpMat.cols*3,QImage::Format_RGB888);

    // Set fix the size of the video stream

    this->setFixedSize(frame.cols,frame.rows);

    repaint();
}
