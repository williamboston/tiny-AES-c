### Tiny AES in C

This is a small and portable implementation of the AES [ECB](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Electronic_Codebook_.28ECB.29), [CTR](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Counter_.28CTR.29) and [CBC](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_Block_Chaining_.28CBC.29) encryption algorithms written in C.

Forked from [kokke's](https://github.com/kokke/tiny-AES-c) original repository.

The purpose of this fork is to implement high performance parallel implementations of each (decryption) variation of AES, to be used on QUT's HPC.

Initial implementation will be using OpenMP, but I would ultimately like to get a pthreads implementation for a bit of fun.