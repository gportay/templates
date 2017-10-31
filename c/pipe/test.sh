#!/bin/sh

cat << EOF | while read l; do echo "$l" && sleep 1; done
one
two
three
four
five
six
seven
eight
nine
ten
EOF
