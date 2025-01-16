# docker build -t kernel-devel .
# docker run --rm -it -v "$PWD:/kernel" --name kernel-devel kernel-devel
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
    wget \
    && rm -rf /var/lib/apt/lists/*

# Copy the build script
COPY build2.sh /build.sh
RUN chmod +x /build.sh

# Create the fs directory
RUN mkdir /fs
WORKDIR /fs
RUN mkdir -p bin sbin etc proc sys usr/bin usr/sbin root home/ctf

WORKDIR /

# Run the build script
RUN /build.sh
