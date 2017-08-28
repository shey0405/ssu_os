#!/bin/sh

yum groupinstall -y "X Window System"

yum groupinstall -y "Internet Browser"

yum install -y gnome-classic-session gnome-terminal nautilus-open-terminal control-center liberation-mono-fonts

yum -y install man-pages-ko autocorr-ko python-libvoikko libreoffice-langpack-ko hunspell-ko ibus-hangul ibus-hangul libhangul ghc-language-java php-symfony-expression-language ghc-language-java-devel system-config-language

localectl set-locale LANG=ko_KR.UTF-8

cat > /etc/yum.repos.d/google-chrome.repo << '__EOF__'

[google-chrome]

name=google-chrome

baseurl=http://dl.google.com/linux/chrome/rpm/stable/x86_64

enabled=1

gpgcheck=1

gpgkey=https://dl-ssl.google.com/linux/linux_signing_key.pub

__EOF__

yum -y install google-chrome-stable


systemctl stop firewalld

systemctl disable firewalld


ln -sf /lib/systemd/system/graphical.target /etc/systemd/system/default.target



