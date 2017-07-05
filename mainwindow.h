/************************************************************************************************************
*    											                                                            *
*    PicMoments - Is a simple program to take photos. Written on C++ and using Opencv library and QT Framework *
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
#include "about.h"
#include "videosettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //onst int h,w; // Set fixed Size for mainwindow with minimum resolution of the webcam

private slots:
    void capture();
    void record();
    void stopRecord();
    void on_timeout();

    void about();
    void videoSettings();

    void noEffects();
    void cannyEdge();
    void colorContours();
    void sobel();

private:
    Ui::MainWindow *ui;
    Threshold *threshCtrl;
    About *aboutDlg;
    VideoSettings *vidDlg;

    std::string picPath;
    std::string ext;
    double threshold;
    int pics; // Count the photos you took
    int recs; // Count the videos you record
    int fourcc;
    int extIndexVal; // Save the index of the extension value on the comboBox(VideoSettings)
    int codecIndexVal;// Save the index of the codec value on the comboBox(VideoSettings)
    int current_h,current_w; // current height and width resolution
    int highest_h,highest_w; // Highest height and width resolution
    int resolutionVal; // Save the index of resolution value on the comboBox(VideoSettings)
    bool isRec;
    bool threshExec;
    bool aboutExec;
    bool vidDlgExec;

    cv::VideoCapture cap;
    cv::VideoWriter rec;
    cv::Mat pic;
    cv::Mat tmpMat;
    cv::Mat flpPic;
    cv::RNG rng;

    std::vector<int> compression_params;

    QTimer timer;
    QImage qimg;

    void maximumResolution();

    void paintEvent(QPaintEvent*);

    void showFrame(const cv::Mat& frame);

    // The function counts the photos you took and places the photo number as the name

    std::string intToString();

    std::string intToStringRec();
};

#endif // MAINWINDOW_H
