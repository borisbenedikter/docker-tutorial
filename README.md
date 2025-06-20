# docker-tutorial

## Introduction

This is a tutorial on how to use Docker. It is intended for beginners who are new to Docker and want to learn how to use it.

### Prerequisites

Before you start, you need to have the Docker Engine installed on your machine. 
If you don't have it installed, you can learn how to do it from the [official website](https://docs.docker.com/engine/install/).

The following steps install the Docker Engine on Ubuntu 22.04:

1. Set up the `apt` repository where the Docker packages are stored:

```bash
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```

2. Install the Docker packages:

```bash
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

3. Verify that Docker is installed correctly by running the `hello-world` container:

```bash
sudo docker run hello-world
```

If you see the message `Hello from Docker!`, then Docker is installed correctly.
Otherwise, if you see the message `docker: Cannot connect to the Docker daemon at unix:///var/run/docker.sock. Is the docker daemon running?`, you need to start the Docker service:

```bash
sudo service docker start
```

### Run Docker without `sudo`

By default, the Docker daemon binds to a Unix socket owned by the user `root`.
If you want to run Docker commands without `sudo`, you need to add your user to the `docker` group.
First, create the `docker` group:

```bash
sudo groupadd docker
```

Then, add your user to the `docker` group:

```bash
sudo usermod -aG docker $USER
```

Log in to the new docker group (to avoid having to log out and log in again; but if not enough, try to reboot):

```bash
newgrp docker
```

Finally, verify that you can run Docker commands without `sudo`:

```bash
docker run hello-world
```

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

#### Specify Which Dockerfile to Use

Some projects may have multiple Dockerfiles in the same directory.
The best practice to handle multiple Dockerfiles is to add meaningful suffixes to the Dockerfile names (e.g., `Dockerfile.dev`, `Dockerfile.prod`).
Then, you can specify which Dockerfile to use by passing the `-f` flag to the `docker build` command.

```bash
docker build . -f Dockerfile.dev -t random-matrix-generator:latest
```

#### List the images

By running the `docker images` command, you can see the list of images on your machine.

```bash
docker images
```

The `docker images` command lists the images on your machine. It shows the repository, tag, image ID, and creation date.

#### Remove unused images (optional)

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

#### Automatically remove the container after it stops

To automatically remove the container after it stops, you can use the `--rm` flag.

```bash
docker run --rm random-matrix-generator:latest
```

This is useful to avoid leaving stopped containers on your machine, which can take up disk space.

To manually remove all stopped containers, you can use the following command:

```bash
docker rm $(docker ps -aq)
```

#### Add command-line arguments

To add command-line arguments to the application, you can simply append them to the `docker run` command.

```bash
docker run random-matrix-generator:latest arg1 arg2
```

#### Mount a Volume

This is not the case in this example, but in general, you may want to access files that the docker container generates.
To do this, you can use the `-v` flag to mount a volume from the host machine to the container.

```bash
docker run -v /path/to/host/dir:/path/to/container/dir random-matrix-generator:latest
```

Mounting a volume allows you to share files between the host machine and the container.
Specifically, host directory `/path/to/host/dir` is mounted to the container directory `/path/to/container/dir`.
Every file that is created in that container directory will be accessible from that host directory.

#### Run the Container in the Background

By default, the `docker run` command runs the container in the foreground.
To run the container in the background, you can use the `-d` flag.

```bash
docker run -dit random-matrix-generator:latest
```

The `-d` flag is used to run the container in detached mode.
This means that the container runs in the background and does not attach to the terminal.
The `-i` flag is used to keep STDIN open even if not attached.
The `-t` flag is used to allocate a pseudo-TTY (i.e., a terminal).

Having the container running in the background is useful when you want to run long-running processes.
This is also useful when you want to attach VSCode to the running container for opening the source code.
Indeed, you can attach VSCode only to a running container.
In case your container runs only for a short time, you can append the `/bin/bash` command to the `docker run` command to open a shell in the container and thus keep it running:

```bash
docker run -dit random-matrix-generator:latest /bin/bash
```

In case your container has an entrypoint, you can append the `--entrypoint /bin/bash` flag to the `docker run` command to override the entrypoint and open a shell in the container:

```bash
docker run -dit --entrypoint /bin/bash random-matrix-generator:latest
```

When running in detached mode, docker assigns a random name to the container (e.g., `frosty_bell`, `happy_banach`).
To assign a specific name to the container, you can use the `--name` flag:

```bash
docker run -dit --name my_container random-matrix-generator:latest
```

A container name must be unique on a Docker host.
Even after the container is stopped, the name is still reserved.
To be able to reuse the name, you must stop and remove the container with the same name:

```bash
docker stop my_container
docker rm my_container
```

### 4. Push the Docker image to a registry

Once you have built the Docker image, you can push it to a registry so that it can be used by others.

#### Docker Hub

To push the image to Docker Hub, you need to:

1. Create an account on the [Docker Hub website](https://hub.docker.com/).
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

To pull the image from Docker Hub, you can use the following command:

```bash
docker pull docker_hub_username/random-matrix-generator:latest
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

Note that you must be either the owner of the repository `your_repo` or have at least the `Maintainer` role to be able to push images to the repository.

To pull the image from the CyVerse container registry, you can use the following command:

```bash
docker pull harbor.cyverse.org/your_repo/random-matrix-generator:latest
```

## Exporting the Docker image to a tar file

If you want to export the Docker image to a tar file, you can use the `docker export` command.
The `docker export` command is used to export the contents of a container's filesystem as a tar archive.

A common use case for this is to export the container's filesystem to a tar file so that it can be used to import a WSL (Windows Subsystem for Linux) distribution.
To do this, you need to first run the container and then use the `docker export` command to export the container's filesystem.

Start by running the container in your WSL environment.

```bash
docker run -t --name my_container ubuntu:22.04 ls /
```

Then, you can use the `docker export` command to export the container's filesystem to a tar file:

```bash
docker export my_container > /mnt/c/temp/my_container.tar
```

This will create a tar file named `my_container.tar` in the `/mnt/c/temp/` directory, which is accessible from your Windows file system 
(`C:\temp\my_container.tar`).

You can now clean up the container by stopping and removing it:

```bash
docker stop my_container
docker rm my_container
```

To import the tar file into WSL, open the PowerShell or Command Prompt and navigate to the directory where the tar file is located.

```powershell
cd C:\temp
```

Then, create a new directory for the WSL distribution:

```powershell
mkdir C:\wslDistroStorage\my_container
```

Then, you can use the `wsl --import` command to import the tar file into WSL.

```bash
wsl --import my_container C:\wslDistroStorage\my_container C:\temp\my_container.tar
```

This will create a new WSL distribution named `my_container` in the `C:\wslDistroStorage\my_container` directory.

You can now run the WSL distribution using the following command:

```bash
wsl -d my_container
```

