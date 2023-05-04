# Use [Docker](https://www.docker.com/) to set up the ACS environment

You can use Docker if you have Nvidia GPU but cannot install Ubuntu.

Docker is a OS-level virtualization tool to deliver software and packages called **containers**. Docker can run on various Linux distributions and Windows (via WSL2).

You can follow this [manual](https://docs.docker.com/engine/install/ubuntu/) to install Docker. Please refer to this [link](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#installing-on-ubuntu-and-debian) to enable Nvidia GPU support.

For the Windows platform, the process is similar, except that you need to enable [WSL2 support](https://docs.docker.com/desktop/windows/wsl/). For Nvidia GPU support, please refer to this [blog](https://www.docker.com/blog/wsl-2-gpu-support-for-docker-desktop-on-nvidia-gpus/).

## Set up the ACS environment

### 1. Build the image
```shell
sudo docker build -t acs:test .
```
You need to execute this command in the folder containing this tutorial because the final argument `.` represents the current directory, which must have a `Dockerfile`. `acs:test` means the image will be put in a repository called `acs`, and the tag is `test`.

### 2. Check the built image
```shell
sudo docker images
```
You should see outputs like below:
```shell
REPOSITORY    TAG                               IMAGE ID       CREATED         SIZE
acs           test                              7d8934ba33a2   4 minutes ago   10.3GB
nvidia/cuda   11.8.0-cudnn8-devel-ubuntu22.04   ccbfa2e86cfa   3 weeks ago     8.37GB
```

### 3. Run a container from the image
```shell
sudo docker run --gpus all --name acs_env -d -it -v ~/docker_share/:/docker_share -p 8822:22 acs:test /bin/bash
```
`-v ~/docker_share/:/docker_share`: create a shared folder (`~/docker_share` on the host and `/docker_share` in the container)

`--name acs_env`: the container name is acs_env

`-p 8822:22`: forward the container port 22(ssh port) to 8822 on the host.

`-it`: starts an interactive terminal.

### 4. Check the container status
```shell
sudo docker ps -a
```
You should see something like below.
```shell
CONTAINER ID   IMAGE      COMMAND       CREATED          STATUS          PORTS                                   NAMES
8f468c97ca53   acs:test   "/bin/bash"   10 minutes ago   Up 10 minutes   0.0.0.0:8822->22/tcp, :::8822->22/tcp   acs_env
```

### 5. Connect to the container terminal
```shell
sudo docker exec -it acs_env /bin/bash
```
Check `cmake`, `nvcc`, `g++`, `nvidia-smi`, `nsys` are working.

### 6. Enable ssh server
In the container terminal, type ```service ssh restart``` to start ssh server.

Now on the host side, you can start to the container ssh session.

```shell
ssh root@127.0.0.1 -p 8822
```

The username is always `root` and the password is `password`.

You can now set up CLion remote debugging.

### 7. Sharing files
The lab code template is available in `/labs`(in container). You can also copy your code to `~/docker_share`(on the host side) to compile it in the container(`/docker_share`). (sudo required because the container is started by sudo).




# Docker cheatsheet
|   |command|
|---|---|
|list all images|`sudo docker images`|
|remove an image|`sudo docker image rm {IMAGE_ID}`|
|list all containers|`sudo docker ps -a`|
|stop a container|`sudo docker stop {CONTAINER_ID}`|
|remove a container|`sudo docker rm {CONTAINER_ID}`|

|   |command(ACS specific)|
|---|---|
|Build the ACS image|`sudo docker build -t acs:test .`|
|Run the container|`sudo docker run --gpus all --name acs_env -d -it -v ~/docker_share/:/docker_share -p 8822:22 acs:test /bin/bash`|
|Connect to the container|`sudo docker exec -it acs_env /bin/bash`|
|Get the container IP address|`sudo docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' acs_env`|