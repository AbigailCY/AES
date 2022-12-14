/*********************************************************************/
/*                       INCLUDES AND DEFINES                        */
/*********************************************************************/

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <string>
#include <omp.h>

#include "Helper.hpp"
#include "AES.hpp"
#include "Mode.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;

int main(int argc, char* argv[])
{
	// Define Variables
	unsigned int iv_length = 12;

	// for (int i = 6; i > 0; i--)
	// {
	int i = argv[1]? atoi(argv[1]) : 1;
	int num_threads = argv[2]? atoi(argv[2]) : 20;
	cout << endl << "Text" << i;
	string file_path_key = "../key.txt";
	string file_path_messages = "../text" + std::to_string(i) + ".txt";

	vector<ByteArray> decrypted_solution;
	vector<ByteArray> encrypted_solution;

	// Load data from files
	ByteArray key = read_key(file_path_key);
	ByteArray IV = random_byte_array(iv_length);
	vector<ByteArray> messages = read_datafile(file_path_messages);

	encrypted_solution = counter_mode(messages, key, IV, num_threads);
	decrypted_solution = counter_mode_inverse(encrypted_solution, key, IV, num_threads);

	cout << endl << "Legit solution: " << check_vector_of_byte_arrays(decrypted_solution, messages) << endl;
	// }

	return 0;
}
