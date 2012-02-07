/**
 * Powerhose worker spawning the processus and receiving / sending back
 * messages through zeromq.
 **/
#include <iostream>

#include "tokencrypto.h"
#include "libhose.h"
#include "response.pb.h"

using namespace tokencrypto;
using namespace std;
using namespace powerhose;

/**
 * Reads the master secret and put it in the registry
 *
 * @param Registry reg the registry to use
 * @returns void
 **/
void readMasterSecret(Registry reg)
{
    string master = (char*) readFile("master");

    pair <string, string> master_pair = pair <string, string>("master", master);
    reg.insert(master_pair);
}

/**
 * Regarding the information contained in the registry, generates a salt and
 * derivate the master secret with it.
 *
 * This function is meant to be used with the powerhose library, thus the
 * signature.
 *
 * @param
 **/
 
string deriveSecret(string job, Registry registry)
{

    // get the master secret value from the registry
    byte* master = (byte*) (registry.find("master")->second).c_str();

    // Generate the salt
    byte* salt = generateRandomSalt(DEFAULT_SALT_SIZE);

    // Derivate the key
    byte* okm = deriveKey(256, (byte*) master, 256, salt, 256);

    cout << "salt" << endl << salt << endl << endl;
    cout << "okm" << endl << okm << endl << endl;
    cout << "master" << endl << master << endl << endl;

    // return the OKM and the salt XXX check that sizeof is doing what's
    // intended
    Response resp;
    resp.set_salt(&salt, 256);
    resp.set_secret(&okm, 256);

    string string_resp;
    resp.SerializeToString(&string_resp);
    return string_resp;
}

int main(int argc, const char *argv[])
{
    map<string, string> reg;
    readMasterSecret(reg);
    cout << deriveSecret("", reg);
}
