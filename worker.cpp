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
void readMasterSecret(Registry reg){
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
 
string derive_secret(string job, Registry reg) {

    // get the master secret value from the registry
    Registry::iterator iter = reg.begin();
    iter = reg.find("master");

    byte* master;

    if (iter != reg.end()) {
        master = (byte*) (iter->second).c_str();
    }

    // Generate the salt
    byte* salt = generateRandomSalt(DEFAULT_SALT_SIZE);

    // Derivate the key
    byte* okm = deriveKey(256, (byte*) master, 256, salt, 256);

    // return the OKM and the salt
    Response resp;
    resp.set_salt(&salt, sizeof(salt));
    resp.set_secret(&okm, sizeof(okm));

    string string_resp;
    resp.SerializeToString(&string_resp);
    return string_resp;
}


int main(int argc, const char *argv[])
{
    // building the map of functions
    Function fderive = Function("derive_secret", &derive_secret);
    Functions functions;
    functions.insert(fderive);
  
    // running 10 workers
    return run_workers(10, functions, &readMasterSecret, NULL);
}
