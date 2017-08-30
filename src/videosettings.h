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

#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

#include <QDialog>

namespace Ui {
class VideoSettings;
}

class VideoSettings : public QDialog
{
    Q_OBJECT

public:
    explicit VideoSettings(QWidget *parent = 0);
    ~VideoSettings();

    // Pass the value of ComboBox indexed to one of the extensions

    int getExtVal();

    // Pass the value of ComboBox indexed to one of the codecs

    int getCodecVal();

    int getResolutionVal();

    void setExtVal(int val);
    void setCodecVal(int val);
    void setResolutionVal(int val);
    // Function to convert resolution integers to string and put the strings on the comboBox
    void setAllResolutions(int cw,int ch,int hw,int hh);

    // Verify if About dialog is running

    bool dlgExecVal();

    bool resolution_changed = false;

private slots:
    void on_okBtn_clicked();

    void on_resolutionBox_currentIndexChanged(int index);

private:
    Ui::VideoSettings *ui;

    bool dlgExec = false;
    int cur_h, cur_w; // current height and width resolution
    int h_h,h_w; // Highest height and width resolution

    // Override the reject function of QDialog, to change dlgExec value

    void reject();
};

#endif // VIDEOSETTINGS_H
