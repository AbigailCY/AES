/*********************************************************************/
/*                       INCLUDES AND DEFINES                        */
/*********************************************************************/

// #include "cuda_runtime.h"
// #include "device_launch_parameters.h"
// #include <cuda.h>
#include <omp.h>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

#include "AES.h"
#include "Helper.h"

using std::cout;
using std::endl;
using std::vector;

//Rijndael multiplication by 2
unsigned char mult_22[256] = 
{
	0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
	0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
	0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
	0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
	0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
	0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
	0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
	0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
	0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
	0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
	0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
	0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
	0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
	0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
	0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
	0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5 
};

//Rijndael multiplication by 3
unsigned char mult_33[256] = 
{
	0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
	0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
	0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
	0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
	0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
	0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
	0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
	0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,
	0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
	0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
	0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,
	0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,
	0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
	0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,
	0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
	0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a 
};

unsigned char IV[KEY_BLOCK] = 
{ 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

unsigned char d_sbox[256] =
{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
	0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
	0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
	0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
	0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
	0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
	0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
	0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
	0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
	0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
	0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
	0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
	0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
	0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
	0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
	0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
	0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

/*********************************************************************/
/*                           MAIN KERNEL                             */
/*********************************************************************/

void aes_encryption(unsigned char *message, unsigned char *result, unsigned char *keys, unsigned int width)
{
	// keysize:  NUM_ROUNDS * KEY_BLOCK; messagesize: width
	int iters = width/KEY_BLOCK;
	if (width%KEY_BLOCK != 0) iters += 1;
	int iter;

	#pragma omp target data map(to:width, keys[0:NUM_ROUNDS * KEY_BLOCK], d_sbox[0:256],IV[0:KEY_BLOCK], mult_22[0:256], mult_33[0:256]) map(tofrom: result[0:width],  message[0:width])
	#pragma omp target teams distribute parallel for private(iter) num_threads(1024)
	for (iter = 0; iter < iters; iter++) {

		int id = iter * KEY_BLOCK;

		short idx, c = 0;
		unsigned char iv_new[KEY_BLOCK];
		unsigned char temp[KEY_BLOCK];

		for (int i = 0; i != KEY_BLOCK; ++i)
		{
			temp[i] = message[id + i];
			iv_new[i] = IV[i];
		}

		// Compute the new IV vector
		for (idx = KEY_BLOCK - 1; idx >= 0; idx--)
		{
			short shift = (KEY_BLOCK - (idx + 1)) * 8;
			unsigned char op1 = IV[idx];
			unsigned char op2 = ((id &(0xff << shift)) >> shift);
			iv_new[idx] = op1 + op2 + c;
			c = (iv_new[idx] > op1 && iv_new[idx] > op2) ? 0 : 1;
		}

		// Starting AES Rounds
		key_addition(iv_new, keys, 0);

		for (int i = 1; i != NUM_ROUNDS; ++i)
		{
			byte_sub(iv_new, d_sbox);
			shift_rows(iv_new);
			mix_columns(iv_new, mult_22, mult_33);
			key_addition(iv_new, keys, i);
		}

		byte_sub(iv_new, d_sbox);
		shift_rows(iv_new);
		key_addition(iv_new, keys, NUM_ROUNDS);

		// XOR the encrypted incremented IV with the message block
		for (int i = 0; i != KEY_BLOCK; ++i)
		{
			unsigned char res = iv_new[i] ^ temp[i];
			iv_new[i] = res;
		}

		for (int i = 0; (i != KEY_BLOCK) && (id + i < width) ; ++i)
		{
				result[id + i] = iv_new[i];
		}
		
	}
}

/*********************************************************************/
/*                      SUB LAYER DEVICE KERNEL                      */
/*********************************************************************/

// Key Addition Kernel
void key_addition(unsigned char *message, unsigned char *key, const unsigned int &round)
{
	for (int i = 0; i != KEY_BLOCK; ++i)
	{
		message[i] = message[i] ^ key[(KEY_BLOCK * round) + i];
	}
}

// byte substitution (S-Boxes)
void byte_sub(unsigned char *message, unsigned char *sharedSbox)
{
	for (int i = 0; i != KEY_BLOCK; ++i)
	{
		message[i] = sharedSbox[message[i]];
	}
}

// Shift rows
void shift_rows(unsigned char *message)
{
	unsigned char j;

	j = message[1];
	message[1] = message[5];
	message[5] = message[9];
	message[9] = message[13];
	message[13] = j;

	j = message[2];
	message[2] = message[10];
	message[10] = j;
	j = message[6];
	message[6] = message[14];
	message[14] = j;

	j = message[15];
	message[15] = message[11];
	message[11] = message[7];
	message[7] = message[3];
	message[3] = j;
}


// Mix column
void mix_columns(unsigned char* column, unsigned char* mult_2, unsigned char* mult_3)
{
	unsigned char i;
	unsigned char cpy[4];

	for (i = 0; i != 4; ++i)
	{
		cpy[i] = column[i];
	}

	column[0] = mult_2[cpy[0]] ^ mult_3[cpy[1]] ^ cpy[2] ^ cpy[3];
	column[1] = cpy[0] ^ mult_2[cpy[1]] ^ mult_3[cpy[2]] ^ cpy[3];
	column[2] = cpy[0] ^ cpy[1]^ mult_2[cpy[2]] ^ mult_3[cpy[3]];
	column[3] = mult_3[cpy[0]] ^ cpy[1] ^ cpy[2] ^ mult_2[cpy[3]];
}

/*********************************************************************/
/*                           KEY FUNCTIONS                           */
/*********************************************************************/

// Computing the round keys
unsigned char* key_schedule(unsigned char *key)
{
	int r;
	unsigned char **subkeys;
	subkeys = new unsigned char*[NUM_ROUNDS];

	for (r = 0; r != SUB_KEYS; r++)
	{
		if (r == 0)
			subkeys[r] = key;
		else
		{
			if (AES_BITS == 128)
				subkeys[r] = sub_key128(subkeys[r - 1], r - 1);
			else
				cout << "TODO! 192-bit and 256-bit not implemented yet." << endl;
		}
	}

	unsigned char *keys;
	keys = new unsigned char[NUM_ROUNDS * KEY_BLOCK];

	for (int i = 0; i != NUM_ROUNDS; ++i)
	{
		for (int j = 0; j != KEY_BLOCK; ++j)
		{
			keys[i * NUM_ROUNDS + j] = subkeys[i][j];
		}
	}

	return keys;
}

// Computing subkeys for round 1 up to 10
unsigned char* sub_key128(unsigned char *prev_subkey, const int &r)
{
	unsigned char *result;
	result = new unsigned char[KEY_BLOCK];
	int i;

	result[0] = (prev_subkey[0] ^ (h_sbox[prev_subkey[13]] ^ RC[r]));
	result[1] = (prev_subkey[1] ^ h_sbox[prev_subkey[14]]);
	result[2] = (prev_subkey[2] ^ h_sbox[prev_subkey[15]]);
	result[3] = (prev_subkey[3] ^ h_sbox[prev_subkey[12]]);

	for (i = 4; i != KEY_BLOCK; i += 4)
	{
		result[i + 0] = (result[i - 4] ^ prev_subkey[i + 0]);
		result[i + 1] = (result[i - 3] ^ prev_subkey[i + 1]);
		result[i + 2] = (result[i - 2] ^ prev_subkey[i + 2]);
		result[i + 3] = (result[i - 1] ^ prev_subkey[i + 3]);
	}

	return result;
}