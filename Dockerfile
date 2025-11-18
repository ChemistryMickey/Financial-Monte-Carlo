FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV PROJECT_HOME="/finance_monte_carlo"

# Update and install critical packages
RUN apt-get update \
    && apt-get install -y \
    gcc-14 \
    g++-14 \
    git \
    lld \
    vim \
    wget \
    python3 \
    curl \
    ca-certificates \
    tree \
    ripgrep \
    && rm -rf /var/lib/apt/lists/*

# Set gcc/++ 14 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100

WORKDIR ${PROJECT_HOME}

# Install Cargo
RUN curl https://sh.rustup.rs -ysSf | sh

# Install uv (Python)
RUN curl -LysSf https://astral.sh/uv/install.sh | sh

# Install Bazelisk
RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.27.0/bazelisk-amd64.deb
RUN dpkg -i bazelisk-amd64.deb
RUN rm bazelisk-amd64.deb

CMD ["/bin/bash"]