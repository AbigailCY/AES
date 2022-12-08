# AES encryption algorithm

AES algorithm in cuda and CPU/GPU openmp

Developed based on https://github.com/franneck94/CUDA-AES

Book from http://www.crypto-textbook.com/ Chapter4

## Usage

All the tests are under NYU Greene cluster. All makefiles are well-written in the subdirectories.

Note that there are some sample text files to test, so `text_file_number` below can be `1,2,3,4,41,5,51,6,61`. Default set to `1`.

1. AES_CUDA compile:
```
ssh burst
# distribute a GPU node
srun --account=csci_ga_3033_025-2022fa --partition=n1s8-v100-1 --gres=gpu:v100:1 --time=00:40:00 --pty /bin/bash 
# Use a singularity image
/share/apps/gcc/run-cuda-11.6.bash
# cd to the AES_CUDA directory
make
```
Usage:
```
./AES_CUDA text_file_number

#use nvprof to profile GPU utilization:
nvprof ./AES_CUDA text_file_number
```

2. AES_SEQ, AES_OMPC, AES_OMPG compile:
```
ssh burst
# distribute a GPU node
srun --account=csci_ga_3033_025-2022fa --partition=n1s8-v100-1 --gres=gpu:v100:1 --time=00:40:00 --pty /bin/bash 
# Use a singularity image (different than above)
/share/apps/gcc/run-gcc-offload-nvptx-12.2.0.bash
# cd to the AES_() directory
make
```

Usage:
```
./AES_SEQ text_file_number
./AES_OMPC  text_file_number num_threads
./AES_OMPG  text_file_number num_threads
```

3. The unit tests are in `AES/exec/run.sh`, the output is in `AES/exec/out.txt`, and the executables in NYU Greene cluster GPU are in `AES/exec/`.
```
sh run.sh > out.txt 2>&1 &
```
