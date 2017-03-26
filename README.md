# v201703
Firmware for v201703

# Rasp Robot Deamon

This app writed on Qt5 use QWebSocket

## Requirements

        $ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev

## Build

        $ qmake
        $ make

## Configure

        $ sudo ln -s `pwd`/etc/rasprobotd /etc/rasprobotd
        $ sudo ln -s `pwd`/etc/init.d/rasprobotd /etc/init.d/rasprobotd
        $ sudo ln -s `pwd`/rasprobotd /usr/bin/rasprobotd
        $ sudo nano /etc/rasprobotd/conf.ini
        $ sudo update-rc.d -f rasprobotd remove
        $ sudo update-rc.d rasprobotd defaults

