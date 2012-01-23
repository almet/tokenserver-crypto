#include <iostream>
#include <fstream>

#include "cryptopp/hkdf.h"
#include "cryptopp/sha.h"
#include "cryptopp/osrng.h"

using namespace CryptoPP;
using namespace std;

#define DEFAULT_SALT_SIZE 128
#define DEFAULT_KEY_SIZE 128

namespace crypto{

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

int main(int argc, const char *argv[])
{
    // byte ikm[80] = { 
    //     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    //     0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    //     0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21,
    //     0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
    //     0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    //     0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
    //     0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f };

    byte* salt = crypto::generateRandomSalt(DEFAULT_SALT_SIZE);
    cout << salt << endl;

    crypto::writeFile("master", salt, DEFAULT_SALT_SIZE);

    byte* master = crypto::readFile("master");
    cout << master << endl;

    byte* okm = crypto::deriveKey(DEFAULT_SALT_SIZE,
                                  master, DEFAULT_SALT_SIZE,
                                  salt, DEFAULT_SALT_SIZE);
    cout << okm << endl;
    return 0;
}
