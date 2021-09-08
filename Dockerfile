# This Dockerfile is intended to have all the tools needed for building
# EVT-core and supporting documentation. This can be used as is to avoid
# installing requirements directly on your system, or used for CI/CD workflows.

# Build on Ubuntu
FROM ubuntu:latest

###############################################################################
# Arguments
###############################################################################
# The branch, tag, commit, etc to checkout
ARG target_ref=feature/cbolles/sphinx-documentation

###############################################################################
# Install requirments
###############################################################################
# Install requirements
RUN apt-get update -y && \
    DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata
RUN apt-get update -y && \
    apt-get install -y cmake wget git doxygen python3.8 python3-pip

# Get GCC arm tools
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.07/gcc-arm-none-eabi-10.3-2021.07-x86_64-linux.tar.bz2 && \
    tar -xf gcc-arm-none-eabi-10.3-2021.07-x86_64-linux.tar.bz2
ENV GCC_ARM_TOOLS_PATH /gcc-arm-none-eabi-10.3-2021.07/bin

# Specify the entry point 
RUN git clone https://github.com/RIT-EVT/EVT-core.git && \
    cd EVT-core && \
    pip3 install -r requirements.txt && \
    git checkout ${target_ref} && \
    mkdir build && \
    cd build && \
    cmake -DEVT_BUILD_DOCS=ON ../
WORKDIR EVT-core/
