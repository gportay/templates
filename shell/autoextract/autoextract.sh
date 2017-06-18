#!/bin/sh
#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

set -e

exec sh -c "sed -n -e '/----- >8 -----/,\${//d;p}' $0 | tar xz"
