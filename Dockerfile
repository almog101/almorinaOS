FROM alpine:latest

RUN apk add curl
RUN curl "https://newos.org/toolchains/x86_64-elf-7.5.0-Linux-x86_64.tar.xz" -o x86.tar.xz
RUN tar -xf x86.tar.xz
