/*********************************************************************/
/*                       INCLUDES AND DEFINES                        */
/*********************************************************************/

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
#include <inttypes.h>

#include "Helper.hpp"
#include "Mode.hpp"
#include "AES.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;

// #define NUM_THREADS 20
#define ROUNDS 10

/*********************************************************************/
/*                     COUNTER MODE FUNCTIONS                        */
/*********************************************************************/

// Increment Counter
ByteArray increment_counter(const ByteArray &start_counter,
							const unsigned int &round)
{
  /* Assuming start_counter will be at most of size 8 Byte    
   */
  int64_t ctr_converted = 0x00;
  unsigned int ctr_size = start_counter.size();
  ByteArray result(ctr_size, 0x00);

  for (unsigned int i = 0; i<ctr_size; ++i) 
    ctr_converted += (int64_t)(start_counter[start_counter.size()-i-1]) << 8*i   & (int64_t)0xFF<<8*i;
  
  ctr_converted = ctr_converted + (uint64_t)round;
  
  for (unsigned int i = 0; i<ctr_size; ++i) {
    result[i] += (ctr_converted >> (ctr_size-1-i)*8) & (int64_t)0xFF;
  }

  return result;
}

// Generate Counters for all Rounds
void generate_counters(vector<ByteArray> &ctrs, const ByteArray &IV)
{
	ByteArray start_counter(KEY_BLOCK - IV.size(), 0x00);
	ByteArray ctr_i(KEY_BLOCK - IV.size(), 0x00);
	ByteArray res(KEY_BLOCK, 0x00);
	size_t i = 0;
	
	for (i = 0; i < ctrs.size(); ++i)
	{	       
		res = IV;
		  
		if (i > 0)
		{
			ctr_i = increment_counter(start_counter, i);
		} 

		res.insert(res.end(), ctr_i.begin(), ctr_i.end());
		ctrs[i] = res;
	}
}

// Execute the Counter Mode for all Message Blocks
const vector<ByteArray> counter_mode(const vector<ByteArray> &messages,
									const ByteArray &key,
									const ByteArray &IV)
{
	AES aes(key);
	vector<ByteArray> encrypted_messages(messages.size(), vector<unsigned char>(KEY_BLOCK, 0x00));
	vector<ByteArray> ctrs(messages.size(), vector<unsigned char>(KEY_BLOCK, 0x00));
	generate_counters(ctrs, IV);
	int i = 0;

	// Starting Timers and Counter Mode for Encryption
	float microseconds = 0.0f;

	cout << endl << "OpenMP - Encrypted Duration  ";

	for (int r = 0; r != ROUNDS; ++r)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		
		// #pragma omp target data map(from: encrypted_messages) map(to: aes, ctrs,messages )
		// {
		// 	#pragma omp target
		// 	#pragma omp parallel for private(i)
			for (i = 0; i < messages.size(); ++i)
			{
				encrypted_messages[i] = XOR(aes.encrypt(ctrs[i]), messages[i]);
			}
		// }

		auto end_time = std::chrono::high_resolution_clock::now();
		auto time = end_time - start_time;
		microseconds += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
	}

	cout << microseconds / (1000.0f * ROUNDS) << endl;
	microseconds = 0.0f;


	return encrypted_messages;
}

// Execute the Inverse Counter Mode for all Decrypted Message Blocks
const vector<ByteArray> counter_mode_inverse(const vector<ByteArray> &encrypted_messages,
											const ByteArray &key,
											const ByteArray &IV)
{
	AES aes(key);
	vector<ByteArray> decrypted_messages(encrypted_messages.size(), vector<unsigned char>(KEY_BLOCK, 0x00));
	vector<ByteArray> ctrs(encrypted_messages.size(), vector<unsigned char>(KEY_BLOCK, 0x00));
	generate_counters(ctrs, IV);
	int i = 0;

	// Starting Timers and Counter Mode for Encryption
	// float microseconds = 0.0f;
	double microseconds = 0.0;
	double start_time, end_time, exec_time;

	// for (int t = 2; t != NUM_THREADS; t += 2)
	// {
	cout << endl << "OpenMP  - Decrypted Duration  ";

	for (int r = 0; r != ROUNDS; ++r)
	{
		// auto start_time = std::chrono::high_resolution_clock::now();
		start_time = omp_get_wtime();

		#pragma omp target data map(from: decrypted_messages) map(to: aes, encrypted_messages, ctrs)
		{
			#pragma omp target
			#pragma omp parallel for private(i)
			for (i = 0; i < encrypted_messages.size(); ++i)
			{
				decrypted_messages[i] = XOR(aes.encrypt(ctrs[i]), encrypted_messages[i]);
			}

		}

		// auto end_time = std::chrono::high_resolution_clock::now();
		// auto time = end_time - start_time;
		// microseconds += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
		end_time = omp_get_wtime();
    	exec_time = end_time - start_time;
		microseconds += exec_time;
		
	}

	cout << microseconds * 1000.0/ ROUNDS << endl;
	microseconds = 0.0;
	// }

	return decrypted_messages;
}
