#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "aes.h"

static void run_CTR_loop();
static int decrypt_ctr(uint8_t buf);

int main(void)
{
    printf("\nTesting AES128 in CTR Mode\n");

    struct timeval *start = malloc(sizeof(struct timeval));
    struct timeval *stop = malloc(sizeof(struct timeval));
    double secs = 0;
    gettimeofday(start, NULL);

    //executes decryption I/O loop
    run_CTR_loop();

    //timestamp end
    gettimeofday(stop, NULL);
    secs = (double)(stop->tv_usec - start->tv_usec) / 1000000 + (double)(stop->tv_sec - start->tv_sec);
    //later this should be an output to csv maybe?
    printf("CTR Time Taken: %f seconds\n\n",secs);

    return 0;
}

static void run_CTR_loop() 
{
    #define CHUNK 1024 /* read 384 bytes at a time - this is 16*24 - as in, 16bytes times the max number of cores at 24*/
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    file = fopen("10gb_lorem.txt", "r");

    //decryption loop
    if (file) {
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //run decryption algorithm
            decrypt_ctr((uint8_t)atoi(buf));
        }
        if (ferror(file)) {
            printf("File Reading Error...");
        }
        fclose(file);
    }
}

static int decrypt_ctr(uint8_t buf)
{
    //init key & init vector
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    //init aes struct
    struct AES_ctx ctx;
    
    //Run CTR on AES Instance
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, &buf, 64);

    return 1;
}