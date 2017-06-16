/************************************************************************************************************
*    											                                                            *
*    PicSelf - Is a simple program to take photos. Written on C++ and using Opencv library and QT Framework *
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

    // Choose the directory you want the photos have saved

    picPath = QFileDialog::getExistingDirectory(this,"Directory to save your Pics",QDir::currentPath()).toStdString();

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

    ui->picSelfBtn->disconnect(ui->picSelfBtn,&QPushButton::clicked,this,&MainWindow::capture);

    // Capture a frame to Mat Image

    cap >> pic;

    // And save it in the path created on intToString function

    cv::imwrite(intToString(),pic);

    pics++;
}


void MainWindow::on_timeout()
{
    // Start capture frames

    cap.read(pic);

    // If the pushButton has been pressed a signal is emitted and initializes your private slot capture()

    connect(ui->picSelfBtn,&QPushButton::clicked,this,&MainWindow::capture);
}
