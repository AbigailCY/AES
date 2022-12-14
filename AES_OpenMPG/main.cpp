/*********************************************************************/
/*                       INCLUDES AND DEFINES                        */
/*********************************************************************/

// #include "cuda_runtime.h"
// #include "device_launch_parameters.h"
// #include <cuda.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <string>
#include <omp.h>

#include "Helper.h"
#include "AES.h"
// #include "timer.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

// #define THREADS_PER_BLOCK 1024
#define ROUNDS 10

/*********************************************************************/
/*                       COUNTER MODE FUNCTIONS                      */
/*********************************************************************/

void counter_launch_kernel(unsigned char *messages, unsigned char *results, unsigned char *keys, const unsigned int &message_size)
{
	double seconds = 0.0;
	double start_time, end_time, exec_time;

	for (int i = 0; i != ROUNDS; ++i)
	{
		start_time = omp_get_wtime();

		aes_encryption (messages, results, keys, message_size);

		end_time = omp_get_wtime();
    	exec_time = end_time - start_time;
		seconds += exec_time;
	}

	cout << seconds * 1000.0/ ROUNDS << endl;
	seconds = 0.0;
}

/*********************************************************************/
/*                        MAIN FUNCTION CALL                         */
/*********************************************************************/

int main(int argc, char* argv[])
{
	// for (int i = 6; i > 0; i--)
	// {
	int i = argv[1]? atoi(argv[1]) : 1;
	int num_threads = argv[2] ? atoi(argv[2]) :20;
	omp_set_num_threads(num_threads);

	#pragma omp parallel
	{
	#pragma omp single
	cout << "Number of threads: "<<omp_get_num_threads() << endl;
	}

	cout << "Text" << i<< endl;
	string file_path_key = "../key.txt";
	string file_path_messages = "../text" + std::to_string(i) + ".txt";
	int filesize = file_size(file_path_messages.c_str());

	// Load data from files
	unsigned char *key = read_key(file_path_key);
	unsigned char *keys = key_schedule(key);

	cout << std::dec << "Starting AES CUDA - COUNTER MODE" << endl;
	cout << "filesize: " << filesize << endl;

	// Read in data
	unsigned char * plaintexts = (unsigned char *)malloc(sizeof(unsigned char)*filesize);
	read_datafile(file_path_messages.c_str(), plaintexts);

	// Malloc Memory for Enc/Decrypted Solutions
	unsigned char *decrypted_solution;
	unsigned char *encrypted_solution;

	decrypted_solution = new unsigned char[filesize];
	encrypted_solution = new unsigned char[filesize];

	// cout << endl << "Ready to start!" << endl << endl;

	// Starting Encryption
	cout << endl << "Starting AES CUDA - COUNTER MODE KERNEL " << endl;
	counter_launch_kernel(plaintexts, encrypted_solution, keys, filesize);
	counter_launch_kernel(plaintexts, encrypted_solution, keys, filesize);

	// Starting Decryption
	cout << endl << "Starting AES CUDA - INVERSE COUNTER MODE KERNEL " << endl;
	counter_launch_kernel(encrypted_solution, decrypted_solution, keys, filesize);

	// Checking if Decryption of Encryption is the plaintext
	cout << endl << "Legit solution: " << check_byte_arrays(plaintexts, decrypted_solution, filesize) << endl;
	// }

	return 0;
}