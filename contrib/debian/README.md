
Debian
====================
This directory contains files used to package kubocoind/kubocoin-qt
for Debian-based Linux systems. If you compile kubocoind/kubocoin-qt yourself, there are some useful files here.

## kubocoin: URI support ##


kubocoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install kubocoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your kubocoinqt binary to `/usr/bin`
and the `../../share/pixmaps/kubocoin128.png` to `/usr/share/pixmaps`

kubocoin-qt.protocol (KDE)

