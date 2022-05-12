
# 3615 SSH

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md) [![fr](https://img.shields.io/badge/lang-fr-blue.svg)](README.fr.md)

An SSH client running on a Minitel : a potientially stupid idea to bring Minitels to the ~~future~~ present using an ESP32 to connect over WiFi.

## How to build

I used platformio and vscode. Things should automagically work, and the libraries should sort themselves out.

## Libraries used

* LibSSH-ESP32 by Ewan Parker
This is an incredible port of the LibSSH library to the ESP32. Truly great work!

* Minitel1B_hard (my own fork)
Originally a library by Éric Serandour (awesome work!) to control a Minitel with an arduino. I forked it to implement a couple of things I needed.

## Other things potentially worthy of your time

* I spent a great deal of time getting the Minitel's specs in an easy to read format. [Go check it here](https://jbellue.github.io/stum1b/). Given the time I wasted on that, I do hope it will someday be useful to someone!

* [miedit](https://zigazou.github.io/miedit/) was extremely useful to design and draw some of the pages
