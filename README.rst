Crypto library for the token server
###################################

This repository contains a library which is used on the `token server
<https://wiki.mozilla.org/Services/NodeAssignment2>`_ to deal with crypto
related things.

This is done in C++ to be able to deal with multiple processors at the same
time, and to have the best possible performances. The underlying library is `a
patched version of Crypto++ <http://github.com/ametaireau/Crypto++>`_, with
added code to deal with `HKDF derivation <https://tools.ietf.org/html/rfc5869>`_.

When you compile the library, it generates a library and an executable binary.
The binary is meant to run with `powerhose
<http://github.com/tziade/powerhose>`_, and the exchanged messages are encoded
using protocol buffers.
