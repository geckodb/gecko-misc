FROM scratch

RUN     mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY    . /usr/src/app

CMD     ["ls", ""]
CMD     ["cmake .", ""]
CMD     ["make", ""]
CMD     ["./docker_scan_exp", ""]
