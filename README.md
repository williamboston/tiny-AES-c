### Tiny AES in C

This is a small and portable implementation of AES encryption algorithms written in C.

Forked from [kokke's](https://github.com/kokke/tiny-AES-c) original repository.

The purpose of this fork is to implement high performance parallel implementations of each AES (decryption) mode of operation, to be used on QUT's HPC - however it can be run on any consumer hardware as well.

[CFB Mode](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_feedback_(CFB)) was added to complement the existing CTR, CFB and ECB.

Initial implementation uses OpenMP.

### To run:
```
make
sh run.sh
```

* Requires large .txt file as file input - this implementation uses a variety of large (512mb, 1gb, 2gb) 'lorem ipsum' and random number text files as dummy input to measure performance - none are included in this repository but they're easy enought to generate.
* Thread count is given as a single argument for each mode of operation in run.sh - edit as necessary. 
* Individual modes can be run in isolation as well - the below would run just CBC mode on 128 threads:
```
./cbc.elf 128
```
* Results are saved in 'out.txt'
* Template qsub file also included for those interested in 'run.sub'

### Verification:
An additional file is included - used to verify the results of each mode of decryption. Takes number of threads as single argument. Simply run:
```
./verify.elf 4
```
This will run through each mode of operation both sequentially and then in parallel (to the specified thread count) to confirm each buffer is correctly encrypted - notably this means the buffers remain encrypted in order through parallel operation, so these files can safely be reversed by the inverse decrypt/encrypt operation. Neither mode saves the resultant output after decryption, but this could easily be added.