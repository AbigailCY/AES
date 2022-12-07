#pragma once
/*********************************************************************/
/*                       INCLUDES AND DEFINES                        */
/*********************************************************************/

#include <vector>
#include <iostream>

#include "Table.h"

#define AES_BITS 128
#define NUM_ROUNDS 10
#define SUB_KEYS (NUM_ROUNDS + 1)
#define KEY_BLOCK 16

using std::vector;


/*********************************************************************/
/*                          DEVICE DECLARATIONS                      */
/*********************************************************************/


void byte_sub(unsigned char *block_biffer, unsigned char *sharedSbox);
void shift_rows(unsigned char *block_biffer);
void mix_columns(unsigned char* column, unsigned char* mult_2, unsigned char* mult_3);
void key_addition(unsigned char *block_biffer, unsigned char *key, const unsigned int &round);

/*********************************************************************/
/*                          KERNEL DECLARATIONS                      */
/*********************************************************************/


void aes_encryption(unsigned char *message, unsigned char *result, unsigned char *keys, unsigned int width);

/*********************************************************************/
/*                        HOST KEY FUNCTIONS                         */
/*********************************************************************/

unsigned char* key_schedule(unsigned char *key);
unsigned char* sub_key128(unsigned char *prev_subkey, const int &r);