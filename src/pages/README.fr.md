
# 3615 SSH

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md) [![fr](https://img.shields.io/badge/lang-fr-blue.svg)](README.fr.md)

Un client SSH sur un minitel : une idée potentiellement stupide pour enfin amener les Minitels vers le ~~futur~~ présent ! Tout ça en utilisant un ESP32 pour se connecter en WiFi.

## Comment l'utiliser

J'ai progammé le 3615 SSH avec vscode, et platformio. Tout devrait marcher automagiquement. Les bonnes versions des bibliothèques devraient se gérer toutes seules.

## Bibliothèques utilisées

* LibSSH-ESP32 par Ewan Parker
C'est un portage incroyable de libSSH sur un ESP32. Un travail remarquable !

* Minitel1B_hard (mon propre fork)
Une bibliothèque d'Éric Serandour (super boulot !) pour contrôler un Minitel avec un arduino. Je l'ai forkée pour ajouter quelques trucs dont j'avais besoin.

## D'autres choses qui peuvent être interessantes

* J'ai passé beaucoup de temps à transcoder les Spécifications Techniques du Minitel dans un format facile à utiliser. Fini, le pdf de 60Mo sans OCR ! [C'est disponible ici](https://jbellue.github.io/stum1b/). Vu le temps que j'ai gaché là-dessus, je serais enchanté que ça serve à quelqu'un !

* [miedit](https://zigazou.github.io/miedit/) m'a aussi été très utile pour faire un peu de mise en page et de design
