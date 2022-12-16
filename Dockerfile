FROM debian:bookworm-20221205-slim
RUN apt update 
RUN apt dist-upgrade -y 
RUN apt install nasm gcc make dosfstools curl mtools libfl2 libmpc3 -y
WORKDIR /tmp
RUN mkdir -p /tmp/cross && mkdir -p /tmp/build && mkdir -p /tmp/log
RUN curl https://kod.prz.edu.pl/repository/micros_devtools_linux/cross.tar.gz | tar -xz --strip-components=1 -C /tmp/cross
COPY ./ /tmp/
RUN CROSS=/tmp/cross BUILD_DIR=/tmp/build LOG_DIR=/tmp/log make build -j $(nproc)