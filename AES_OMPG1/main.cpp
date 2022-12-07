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
	// // Define launch config
	// int chunks = filesize / KEY_BLOCK;
	// int ThreadsPerBlock = THREADS_PER_BLOCK;
	// int Blocks = ceil(chunks / ThreadsPerBlock);

	// // Messages to device memory
	// unsigned char *d_messages;
	// gpuErrchk(cudaMalloc((void **)&d_messages, message_size * sizeof(unsigned char)));
	// gpuErrchk(cudaMemcpy(d_messages, messages, message_size * sizeof(unsigned char), cudaMemcpyHostToDevice));

	// Results to device memory
	// unsigned char *d_results;
	// gpuErrchk(cudaMalloc((void **)&d_results, message_size * sizeof(unsigned char)));
	// gpuErrchk(cudaMemcpy(d_results, messages, message_size * sizeof(unsigned char), cudaMemcpyHostToDevice));

	// SBOX to device memory
	// unsigned char *d_sbox;


	// Subkeys to device memory
	// unsigned char *d_keys;
	// gpuErrchk(cudaMalloc((void **)&d_keys, NUM_ROUNDS * KEY_BLOCK * sizeof(unsigned char)));
	// gpuErrchk(cudaMemcpy(d_keys, keys, NUM_ROUNDS * KEY_BLOCK * sizeof(unsigned char), cudaMemcpyHostToDevice));
	double microseconds = 0.0;
	double start_time, end_time, exec_time;

	for (int i = 0; i != ROUNDS; ++i)
	{
		start_time = omp_get_wtime();

		aes_encryption (messages, results, keys, message_size);

		end_time = omp_get_wtime();
    	exec_time = end_time - start_time;
		microseconds += exec_time;
	}

	cout << microseconds * 1000.0/ ROUNDS << endl;
	microseconds = 0.0;

	// gpuErrchk(cudaMemcpy(results, d_results, message_size * sizeof(unsigned char), cudaMemcpyDeviceToHost));
	// gpuErrchk(cudaFree(d_results));
	// gpuErrchk(cudaFree(d_messages));
	// gpuErrchk(cudaFree(d_keys));
}

/*********************************************************************/
/*                        MAIN FUNCTION CALL                         */
/*********************************************************************/

int main()
{
	for (int i = 6; i > 0; i--)
	{
		cout << endl << "Text" << i;
		string file_path_key = "../key.txt";
		string file_path_messages = "../text" + std::to_string(i) + ".txt";
		int filesize = file_size(file_path_messages.c_str());

		// Load data from files
		unsigned char *key = read_key(file_path_key);
		unsigned char *keys = key_schedule(key);

		cout << endl << std::dec << "Starting AES CUDA - COUNTER MODE" << endl;

		// Read in data
		unsigned char * plaintexts = (unsigned char *)malloc(sizeof(unsigned char)*filesize);
		read_datafile(file_path_messages.c_str(), plaintexts);

		// Malloc Memory for Enc/Decrypted Solutions
		unsigned char *decrypted_solution;
		unsigned char *encrypted_solution;

		decrypted_solution = new unsigned char[filesize];
		encrypted_solution = new unsigned char[filesize];

		cout << endl << "Ready to start!" << endl << endl;

		// Starting Encryption
		cout << endl << "Starting AES CUDA - COUNTER MODE KERNEL " << endl;
		counter_launch_kernel(plaintexts, encrypted_solution, keys, filesize);

		// Starting Decryption
		cout << endl << "Starting AES CUDA - INVERSE COUNTER MODE KERNEL " << endl;
		counter_launch_kernel(encrypted_solution, decrypted_solution, keys, filesize);

		// Checking if Decryption of Encryption is the plaintext
		cout << endl << "Legit solution: " << check_byte_arrays(plaintexts, decrypted_solution, filesize) << endl;
	}

	return -1;
}