#include <fstream>

#include "tokencrypto.h"
#include "exceptions.h"
#include "cryptopp/hkdf.h"
#include "cryptopp/sha.h"
#include "cryptopp/osrng.h"

using namespace CryptoPP;
using namespace std;

namespace tokencrypto
{

byte* generateRandomSalt(int size=DEFAULT_SALT_SIZE)
{

    // Get a random number generator from the OS
    AutoSeededRandomPool rng;

    byte* salt = new byte[size];
    // fill the variable with zeros
    memset(salt, 0, size);

    rng.GenerateBlock(salt, size);
    return salt;
}

byte* deriveKey(int size, byte* ikm, int ikm_size, byte* salt, int salt_size)
{

    byte* okm = new byte[size];

    HMACKeyDerivationFunction<SHA256> hkdf;
    hkdf.DeriveKey(okm, size,
                   ikm, ikm_size,
                   salt, salt_size,
                   NULL, 0);
    return okm;
}

byte* readFile(string filename)
{
    FILE* pFile;
    long lSize;
    char* buffer;
    size_t result;
  
    pFile = fopen(filename.c_str(), "r");
    if (pFile == NULL) {
        throw UnknownFile();
    }
  
    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
  
    // allocate memory to contain the whole file:
    buffer = (char*) malloc(sizeof(char) * lSize);
    if (buffer == NULL) {
        throw MemoryError();
    }
  
    // copy the file into the buffer:
    result = fread(buffer, 1, lSize,pFile);
    if ((long)result != lSize) {
        throw ReadingError();
    }
  
    // terminate
    fclose (pFile);
    return (byte*) buffer;
}

void writeFile(string filename, byte* content, int size)
{
    ofstream oFile;
    oFile.open(filename.c_str(), ofstream::binary);
    oFile.write((char*) content, size);
    oFile.close();
}
}
