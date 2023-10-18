# Parallel Bucketsort with OpenMP
by 6488069 Chattawat Riyong
> This program is part of ITCS443 Parallel and Distributed System.  
> Faculty of Information and Communication Technology, Mahidol University.  
> Semester 1/2023  

## How to run?
There are 2 files  
* __bucketSort.c__  
This is the source file written in C. You can compile this file with  
`gcc -lm -fopenmp bucketSort.c -o bucketSort`  
You can also specify C standard in case of standard warning by adding `-std=c99`
* __bucketSort__ (No file extension)  
This is the executable compiled in ICT Cluster with above arguments.
The program accept 2 arguments which are `Number of Thread` and `Amount of Integer`  
`./bucketSort <number of thread> <amount of integer>`  
For example: `./bucketSort 4 1000000` (4 threads and 1 million integers)