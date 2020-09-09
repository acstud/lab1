#Make chnages, update or add any new source file in students/ folder for Lab-1 assignment

#To submit your students/ repo to BrightSpace, do the following

#compress your students/ code repo
zip -r -q GROUP_01_alee_bzhang_cwang students/
#change the extension from *.zip to *.txt and upload this *.txt file to BrightSpace
cp GROUP_01_alee_bzhang_cwang.zip GROUP_01_alee_bzhang_cwang.txt
 
# We recommend to check/verify if your code is being compiled properly and running as expected by following script. 
#This script will be used to evaluate your results.

#change the extension from *.txt to *.zip 
cp GROUP_01_alee_bzhang_cwang.txt GROUP_01_alee_bzhang_cwang.zip
#copy this zip file to lab1/app/src/acsmatmult repo
cp -r GROUP_01_alee_bzhang_cwang.zip ~/lab1/app/src/acsmatmult
#uncompress the *.zip file
cd ~/lab1/app/src/acsmatmult && unzip GROUP_01_alee_bzhang_cwang.zip

#verify your code 
mkdir ~/lab1/app/debug && cd ~/lab1/app/debug
#cmake or cmake3
cmake .. 
make
./acsmatmult -a


