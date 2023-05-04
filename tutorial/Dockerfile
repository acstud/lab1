FROM nvidia/cuda:11.8.0-cudnn8-devel-ubuntu22.04
 
RUN apt-get -y update
RUN apt-get install -y nano git cmake cuda-nsight-systems-11-8 cuda-nsight-compute-11-8 openssh-server
RUN mkdir labs
RUN git clone https://github.com/acstud/lab1.git /labs/lab1
RUN git clone https://github.com/acstud/lab2.git /labs/lab2
RUN git clone https://github.com/acstud/lab3-km.git /labs/lab3-km
RUN git clone https://github.com/acstud/lab3-cnn.git /labs/lab3-cnn

RUN  echo 'root:password' | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/g' /etc/ssh/sshd_config
RUN service ssh start
EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]
