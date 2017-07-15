# PicYou
PicYou is a simple program to take photos and recordo videos with effects or not
 
Written on C++ and using Opencv library and QT Framework

# Installations

# Linux installation (2 ways)

## First Way (Pre-compiled executable): 
This way probably give a better GUI interface than second way. But the second is more recommended

1 - Download this .zip -> http://www.mediafire.com/file/zvb5nya8bait4ha/PicMoments.zip

2 - Unpack .zip file, rgiht-click on the .zip and extract here

3 - Now install the dependencies, open the terminal (CTRL + T) and copy and paste this command(for Debian/Ubuntu/Mint):
   `sudo apt-get install build-essential libopencv-dev qt5-default qtdeclarative5-dev qml-module-qtquick-controls`

4 - Finally open the extracted folder and double click on PicMoments executable file.

## Second Way (Compile and Run):
This is the recommnended way, but GUI interface is a little bit bad (for now)

1 - Download .zip file or clone this repository, to clone(You need to install git) go to a directory and open a terminal         right there, for example:
   `git clone https://github.com/TigaxMT/PicMoments.git`
    
   After unpack or clone the repository you need to open a terminal and go to the PicMoments folder directory.
   For example if you unpack the .zip file or clone on the Documents you type this command on terminal:
   `cd Documents/PicMoments`

2 - Install the dependencies: `sudo apt-get install cmake build-essential libopencv-dev qt5-default qtdeclarative5-dev qml-module-qtquick-controls`

3 - Now we're going to compile the software, copy and paste this commands one by one:
```
   export OpenCV3_DIR=/usr/local/share/OpenCV
   
   cmake .
   
   make
```

4 - Finally , double click on the PicMoments executable (you find it on the PicMoments folder)  

# Windows Compiled Version

I recommend compiling the code on its own, but if you want the software ready, I've compile an .exe 

To download it click here-> http://www.mediafire.com/file/lrpnecgczfa7b7r/PicMoments+V1.0.7.exe

I did the zip installer with the NSIS tool

# OpenCV and QT Versions

··OpenCV 3.2 - http://opencv.org/releases.html

·· QT 5.9 - https://www.qt.io/download-open-source/#section-2

# Contact Me

Email: thebigger018@protonmail.com
