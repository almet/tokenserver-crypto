#include "cryptopp/hkdf.h"

using namespace CryptoPP;
using namespace std;

namespace tokencrypto {
    const int DEFAULT_SALT_SIZE = 128;
    const int DEFAULT_KEY_SIZE = 128;

	/**
	 * Generates a random salt of the specified lenght.
	 * 
	 * @param int the number of bytes to generate
	 *
	 * @return byte* the generated salt, in bytes
	 **/
	byte* generateRandomSalt(int size);

	/**
	 * Derivate a key using the HKDF algorithm.
	 *
	 * The underlying algorithm used is SHA256.
	 *
	 * @param byte* the adress of the input keyring material
	 * @param byte* the adress of the salt to use to derivate the key
	 * @param int the size of the key to output
	 * @param byte* the adress of some additionale info to use while derivating
	 *              the key.
	 *
	 * @return byte* the derivated output keyring material
	 **/
	byte* deriveKey(int size, byte* ikm, int ikm_size, byte* salt, int salt_size);

	/**
	 * Write the specified content into filename.
	 *
	 * @param string path to the file to write to
	 * @param byte* a pointer to the content to write to the file
	 * @param int the size of the content
	 **/
	void writeFile(string filename, byte* content, int size);

	/**
	 * Reads a key from the filesystem and return it's content in bytes
	 *
	 * @param string the name of the file to read from the disk
	 *
	 * @return byte* the content of the file, in bytes
	 **/
	byte* readFile(string filename);
}
