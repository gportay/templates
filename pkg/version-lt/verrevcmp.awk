#!/usr/bin/awk -f
#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

{
	split($1, val, ".");
	split($2, ref, ".");

	i = 1;
	while (val[i] != "" || ref[i] != "") {
		if (val[i] == "" || val[i] < ref[i]) {
			ret = -1;
			break;
		}
		else if (ref[i] == "" || val[i] > ref[i]) {
			ret = 1;
			break;
		}

		i++;
	}

	if (ret >= 0) {
		exit 1
	}
}
