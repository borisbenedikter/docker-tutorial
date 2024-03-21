# Base image: Ubuntu 22.04
FROM ubuntu:22.04

# Metadata
LABEL author="Your Name"
LABEL version="1.0"
LABEL description="This is a simple C++ application that prints a random 3x3 matrix."

# Install dependencies
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y build-essential
RUN apt-get install -y libboost-all-dev libeigen3-dev cmake

# Copy the source code 
# The destination is a path in the container
COPY . /random-matrix-generator

# Build the application
WORKDIR /random-matrix-generator
RUN mkdir build
WORKDIR /random-matrix-generator/build
RUN cmake ..
RUN make

# Run the application
ENTRYPOINT ["/random-matrix-generator/bin/random_matrix_generator"]
