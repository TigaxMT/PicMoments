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

    // set the maximum resolution of webcam supports

    maximumResolution();

    cap.set(CV_CAP_PROP_FRAME_WIDTH, current_w);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, current_h);

    ui->actionNo_Effects->setChecked(true);

    // Init some vars

    pics = 0; recs = 0; threshold = 0; extIndexVal = 0; codecIndexVal = 0, resolutionVal = 0;
    isRec = false; threshExec = false; aboutExec = false; vidDlgExec = false;

    rng(12345);

    fourcc = CV_FOURCC('X','V','I','D');

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

    if(ui->actionCanny_Edges->isChecked() || ui->actionSobel->isChecked())
    {
        cv::imwrite(intToString(),fxPic,compression_params);

        ui->statusBar->setStyleSheet("color: red");
        ui->statusBar->showMessage("Pic Captured",500);
    }
    else
        if(ui->actionColor_Contours->isChecked())
        {
            cv::imwrite(intToString(),fxPic,compression_params);

            ui->statusBar->setStyleSheet("color: white");
            ui->statusBar->showMessage("Pic Captured",500);
        }
        else
        {
            // Capture a frame to Mat Image

            cap >> pic;

            //Flip the frame

            cv::flip(pic,fxPic,1);

            // And save it in the path created on intToString function

            cv::imwrite(intToString(),fxPic,compression_params);

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


        if(ui->actionCanny_Edges->isChecked() || ui->actionSobel->isChecked())
            rec.open(intToStringRec(),fourcc,10,S,false);
        else
            rec.open(intToStringRec(),fourcc,10,S,true);


        if(!rec.isOpened())
            return;

        isRec=true;
    }

    rec.write(fxPic);
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

    ui->actionAbout_PicMoments->disconnect(ui->actionAbout_PicMoments,&QAction::triggered,this,&MainWindow::about);
}

void MainWindow::videoSettings()
{
    if(!vidDlgExec)
    {
       vidDlg = new VideoSettings(this);

       vidDlg->show();

       vidDlg->setExtVal(extIndexVal);
       vidDlg->setCodecVal(codecIndexVal);
       vidDlg->setAllResolutions(current_w,current_h,highest_w,highest_h);
       vidDlg->setResolutionVal(resolutionVal);

       vidDlgExec = true;
    }

    // Only update webcam resolution when resolution comboBox index had changed
    // To prevent some lag

    if(vidDlg->resolution_changed)
        switch(vidDlg->getResolutionVal())
        {
            case 0: cap.set(CV_CAP_PROP_FRAME_WIDTH, current_w);
                    cap.set(CV_CAP_PROP_FRAME_HEIGHT, current_h);
                    resolutionVal = 0;vidDlg->resolution_changed = false;
                        break;

           case 1: cap.set(CV_CAP_PROP_FRAME_WIDTH,highest_w);
                   cap.set(CV_CAP_PROP_FRAME_HEIGHT,highest_h);
                   resolutionVal = 1;vidDlg->resolution_changed = false;
                        break;
        }

    switch(vidDlg->getExtVal())
    {
        case 0: extIndexVal = 0;
                ext = ".avi";
                break;
        case 1: extIndexVal = 1;
                ext = ".mov";
                break;
    }

    switch(vidDlg->getCodecVal())
    {
        case 0: fourcc = CV_FOURCC('X','V','I','D');codecIndexVal = 0;break;
        case 1: fourcc = CV_FOURCC('D','I','V','X');codecIndexVal = 1;break;
        case 2: fourcc = CV_FOURCC('D','I','V','3');codecIndexVal = 2;break;
        case 3: fourcc = CV_FOURCC('D','I','V','2');codecIndexVal = 3;break;
        case 4: fourcc = CV_FOURCC('M','J','P','G');codecIndexVal = 4;break;
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
            if(ui->actionSobel->isChecked())
                sobel();
            else
            {
                noEffects();
            }

    if(isRec)
        record();

    if(ui->actionVideo_Settings->isChecked())
    {
        if(vidDlg->dlgExecVal() == true)
              videoSettings();
        else
        {
            ui->actionVideo_Settings->setChecked(false);
            vidDlgExec = false;
        }
    }

    if(ui->actionAbout_PicMoments->isChecked())
    {
        if(aboutDlg->dlgExecVal() == false)
        {
            ui->actionAbout_PicMoments->setChecked(false);

            aboutExec = false;
        }
    }

    // Signals and Slots

    connect(ui->picBtn,&QPushButton::clicked,this,&MainWindow::capture);
    connect(ui->recBtn,&QPushButton::clicked,this,&MainWindow::record);
    connect(ui->stopRecBtn,&QPushButton::clicked,this,&MainWindow::stopRecord);
    connect(ui->actionNo_Effects,&QAction::triggered,this,&MainWindow::noEffects);
    connect(ui->actionCanny_Edges,&QAction::triggered,this,&MainWindow::cannyEdge);
    connect(ui->actionColor_Contours,&QAction::triggered,this,&MainWindow::colorContours);
    connect(ui->actionSobel,&QAction::triggered,this,&MainWindow::sobel);
    connect(ui->actionAbout_PicMoments,&QAction::triggered,this,&MainWindow::about);
    connect(ui->actionVideo_Settings,&QAction::triggered,this,&MainWindow::videoSettings);
}

void MainWindow::noEffects()
{
    // noEffects() doesn't need threshold dialog, so if he are running we stop him

    if(threshExec)
    {
        threshCtrl->close();

        delete threshCtrl;

        threshExec = false;

        ui->menuBar->setStyleSheet("color: white");
    }

    ui->actionColor_Contours->setChecked(false);
    ui->actionSobel->setChecked(false);
    ui->actionCanny_Edges->setChecked(false);
    ui->actionNo_Effects->setChecked(true);

    ui->actionNo_Effects->disconnect(ui->actionNo_Effects,&QAction::triggered,this,&MainWindow::noEffects);

    // Flip the frame

    cv::flip(pic,fxPic,1);

    showFrame(fxPic);
}

void MainWindow::cannyEdge()
{
    ui->actionColor_Contours->setChecked(false);
    ui->actionSobel->setChecked(false);
    ui->actionCanny_Edges->setChecked(true);
    ui->actionNo_Effects->setChecked(false);

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

    cv::flip(pic,fxPic,1);

    cv::cvtColor(fxPic,pic_gray,cv::COLOR_BGR2GRAY);

    cv::blur(pic_gray,fxPic,cv::Size(3,3));

    cv::Canny(fxPic,fxPic,threshold,threshold*3,3);

    showFrame(fxPic);
}

void MainWindow::colorContours()
{
    ui->actionColor_Contours->setChecked(true);
    ui->actionSobel->setChecked(false);
    ui->actionCanny_Edges->setChecked(false);
    ui->actionNo_Effects->setChecked(false);

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

    cv::flip(pic,fxPic,1);

    cv::cvtColor(fxPic,pic_gray,cv::COLOR_BGR2GRAY);

    cv::blur(pic_gray,pic_gray,cv::Size(3,3));

    cv::Canny(pic_gray,canny,threshold,threshold*2,3);

    cv::findContours(canny,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point(0,0));

    fxPic = cv::Mat::zeros(canny.size(),CV_8UC3);

    for(size_t i = 0;i < contours.size();i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

        cv::drawContours(fxPic,contours,(int)i,color,2,8,hierarchy,0,cv::Point());
    }

    showFrame(fxPic);
}

void MainWindow::sobel()
{
    // sobel() doesn't need threshold dialog, so if he are running we stop him

    if(threshExec)
    {
        threshCtrl->close();

        delete threshCtrl;

        threshExec = false;

        ui->menuBar->setStyleSheet("color: white");
    }

    ui->actionColor_Contours->setChecked(false);
    ui->actionSobel->setChecked(true);
    ui->actionCanny_Edges->setChecked(false);
    ui->actionNo_Effects->setChecked(false);

    ui->menuBar->setStyleSheet("color: white");
    ui->actionSobel->disconnect(ui->actionSobel,&QAction::triggered,this,&MainWindow::sobel);

    cv::Mat pic_gray,grad_x, grad_y,abs_grad_x, abs_grad_y;

    cv::flip(pic,fxPic,1);

    cv::GaussianBlur(fxPic,fxPic,cv::Size(3,3),0,0,cv::BORDER_DEFAULT);

    cv::cvtColor(fxPic,pic_gray,cv::COLOR_BGR2GRAY);

    cv::Sobel(pic_gray,grad_x,CV_16S,1,0,3,1,0,cv::BORDER_DEFAULT);
    cv::Sobel(pic_gray,grad_y,CV_16S,0,1,3,1,0,cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x,abs_grad_x);
    cv::convertScaleAbs(grad_y,abs_grad_y);

    cv::addWeighted(abs_grad_x,0.5,abs_grad_y,0.5,0,fxPic);

    showFrame(fxPic);
}

void MainWindow::maximumResolution()
{
    //Save current height and width

    current_w = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    current_h = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    // Get maximum resolution

    cap.set(CV_CAP_PROP_FRAME_WIDTH,10000);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,10000);

    highest_w = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    highest_h = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
}

void MainWindow::showFrame(const cv::Mat &frame)
{
    //Convert the image to the to RGB888 format, a format Qt supports

    switch(frame.type())
    {
        //case CV_8UC:  cv::cvtColor(frame,tmpMat,CV_GRAY2RGB); break;

        case CV_8UC1: cv::cvtColor(frame,tmpMat,CV_GRAY2RGB); break;

        case CV_8UC3: cv::cvtColor(frame,tmpMat,CV_BGR2RGB); break;
    }

    //You need keep tmpMat in memory for QImage

    assert(tmpMat.isContinuous());

    // Set the QImage variable with the same size of tmpMat and copy the data of tmpMat into qimg

    qimg = QImage(tmpMat.data,tmpMat.cols,tmpMat.rows,tmpMat.cols*3,QImage::Format_RGB888);

    // Resize the QImage for keep the mainwindow small (because the minimum resolution of the webcam is used for the fixed size window)

    if(resolutionVal == 1)
    {
        QImage scaled = qimg.scaled(current_w,current_h,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

        qimg = scaled;
    }
    // Set fix the size of the video stream

    this->setFixedSize(current_w,current_h);

    repaint();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //Display the frame, in this case a group of frames(a video)

    QPainter painter(this);

    painter.drawImage(QPoint(0,0),qimg);

    painter.end();
}

std::string MainWindow::intToString()
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

std::string MainWindow::intToStringRec()
{
    // It was used a stringstream to do the conversion int to string easily

    std::stringstream print;

    // Create a temporary string for not change the string where path is

    std::string tmp = picPath;

    print << recs; // converting a integer to a string

    tmp +="/";
    tmp += print.str();
    tmp += ext;

    return tmp;
}
