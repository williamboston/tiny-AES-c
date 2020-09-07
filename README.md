### Tiny AES in C

This is a small and portable implementation of the AES ECB, CTR and CBC encryption algorithms written in C.

Forked from (kokke's)[https://github.com/kokke/tiny-AES-c] original repository.

The purpose of this fork is to implement high performance parallel implementations of each (decryption) variation of AES, to be used on QUT's HPC.

CFB Mode was added.

Initial implementation will be using OpenMP.

### To run:

- make
- sh run.sh

* Requires large .txt file as file input - this implementation uses a variety of large (512mb, 1gb, 2gb) 'lorem ipsum' text files as dummy input to measure performance.
* Results are saved in 'output.txt'