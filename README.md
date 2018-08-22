# PicYou
PicYou is a simple program to take photos and recordo videos with effects or not
 
Written on C++ and using Opencv library and QT Framework

# Warning

This project will no longer be continued. Any errors or functionality that you want to correct or implement will be welcome, but I will not work on this project anymore because I am focused on web development. Thanks for everything :)

# Installations

# Linux installation

1 - Download .zip file or clone this repository, to clone(You need to install git) go to a directory and open a terminal         right there, for example:
   `git clone https://github.com/TigaxMT/PicMoments.git`
    
   After unpack or clone the repository you need to open a terminal and go to the PicMoments folder directory.
   For example if you unpack the .zip file or clone on the Documents you type this command on terminal:
   `cd Documents/PicMoments`

2 - Install the dependencies(Debian Based): `sudo apt-get install cmake build-essential libopencv-dev qt5-default qtdeclarative5-dev qml-module-qtquick-controls`
     
    Install the dependencies(RedHat, Fedora) : `sudo dnf install automake gcc gcc-c++ kernel-devel make cmake opencv opencv-core opencv-devel qt5 qt5-devel qt5-qtbase qt5-qtbase-devel qt5-qtdeclarative qt5-qtdeclarative-devel`    

3 - Now we're going to compile the software, copy and paste this commands one by one:
    
   Note: Before, you need to verify if /usr/local/share/OpenCV directory exists.
          
   If not exist verify this: /usr/lib/OpenCV , if this directory exists so change the export path, like this:
          
```
    export OpenCV3_DIR=/usr/lib/OpenCV
   
    cmake .
   
    make
        
```
        
   Now, if when you run make have an error try this:
        
```
    export OpenCV3_DIR=/usr/lib64/OpenCV
   
    cmake .
   
    make
```
   But if /usr/local/share/OpenCV exists just do like this:     
```
   export OpenCV3_DIR=/usr/local/share/OpenCV
   
   cmake .
   
   make
```


4 - Finally , double click on the PicMoments executable (you find it on the PicMoments folder)  

# Windows Compiled Version

I recommend compiling the code on its own, but if you want the software ready, I've compile an .exe 

To download it click here-> http://www.mediafire.com/file/1xe3iate46ippbg/PicMoments+V1.0.7.exe

I did the zip installer with the NSIS tool

# OpenCV and QT Versions

··OpenCV 3.2 - http://opencv.org/releases.html

·· QT 5.9 - https://www.qt.io/download-open-source/#section-2

# Contact Me

Email: ttiagommartins127@gmail.com
