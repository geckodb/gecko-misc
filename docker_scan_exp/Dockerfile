FROM ubuntu:16.04

RUN     apt-get update && \
        apt-get upgrade -y && \
        apt-get install -y clang cmake

RUN     mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY    . /usr/src/app

CMD     ["ls", ""]
CMD     ["cmake .", ""]
CMD     ["make", ""]
CMD     ["./docker_scan_exp", ""]