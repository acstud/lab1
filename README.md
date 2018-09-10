# Advanced Computing Systems 2018-2019 Lab 1 - Matrix Multiplication

This repository contains all files and explanations needed to perform
the first lab of the EE4C07 - Advanced Computing Systems course at the
Delft University of Technology.

## What do I have to do for Lab 1?

Quick version:

* Download the baseline project.
* Implement the matrix multiplication functions in the baseline project, 
  using the following techniques:
  * SIMD extensions (AVX)
  * Multi-core (OpenMP)
  * GPU Accelerator (OpenCL)
* Implement the experiments that benchmark these functions.
* Make sure all tests pass.
* Benchmark your solution.
* Write a report.
* Turn in your report.
* Rejoice with the knowledge that you have gained.

The long version:

The questions below can be read as a sequence, and they will become a
sort of lab manual!

## What preliminary skills should I have to complete this lab?

* You need to be proficient in C/C++.
* You need to be able to use the GNU/Linux command-line (bash).
* You need to be able to use LaTeX to write a report.

These are not only skills to help you in this course, but they are used in
many master courses. If you miss any of them, it is recommended to learn
them as soon as possible.

### Help! I cannot C/C++!
If you have never written applications using C/C++, it is highly likely that
you will find this course too difficult to complete. You might want to take 
some bachelor courses in programming C/C++ before attempting this course.
You should at least be able to work with pointers and dynamic allocation
functions such as `malloc` and `free`.

### Help! I cannot GNU/Linux command line (bash)!
While there are numerous tutorials on how to use the GNU shell (bash), it might
be helpful to keep 
[a cheat-sheet](https://github.com/LeCoupa/awesome-cheatsheets/blob/master/languages/bash.sh)
at hand.

### Help! I cannot LaTeX!
Very nice open-source books about LaTeX
[exist](https://en.wikibooks.org/wiki/LaTeX).

### Help! I do not have a GNU/Linux distribution on my laptop/computer.
It's a very good idea to get your computer to dual boot a GNU/Linux distribution.
[Ubuntu](https://www.ubuntu.com/download/desktop)
is a good distribution for beginners with a large userbase that have probably
asked and answered all questions you can think of already somewhere online.

#### I am extremely stubborn and I do not want to use GNU/Linux!

[MobaXTerm](https://mobaxterm.mobatek.net/) is a  pretty good terminal client
for Windows.

## Can I fork this repository as a student?

Answer: __do not fork it (publicly)__

#### Why not? This is ridiculous!

According to examination rules and regulations, if you made it possible to 
commit fraud, you are partially responsible. Therefore, if you fork this 
repository publicly and you commit changes to it, and other students plagiarize
from your repository, you are partially responsible for the fraud.

We didn't make those rules, but let's respect them to not get into trouble!

## What is AVX?
AVX stands for Advanced Vector Extensions. They are additional instructions
to the x86 ISA to perform operations on vectors. For example, they make it
possible to not just add two single-precision floating point numbers with one
instruction, but multiple single-precision floating point numbers with one
instruction, at the same time. This can speed up all sorts of algorithms 
begin run on a CPU. These extensions are commonly found in microprocessors used
in laptops to workstations to HPC clusters. It is important to know that they 
are available and how to use them.

## What is OpenMP?
OpenMP stands for Open Multi-Processing. It is a set of compiler directives and
libraries to make it easy to write multi-threaded programs. Because nowadays
many microprocessors have multiple CPU cores in them, it is important to know
how to use OpenMP and that it can help you to quickly parallelize parts of your
program.

## What is OpenCL?
OpenCL stands for Open Compute Language. This is a standard that is meant to 
provide the means of writing portable programs for heterogeneous systems, such
as computers that have a GPU or FPGA. A similar proprietary framework called
CUDA exists. CUDA is very similar to OpenCL, although it runs well mainly on 
NVIDIA GPUs. While OpenCL enjoys less matured tooling for NVIDIA GPUs itself,
more matured tools are available for other types of platforms, such as FPGAs.

## Help! I don't have a GPU / CPU with AVX support / multicore processor.

For this lab, all benchmarks must be performed on two nodes of
[a cluster managed by the INSY department](http://insy-login.hpc.tudelft.nl/).
It is recommended to read the cluster documentation before working on the 
cluster.

The names of these nodes are:

```
insy-node05
insy-node06
```

Although possible, you do not necessarily have to debug all parts of your 
application on these machines. It is recommended to first functionally test and
debug your application on a local machine before benchmarking on these nodes.

Most modern laptops, for example, have vector extensions with multiple cores,
so you should at least be able to implement the vector / AVX / SIMD functions 
and the OpenMP functions. You might not have a GPU in your laptop, but still 
most modern Intel and AMD CPUs can be used to run OpenCL kernels as well.

## How do I install OpenCL on my own computer?

There are numerous implementations of OpenCL. Pick one that corresponds to 
your available hardware.

* [Intel](https://software.intel.com/en-us/articles/opencl-drivers)
* [AMD](https://www.amd.com/en-us/solutions/professional/hpc/opencl)
* [NVIDIA](https://developer.nvidia.com/opencl)

## How do I connect to the GPU cluster?

Log in to the `student-linux.tudelft.nl`:

```console
ssh <netid>@student-linux.tudelft.nl
```
From there, log in to __one__ of the login nodes:

```console
ssh insy-login
ssh sanger
```

While the course is busy and many people are building, you might want to switch
to one or the other login server. However, the Sanger node is a bit older, and
has no support for AVX. You can still build programs that use AVX on it, 
though.

## What sort of applications am I allowed to run on the cluster?

* You are __only allowed to run jobs that are directly related to the course__
and your studies.

* If you run anything unrelated, such as e.g. cryptocoin miners or video 
renderers, __you and your group__ will, without warning:
  * immediately fail the course without any chance of resit.
  * be revoked of access to the cluster.
  
* If you think you do need to run something that might appear suspicious, and 
you are not sure if your application is allowed, it is your responsibility to 
ask first.

## Do I have root access on the cluster nodes / why can't I use sudo?

No, nee, nein, non, 没有, tidak, Không, Όχι, ne, hakuna, yok, 아니, لا, engin, 
Нет, nu, nie, etc..

You will not have root access or superuser rights. If you are reading tutorials
that tell you to "sudo" something, it is __absolutely useless__ to try and
execute that command. Please refrain from using it, as it might spam the 
system administrator inbox.

## What node am I on right now?

Through multiple / nested SSH sessions it could become confusing which node
you are actually on at the moment. Use this to check out which node you are on:

```console
hostname
```

## How to enable GPU support on a node?

GPU support on the nodes is enabled through the use of Environment Modules.
[Environment Modules](http://modules.sourceforge.net/) can be used to quickly
set up your system environment to, for example, change versions of software
packages, etc...
You can enable GPU support to build your programs even on the login servers,
although they do not have a GPU themselves (so running your programs won't 
work).

### Detecting available modules

```console
module use /opt/insy/modulefiles
module avail
```

This will detect the available Environment Modules. It is recommended to use
the latest CUDA version for this lab (9.2 at the time of writing).

### Loading the CUDA/OpenCL module
```console
module use /opt/insy/modulefiles
module load cuda/<version>
nvcc --version
```

## How do I install CMake?

To build the baseline project, you need a 3.10+ version of CMake. This is not
installed on the cluster by default.

### Build CMake

When you are logged in in your home directory on the cluster login node, 
you can install CMake as follows:

* Download CMake 3.10 or higher and untar it. Example:

```console
wget https://cmake.org/files/v3.12/cmake-3.12.1.tar.gz
tar xf cmake-3.12.1.tar.gz
```

* Bootstrap CMake using the bootstrap script, while using your home directory
`~/usr` as a prefix.

```
mkdir -p usr
cd cmake-3.12.1/
./bootstrap --prefix=~/usr -- -DCMAKE_BUILD_TYPE:STRING=Release
```

* Build the executable. This might take a while:

`make`

* Now that the sources have been compiled, you can install it in ~/usr by 
running:

`make install`

* Add the `~/usr/bin path` to your `PATH` environment variable.

`export PATH=~/usr/bin:${PATH}`

If you append this line to `~/.bash_profile`, you do not need to type this 
command after a fresh login.

* Test to see if you have the right CMake version.

`cmake --version`

## How do I edit files on the command line?

The default cluster installation comes with `nano` and `vim`.

## Is there any good IDE to write C/C++?

If you like IDEs, [CLion](https://www.jetbrains.com/clion/) is by far the best
IDE out there at the moment. You can get it for free as a student, if you sign
up with your TU Delft e-mail account.

## Where do I get the baseline project?

```console
git clone https://github.com/acstud/lab1.git
```

## How do I compile and run the baseline project?

A CMake script has been supplied with the baseline project.
If you have the baseline project in some directory, and you are in that
directory, you can run the following:

```console
mkdir debug
cd debug
cmake ..
```

This will create a build directory, go into that directory, and lets CMake 
create the project files there. Note that the CMake script will exclude files
from the build where you are supposed to implement technologies that your 
systems doesnt have any support for (it will complain about this on the 
command line).

Now you can build it with:

```console
make
```

And run it with

```console
./matmult-benchmark -h
```

To show help information.

## What should I do with the baseline project?

You should read the baseline source code and figure out how to program works.
The first thing you must benchmark for your report is the baseline vector
multiplication (also see the [the LaTeX report template](report/template.tex)).

Then, you must:
 
* Implement an experiment for matrix multiplication.

And implement experiments as well after completing each of the following:

* Implement matrix multiplication using SIMD instructions (AVX).
* Implement matrix multiplication on multiple cores using OpenMP.
* Implement matrix multiplication on a GPU using OpenCL.

## What will you run to test if I've implemented everything correctly?

`matmult-benchmark -t`

However, be aware that we will only copy the [baseline/src/students](baseline/src/students)
back into our own baseline project. So anything you change outside that
file will no work for us. Sorry!

## Where are the files that I have to implement?

You can find them here:

[baseline/src/students](baseline/src/students)

## Can I change any other files?

You can do so, but it should not be necessary, unless you find any bugs.
If you find any, please let us know, and we will try to update the baseline
projects as soon as possible.

Other than that, if you change any of the files, you should explain in your
report why you did so.

## What are good resources to have at hand during the lab?

__HIGHLY RECOMMENDED__ resources are:

* [Intel Intrinsics Guide](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)
    * This link will take you to the Intel intrinsics guide. This is a 
    searchable library of all the x86 intrinsics that an Intel machine might 
    or might not support.
* [OpenMP API C/C++ Syntax Reference Guide](https://www.openmp.org/wp-content/uploads/OpenMP-4.5-1115-CPP-web.pdf)
    * This is a handy cheat sheet. You can also discover a lot of functions
    that OpenMP supplies here. 
* [OpenCL API docs](https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml)
    * We use OpenCL 1.2 in the lab. This links will bring you to the API docs.
    Here you can read the interface specification of various OpenCL functions.
* [OpenCL API 1.2 reference card](https://www.khronos.org/files/opencl-1-2-quick-reference-card.pdf)
    * This is another handy cheat sheet to get a quick overview of all the
    functions that you can use in OpenCL.
    
## Help! Some bug in the baseline project is preventing me from completing the lab!

While we have tested the deployment of the baseline project, it is possible
that some bugs exist, as the lab is relatively new, and this year is the first
time we use the INSY cluster. Before you report this bug, try to make sure that
it's an actual bug in the baseline and not in your own code.

Once you have done that, do not panic. The lab instructors are reasonable
people. Let us know as soon as possible. We will try to fix the bug and
if it's something really serious, we may even postpone a deadline (although
you should never count on it).

## How to get CMake to detect OpenCL on the cluster nodes?

In the CMake build scripts called [CMakeLists.txt](baseline/CMakeLists.txt), some scripts 
are included that attempt to find OpenCL support. Once you've loaded the CUDA 
module, CMake will probably still not detect OpenCL.

If CMake cannot find OpenCL, the CMake script will complain about it as 
follows: "OpenCL could not be found, disabling OpenCL functions."

Also, it will not include the OpenCL examples in your build. This is done such
that you can still compile and run the application if you are on a system that
doesn't have OpenCL support. For example, when you are debugging the AVX or 
OpenMP part on your laptop.

On the cluster, the script is able
to properly find CUDA, but not OpenCL. We have to help it a little bit by 
pointing it to the right directories.

```console
cmake .. -DOpenCL_INCLUDE_DIR=/opt/insy/cuda/<version>/targets/x86_64-linux/include \
         -DOpenCL_LIBRARY=/opt/insy/cuda/<version>/targets/x86_64-linux/lib/libOpenCL.so
```

## How do I submit a job to one of the GPU cluster nodes?

The cluster uses SLURM to manage the cluster resources and to schedule jobs on
cluster nodes. You can use the "sbatch" command to submit jobs. Usually, these
jobs are bash scripts. 

When you are on the cluster, you can submit jobs using the sbatch command.
An more detailed explanation is recommended reading material, and can be found
[here](http://insy-login.hpc.tudelft.nl/).

You __must__, however, use a specific partition (a group of nodes, let's say) and
a specific quality-of-service (a set of requirements on the resources you can
request from the cluster).

These are:
```console
--partition=ee4c07
--qos=ee4c07
```

When you want to submit a script to SLURM, you must always supply these
arguments to sbatch (or you must put them in a job script, see below).

Example:

```console
sbatch --partition=ee4c07 --qos=ee4c07 <job script.sh>
```

## Where is the output?

In a file called slurm-<jobid>.out. You can easily show the output using, 
for example:

```console
cat slurm-1234.out
```

## Do you have a template for a job script?
```console
#!/bin/sh
#SBATCH --partition=ee4c07
#SBATCH --qos=ee4c07
#SBATCH --time=0:01:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=1024
#SBATCH --gres=gpu
#SBATCH --mail-type=FAIL

srun <your command>
```

Please consult the INSY cluster presentation for more information about those options. 
If you don't know what you're doing, do not change anything.

Please be aware that if you are going to run the OpenMP benchmark, you might want to
change to `--cpus-per-task=8`. You cannot get more than 8 cores within this specific
QoS.

## I am editing on my local machine! How do I copy files to the cluster?

Upload the sources to your home directory on `student-linux.tudelft.nl`.
This is in sync with the INSY cluster nodes.

For example, when you are in the lab1 baseline project directory, you can use 
`scp` with the recursive option `-r` to copy the whole `src` directory over at 
once:

```bash
scp -r src <netid>@student-linux.tudelft.nl:~/path/to/your/src
```

Then you can go into a second terminal that is logged in to an INSY cluster
login node, build the sources there and submit a batch job to run it on a 
node with GPU capabilities.

## I am getting tired of typing in the commands, this sucks!

It is recommended to automate various steps of this development flow using 
scripts. Any command that you can type on the command-line, you can also put
in a bash script.

* For example, create a new script with:

```console
vim <script name>.sh
```

* Press insert to start typing in vim.
* Press escape and :wq to write the changes to the file and quit vim.
* Make the script executable by running

```console
chmod +x <script name>.sh
```

* Run the script with: 

```console
./<script name>.sh
```

From now on, the lab will be less tiring!

## Can I see a list of commands that I typed previously?

```console
history
```

You could even pipe it into grep to search your history, example:

```console
history | grep cmake
```

## Is there a template for the lab report?

Yes. This is a [the LaTeX template](baseline/report/template.tex) and it is actually
__mandatory__ to use this template. As you can read in the next question, 
any lab reports that do not use this template are denied. This is to make 
sure we can correct your lab reports quickly. In this way, you will receive
your grade and feedback quickly as well.

## When do I measure the performance and put some graphs in the report?

First of all, whenever it is explicitly stated.

Also, after every significant change/improvement. That means when you 
change something and it only gives a 0.1% improvement, you can just write
about what you've changed, but that it didn't change significantly.

However, if you get over, let's say, 5% improvement, then it is worth to
show new graphs. 

Also if you get _less_ performance but did not initially expect so, 
you may write this down. If you show insight on why you did
not get what you've expected, this is seen as a learning opportunity and
thus it is not met with reduced points. On the contrary, we encourage 
this.

## How do I make plots?

It is recommended to use a decent plotting tool (e.g. not Microsoft Excel or 
LibreOffice Calc). Some good plots can be made with free tools such as matplotlib
in Python. You could also use MATLAB or Scilab.

There is [an IPython Notebook with an example available here](report/plotting_tutorial.ipynb).

## What ranges should I measure in terms of problem size / run-time?

The beginning of your measurement range should be as small as possible, to 
identify any initialization overhead.

The end of your measurement range is arrived when measurements take a 
couple of seconds. Then you are in a range where initialization 
overhead of the different techniques becomes somewhat negligible (__for this lab__!). 
However, be aware that you don't have infinite memory.

You also measure everywhere in between. You _could_ make this very fine grained, but
if this doesn't show any anomalies, please refrain from cluttering your plot
and take bigger steps.


## How do I turn in my report / lab sources?

Only submissions that follow these requirements are accepted:

* __You have used [the LaTeX template](../report/template.tex) to create the report.__

* __The file name of your report is GROUP_XX_netid0_netid1_netid2.pdf__
  * Example: GROUP_01_alee_bzhang_cwang.pdf - accepted.
  * Example: GROUP_13_dnguyen_egarcia_fgonzales.pdf - accepted.
  * Example: New Document by n00bk1ng_94.pdf - denied.
  * Note the use of two positions for the group number.

* __All your source code has been compressed into a tar.gz archive__
  * Please make sure to clean your build directories before archiving.
  
* __The file name of your source code archive is GROUP_XX_netid0_netid1_netid2.tar.gz__
  * Example: GROUP_01_alee_bzhang_cwang.tar.gz - accepted.
  * Example: GROUP_13_dnguyen_egarcia_fgonzales.zip - denied.
  * Example: acslab1.tar.gz - denied.
  
* __They have been uploaded to Brightspace before the deadline has expired.__
  
  * In general, the deadline is entered in Brightspace and it is not 
  possible to upload stuff after the deadline.
  
  * Brightspace was down? E-mail your report on time to 
    [acs-ewi@tudelft.nl](mailto:acs-ewi@tudelft.nl).
    * Example: deadline is Monday 17th of September, 09:00 AM CEST. 
    You have taken ample time to finish the report without having to worry,
    and so your clock shows 08:59:59. You hit the send button. The course
    instructors check the time on the e-mail. It says 09:00:00 AM CEST. 
    The report is on time.
    * Example: deadline is Monday 17th of September, 09:00 AM CEST. 
        You have taken ample time to finish the report without having to worry,
        and so your clock shows 08:59:59. You hit the send button. The course
        instructors check the time on the e-mail. It says 09:00:01 AM CEST. 
        The report is not on time. You will not receive points or feedback.
    * Your e-mail was sent from a TU Delft (student) e-mail account.
        * Example: an e-mail sent from bzhang-1@student.tudelft.nl - accepted.
        * Example: an e-mail sent from 23476324@qq.com - denied.
        * Example: an e-mail sent from n00bk1ng_94@yahoo.com - denied.

## How is the lab graded?

Using your report and a rubric that gives indicators for grades based on
the report. You can find [this rubric here](rubric.pdf). This rubric is
used for all labs.

## What happens if I plagiarize?

Many bad things.

In general, if you plagiarize (read about it in the exam regulations) it
means we cannot grade you personally anymore, as it is not your work.

Therefore, as examinators, we cannot perform our job correctly. Thus, we
must consult the exam committee about the case. Depending on the severity
they can decide to prevent you from taking examinations all the way up to
expel you from the master program!

Sometimes, for source code it is hard to determine when it is plagiarism.
Therefore, observe the following examples:

* You find code that exactly solves one of the parts of the lab. You blindly
copy-paste the code without modifications. After we talk to you about this, 
it seems like you can not explain the code either. You also haven't included
a source. 
  * Our verdict: plagiarism.
  * Reason: it is not your work, so we cannot grade you
  * Consequence: case will be brought before exam committee.
  
* You find code that exactly solves one of the parts of the lab. You blindly
copy-paste the code without modifications. After we talk to you about this, 
it seems like you can not explain the code either. You have included a 
source.
  * Our verdict: not formally plagiarism, as you have stated your source,
   but your grade is zero because you didn't learn anything.
  * Reason: it is not your work, so we cannot grade you; you will not 
    receive any points.
  * Consequence: you will have to do some form of resit and not repeat
  this behaviour.
  
* You find code that partially solves one of the parts of the lab. You 
copy-paste some parts of the code that are useful to your problem and 
apply several modifications. You can explain how the code works through
the report and include your sources of inspiration in the report or in
your source code.
  * Our verdict: well done
  * Reason: you have searched the internet for interesting solutions and
  modified them to work within your own context. You also understand how
  and why it solves your problem.
  * Consequence: nothing, but you might want to upvote the source and 
  buy a StackOverflow t-shirt.

Obviously, similar examples could be given for the report, although 
do not copy any sentences from any sources, without citing them correctly.
Always completely formulate your own sentences, even if your language
skills are not what you wish they would be. We will also give feedback
in terms of language.

## Other questions? Brightspace down? E-mail?

You can e-mail [acs-ewi@tudelft.nl](mailto:acs-ewi@tudelft.nl). 
One of the TA's / course instructors will answer you A.S.A.P.

## Can I talk face-to-face?

Yes, and this is preferred and faster than e-mailing.
You can always drop by (open door policy):

* TA's : HB.10.230/240
* Course Instructor : HB.10.030

However, if you want to make sure that we are available, you should make
an appointment.

## Commonly seen OpenCL error codes

* OpenCL error: -1001
  * Are you running on a node that has a GPU?
