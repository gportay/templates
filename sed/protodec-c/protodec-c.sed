#!/bin/sed -nf
#
# Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License version 3 as published by the
# Free Software Foundation.
#

/^typedef enum \+_/,/} .\+;/ {
	/typedef enum \+_/s,typedef enum \+_\(.*\) {,enum \1 {,;/PROTOBUF_C__/d
	/^  .\+__.\+ = /s/[A-Z0-9_]\+__\([A-Z0-9_]\+ = [0-9]\+\),\?/\1;/
	/^} .\+;$/s, .\+;,,
}

/^struct \+_/,/} .\+;/ {
	s,struct \+_\(.*\),message \1 {,;
	/^{/d
	/^};$/s,;$,,
	/ProtobufCMessage \+base/d
	/protobuf_c_boolean \+has_/{s,protobuf_c_boolean.*$,optional,;N;s/\n \+/ /}
	/size_t \+n_/{s,size_t \+.*$,repeated,;N;s/\n \+/ /}
	/u\?int[0-9]\+_t/s,_t,,
	/char \+\*/s,char \+\*,string ,
	/protobuf_c_boolean \+/s,protobuf_c_boolean,bool,
	/ProtobufCBinaryData \+/s,ProtobufCBinaryData,bytes,
	/__.* \+\*/s,\*,,
	/.* \*.*;/s,\*,,
}

/^message \+/,/}/p
/^enum \+/,/}/p

# ex: filetype=sed
