FROM ubuntu:latest

# Install requirements
RUN apt-get install cmake wget git
RUN pip3 install sphinx sphinx-rtd-theme breathe

# Get GCC arm tools
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.07/gcc-arm-none-eabi-10.3-2021.07-x86_64-linux.tar.bz2 && \
tar -xf gcc-arm-none-eabi-10.3-2021.07-x86_64-linux.tar.bz2 

ENV GCC_ARM_TOOLS_PATH ~/gcc-arm-none-eabi-10.3-2021.07/bin

# Get the code base
RUN git clone https://github.com/RIT-EVT/EVT-core && \
    cd EVT-core && \
    git checkout feature/cbolles/sphinx-documentation 

ENTRYPOINT
