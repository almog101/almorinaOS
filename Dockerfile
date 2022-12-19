FROM alpine:latest

RUN apk add build-base grub xorriso curl gcompat libc6-compat libstdc++ make nasm
RUN curl "https://newos.org/toolchains/x86_64-elf-7.5.0-Linux-x86_64.tar.xz" -o x86.tar.xz
RUN tar -xf x86.tar.xz && rm x86.tar.xz

WORKDIR "x86_64-elf-7.5.0-Linux-x86_64"

RUN cp -r bin/. /usr/bin/
RUN cp -r lib/. /lib
RUN cp -r share/. /usr/share
RUN mkdir -p /usr/libexec && cp -r libexec/. /usr/libexec

WORKDIR /
run rm -rf "x86_64-elf-7.5.0-Linux-x86_64"
