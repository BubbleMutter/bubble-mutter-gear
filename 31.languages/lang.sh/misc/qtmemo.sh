#!/bin/bash

:<<! qtcreator fcitx-sogou stuff
# need platforminputcontexts/libfcitxplatforminputcontextplugin.so
# in gentoo,        $emerge fcitx-qt5
# in debianLike,    $apt-get install fcitx-frontend-qt5
# needed .so-dir is under <qt5_libs_top>/plugins/platforminputcontexts/
# cp platforminputcontexts/ to <qtcreator_libs_top>/plugins/
# in gentoo, under /usr/lib,  qt5/ and qtcreator/
# in debian, under /usr/lib/<!-- <arch_compiler> -->, qt5/ and qtcreator/
!
# Qt 打包 on Windows7/8/10
windeployqt -release . --no-quick-import  --no-plugins