FROM ethereum/client-go:alltools-latest

RUN apk update
RUN apk upgrade
RUN apk add bash

COPY start.sh /root/start.sh
RUN chmod +x /root/start.sh

ENTRYPOINT /root/start.sh 