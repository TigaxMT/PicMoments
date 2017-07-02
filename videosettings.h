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

    // Verify if About dialog is running


    bool dlgExecVal()
    {
        return dlgExec;
    }

private:
    Ui::VideoSettings *ui;

    bool dlgExec = false;

    // Override the reject function of QDialog, to change dlgExec value

    void reject();
};

#endif // VIDEOSETTINGS_H
