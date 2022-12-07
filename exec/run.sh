for FILE in 1 2 3 4 41 5 51 6 61
do
    ./AES_SEQ $FILE
    ./AES_OMPC $FILE
    ./AES_OMPG $FILE
    ./AES_CUDA $FILE
done


for threads in 4 8 12 16 20 24 28 32 40 50 60 80 100 200 300 400 500 600
do 
    ./AES_OMPC 51 $threads
    nvprof ./AES_OMPG 51 $threads
done  

nvprof ./AES_CUDA 51
