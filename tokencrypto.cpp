#include <fstream>

#include "tokencrypto.h"
#include "cryptopp/hkdf.h"
#include "cryptopp/sha.h"
#include "cryptopp/osrng.h"

using namespace CryptoPP;
using namespace std;

namespace tokencrypto{

    /**
     * Generates a random salt of the specified lenght.
     * 
     * @param int the number of bytes to generate
     * @returns byte* the generated salt, in bytes
     **/
    byte* generateRandomSalt(int size=DEFAULT_SALT_SIZE){

        // Get a random number from the OS
        byte* random_salt;
        random_salt = new byte[size];
        AutoSeededRandomPool rng;
        rng.GenerateBlock(random_salt, size);

        byte* salt;
        salt = new byte[size];

        memset(salt, 0, sizeof(salt));
        memcpy(salt, random_salt, size);

        return salt;
    }

    /**
     * Derivate a key using the HKDF algorithm.
     *
     * The underlying algorithm used is SHA256.
     *
     * @param byte* the adress of the input keyring material
     * @param byte* the adress of the salt to use to derivate the key
     * @parm int the size of the key to output
     * @param byte* the adress of some additionale info to use while derivating
     *              the key.
     * @returns byte* the derivated output keyring material
     **/
    byte* deriveKey(int size, byte* ikm, int ikm_size, byte* salt,
                    int salt_size){

        byte* okm = new byte[size];

        HMACKeyDerivationFunction<SHA256> hkdf;
        hkdf.DeriveKey(okm, size,
                       ikm, ikm_size,
                       salt, salt_size,
                       NULL, 0);
        return okm;
    }

    /**
     * Reads a key from the filesystem and return it's content in bytes
     *
     * @param string the name of the file to read from the disk
     * @returns byte* the content of the file, in bytes
     **/
    byte* readFile(string filename){
        FILE* pFile;
        long lSize;
        char* buffer;
        size_t result;
      
        pFile = fopen(filename.c_str(), "r");
        if (pFile == NULL) {
            fputs("Unknown file", stderr); exit(1);
        }
      
        // obtain file size:
        fseek(pFile, 0, SEEK_END);
        lSize = ftell(pFile);
        rewind(pFile);
      
        // allocate memory to contain the whole file:
        buffer = (char*) malloc(sizeof(char) * lSize);
        if (buffer == NULL) {
            fputs("Memory error", stderr);
            exit (2);
        }
      
        // copy the file into the buffer:
        result = fread(buffer, 1, lSize,pFile);
        if ((long)result != lSize) {
            fputs("Reading error", stderr);
            exit (3);
        }
      
        // terminate
        fclose (pFile);
        return (byte*) buffer;
    }

    void writeFile(string filename, byte* content, int size){
        ofstream oFile;
        oFile.open(filename.c_str(), ofstream::binary);
        oFile.write((char*) content, size);
        oFile.close();
    }
}
