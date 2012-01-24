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

How to run it?
==============

To run this code, you need to compile it. First, make sure that you have the
right version of Crypto++. What we are doing here is generating a derivated
secret from a key, using the HKDF algorithm provided by the patched version of
Crypto++.

Then, make sure you have the right libraries to run the project. You need to
install powerhose on your system and compile it. Once done that, running `make`
should be enough for building the code able to spawn the processes::

    make serve&

You also need to start a consumer for this lib. This code ships with a python
consumer. You need to have the `pyzmq <http://pypi.python.org/pypi/pyzmq/2.1.11>`_
library installed as well as powerhose. If everything is correct, then you can
run the consumer by doing ::

    make consume
