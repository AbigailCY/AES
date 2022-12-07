for FILE in 1 2 3 4 41 5 51 6
do
    ./exec/AES_SEQ $FILE
    ./exec/AES_OMPC $FILE
    ./exec/AES_OMPG $FILE
    ./exec/AES_CUDA $FILE
done


for threads in 4 8 12 16 20 24 28 32 40 50 60 80 100 200 300 400 500 600
do 
    nvprof ./exec/AES_OMPC 5 $threads
    nvprof ./exec/AES_OMPG 5 $threads
done  