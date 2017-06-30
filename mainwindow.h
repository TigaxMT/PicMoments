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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "threshold.h"

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
    void capture();
    void record();
    void stopRecord();
    void on_timeout();

    void noEffects();
    void cannyEdge();

private:
    Ui::MainWindow *ui;
    Threshold *threshCtrl;

    std::string picPath;
    int pics = 0; // Count the photos you took
    int recs = 0; // Count the videos you record
    int threshold = 0;
    bool isRec;
    bool threshExec = true;

    cv::VideoCapture cap;
    cv::VideoWriter rec;
    cv::Mat pic;
    cv::Mat tmpMat;
    cv::Mat flpPic;

    std::vector<int> compression_params;

    QTimer timer;
    QImage qimg;

    void paintEvent(QPaintEvent*)
    {
        //Display the frame, in this case a group of frames(a video)

        QPainter painter(this);

        painter.drawImage(QPoint(0,0),qimg);

        painter.end();
    }

    void showFrame(const cv::Mat& frame);

    // The function counts the photos you took and places the photo number as the name

    inline std::string intToString()
    {
        // It was used a stringstream to do the conversion int to string easily

        std::stringstream print;

        // Create a temporary string for not change the string where path is

        std::string tmp = picPath;

        print << pics; // converting a integer to a string

        tmp +="/";
        tmp += print.str();
        tmp += ".jpg";

        return tmp;
    }

    inline std::string intToStringRec()
    {
        // It was used a stringstream to do the conversion int to string easily

        std::stringstream print;

        // Create a temporary string for not change the string where path is

        std::string tmp = picPath;

        print << recs; // converting a integer to a string

        tmp +="/";
        tmp += print.str();
        tmp += ".mp4";

        return tmp;
    }
};

#endif // MAINWINDOW_H
