FROM ubuntu:xenial

# update and install dependencies
RUN         apt-get update \
                && apt-get install -y \
                    software-properties-common \
                    wget \
                && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
                && apt-get update \
                && apt-get install -y \
                    make \
                    curl \
                && apt-get install -y \
                    gcc-4.9 g++-4.9 gcc-4.9-base \
                && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 100 \
                && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 100

# Install toolchain
RUN apt-get update \
  && apt-get install -y git \
    mercurial \
    libtool \
    pkg-config \
    build-essential \
    autoconf \
    automake \
    uuid-dev \
    gcc \
    make \
    libc6-dev \
    libpcap-dev \
    libssl-dev \
    autoconf \
    gettext \
    apt-utils \
    libtool \
  && ln -s /usr/bin/libtoolize /usr/bin/libtool


  COPY . /usr/src/service

  WORKDIR /usr/src/service

  RUN ./autogen.sh
  # RUN sleep 5 && ./bin/service & \

  COPY docker-entrypoint.sh /usr/local/bin/

  CMD ["./bin/service"]
