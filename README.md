# docker-tutorial

## Introduction

This is a tutorial on how to use Docker. It is intended for beginners who are new to Docker and want to learn how to use it.

### Prerequisites

Before you start, you need to have Docker installed on your machine. If you don't have it installed, you can download it from the official website: [https://www.docker.com/](https://www.docker.com/)

### What is Docker?

Docker is a platform for developers to develop, ship, and run applications. It allows you to separate your applications from your infrastructure so you can deliver software quickly.

### Why use Docker?

Docker is a great tool for developers because it allows you to package your application and all its dependencies into a container. This makes it easy to ship and run your application on any platform.

### What example is used in this tutorial?

In this tutorial, we will consider a simple C++ application that prints a random 3x3 matrix.
The app is meant to be compiled and run on a Ubuntu 22.04 system.
Two external libraries are used: `boost` and `eigen3`.
Also, the app is built using `CMake`, so the `cmake` package is required as well.

## How to use Docker?

### 1. Create a Dockerfile

To use Docker, you need to create a Dockerfile which contains the instructions to build your application.
The Dockerfile is a text file that contains a list of commands that Docker will execute to build your application.

#### Base image

A Dockerfile always starts with a `FROM` command, which specifies the base image for your application.
Two types of images can be used:

- A _parent image_ is the image that your image is based on. It refers to the contents of the `FROM` directive in the Dockerfile. Each subsequent declaration in the Dockerfile modifies this parent image.
- A _base image_ has `FROM scratch` in its Dockerfile. It is used when you need to completely control the contents of your image.

Most Dockerfiles start from a parent image, rather than a base image. However, the terms are sometimes used interchangeably.

In this tutorial, since we want to run our application on a Ubuntu 22.04 system, we will use the `ubuntu:22.04` image as the base image.
Thus, the first line of our Dockerfile will be:

```Dockerfile
# Base image: Ubuntu 22.04
FROM ubuntu:22.04
```

#### Metadata

The `LABEL` command is used to add metadata to the image. It is a key-value pair that can be used to add information about the image, such as the maintainer, version, and description.

```Dockerfile
# Metadata
LABEL author="Your Name"
LABEL version="latest"
LABEL description="This is a simple C++ application that prints a random 3x3 matrix."
```

#### Install dependencies

The `RUN` command is used to execute commands in the container. It is used to install dependencies, run commands, and perform other tasks.

In our case, we need to install the `boost` and `eigen3` libraries. We can do this using the `apt-get` package manager.

```Dockerfile
# Install dependencies
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y build-essential
RUN apt-get install -y libboost-all-dev libeigen3-dev cmake
```

Note that the `update` and `upgrade` commands are used to update the package list and upgrade the installed packages to the latest versions.
Also, the base image is a minimal installation of Ubuntu, so we need to install the `build-essential` package, which contains the essential tools for building software (e.g., `gcc`, `g++`, `make`).

#### Copy the source code

The `COPY` command is used to copy files from the host machine to the container. It takes two arguments: the source and the destination.

In our case, we need to copy the source code and the `CMakeLists.txt` file to the container.
Usually, rather than specifying individual files, it is better to copy the entire directory except for the files that are listed in the `.dockerignore` file.
The `.dockerignore` file has the same syntax as the `.gitignore` file and is used to avoid the copying of unnecessary files.

```Dockerfile
# Copy the source code 
# The destination is a path in the container
COPY . /random-matrix-generator
```

#### Build the application

Once the source code is copied to the container, we can build the application using the `RUN` command.

```Dockerfile
# Build the application
WORKDIR /random-matrix-generator
RUN mkdir -p build
WORKDIR /random-matrix-generator/build
RUN cmake ..
RUN make
```

The `WORKDIR` command is used to set the working directory for the container. It is similar to the `cd` command in the shell.

#### Entrypoint

The `ENTRYPOINT` command is used to specify the command that will be executed when the container starts.
In our case, we want to run the executable that was built in the previous step.

```Dockerfile
# Run the application
ENTRYPOINT ["/random-matrix-generator/bin/random_matrix_generator"]
```

### 2. Build the Docker image

Once you have created the Dockerfile, you can use the `docker build` command to build the Docker image.

The `docker build` command takes two arguments: the path to the directory containing the Dockerfile and the tag for the image.

```bash
docker build . -t random-matrix-generator:latest
```

The `-t` flag is used to specify the tag for the image. The tag is used to identify the image when you run it.

#### List the images

By running the `docker images` command, you can see the list of images on your machine.

```bash
docker images
```

The `docker images` command lists the images on your machine. It shows the repository, tag, image ID, and creation date.

#### Remove unused images

By default, Docker does not remove images that are not used by any container.
To list the unused images, you can use the following command:

```bash
docker images -f dangling=true
```

To remove the unused images, you can use the `docker rmi` command:

```bash
docker rmi $(docker images -f dangling=true -q)
```

### 3. Run the Docker image

Once the image is built, you can use the `docker run` command to run the image.

```bash
docker run random-matrix-generator:latest
```

The `docker run` command takes the tag of the image as an argument. It runs the image in a container and starts the application.

### 4. Push the Docker image to a registry

Once you have built the Docker image, you can push it to a registry so that it can be used by others.

#### Docker Hub

To push the image to Docker Hub, you need to:

1. Create an account on the Docker Hub website
2. Create a repository with any name `repo_name` (e.g., `random-matrix-generator`)
3. Log into the Docker Hub from the command line

```bash
docker login -u docker_hub_username
# Enter your password when prompted
```

Then, you can use the `docker tag` command to tag the image you built with your Docker Hub username and the repository name.

```bash
docker tag random-matrix-generator:latest docker_hub_username/repo_name:tag_name
```

The `tag_name` is optional. If you don't specify it, Docker will use `latest` as the tag.

Then, you can use the `docker push` command to push the image to Docker Hub.

```bash
docker push docker_hub_username/repo_name:latest
```

The `docker push` command is used to push the image to a registry.
For instance, to push the image to Docker Hub, you can use the following command:

```bash
docker push docker_hub_username/random-matrix-generator:latest
```

#### CyVerse container registry (Harbor)

Instead, to push it to CyVerse container registry, called `Harbor`, you should first create an account on the CyVerse website and then log into [Harbor](https://harbor.cyverse.org/) using your CyVerse username and password.

Then, you must log into the CyVerse container registry from the command line.

```bash
docker login -u your_username harbor.cyverse.org
# Enter your CLI password when prompted
```

The CLI password is not the same as your CyVerse password.
You can find it at [Harbor](https://harbor.cyverse.org/) in the top right menu under `your_username` -> `User Profile` -> `CLI secret`.

Then, you can use the `docker tag` command to tag the image you built with the path to your Harbor repository.

```bash
docker tag random-matrix-generator:latest harbor.cyverse.org/your_repo/random-matrix-generator:latest
```

Then, you can use the `docker push` command to push the image to the CyVerse container registry.

```bash
docker push harbor.cyverse.org/your_repo/random-matrix-generator:latest
```

Note that you must be either the owner of the repository `your_repo` or have the `Maintainer` role to be able to push images to the repository.
