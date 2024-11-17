FROM ubuntu:latest

RUN apt update
RUN apt install -y nasm build-essential file

WORKDIR /thefinalone

CMD ["make"]
