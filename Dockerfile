FROM gcc:latest

LABEL description="Docker wrapper for a custom service"

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

# CMD ["./bin/service"]

# Run
ENTRYPOINT ["./bin/service"]
