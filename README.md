# SopovRobotics firmware for v201703 (Infrared eye)

This server based on Qt5 + QWebSocket Server

## Requirements

        $ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev

## Build

        $ qmake
        $ make

## Configure

        $ sudo ln -s `pwd`/etc/sopovrobotics /etc/rasprobotd
        $ sudo ln -s `pwd`/etc/init.d/sopovrobotics /etc/init.d/sopovrobotics
        $ sudo ln -s `pwd`/sopovrobotics /usr/bin/sopovrobotics
        $ sudo nano /etc/sopovrobotics/conf.ini
        $ sudo update-rc.d -f sopovrobotics remove
        $ sudo update-rc.d sopovrobotics defaults

