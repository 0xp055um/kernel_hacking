# docker build -t kernel-dev .
# docker run --rm -it -v "$PWD:/kernel" --name kernel-dev kernel-dev
# docker commit kernel-devel kernel-dev

# Use an Ubuntu base image
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update -q && apt-get install -yq \
    bc \
    bison \
    flex \
    libelf-dev \
    cpio \
    build-essential \
    libssl-dev \
    qemu-system-x86 \
    wget

# Set the working directory
RUN mkdir /kernel
WORKDIR /kernel
