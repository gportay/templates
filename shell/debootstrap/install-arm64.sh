#!/usr/bin/debootstrap-shell -C rootfs-arm64 --arch arm64
#
# Copyright 2019 Gaël PORTAY
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

set -e

apt update
apt install -y file
