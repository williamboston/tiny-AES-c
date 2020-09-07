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

* Requires large .txt file as file input - this implementation uses a variety of large (512mb, 1gb, 2gb) 'lorem ipsum' text files as dummy input to measure performance - none are included in this repository. See this [link](https://www.windows-commandline.com/how-to-create-large-dummy-file/) for information on how to create these files if required (windows only)
* Thread count is declared in run.sh
* Results are saved in 'output.txt'