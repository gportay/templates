#
# Copyright 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# Licensed under the MIT license.
#

FROM sebp/lighttpd

RUN apk add --update --no-cache openssl

COPY genkeys.sh /usr/bin/genkeys.sh

EXPOSE 443

CMD ["lighttpd", "-D", "-f", "/etc/lighttpd/lighttpd-ssl.conf"]
