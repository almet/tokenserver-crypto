#include "cryptopp/hkdf.h"

using namespace CryptoPP;
using namespace std;

namespace tokencrypto {
	byte* generateRandomSalt(int size);
	byte* deriveKey(int size, byte* ikm, int ikm_size, byte* salt, int salt_size);
	void writeFile(string filename, byte* content, int size);
	byte* readFile(string filename);
}
