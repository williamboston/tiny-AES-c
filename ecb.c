#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "aes.h"

static int decrypt_ecb(uint8_t *buf);
static void run_ECB_loop(int p_count);
static void write_ECB_output(double time, int p_count);


int main(int argc, char* argv[])
{
    printf("\nTesting AES128 in ECB Mode...\n");

    //timestamp start
    struct timeval *start = malloc(sizeof(struct timeval));
    struct timeval *stop = malloc(sizeof(struct timeval));
    double secs = 0;
    gettimeofday(start, NULL);

    //get process count
    int p_count = atoi(argv[1]);

    //executes decryption I/O loop
    run_ECB_loop(p_count);

    //timestamp end
    gettimeofday(stop, NULL);
    secs = (double)(stop->tv_usec - start->tv_usec) / 1000000 + (double)(stop->tv_sec - start->tv_sec);

    //write run time to file
    write_ECB_output(secs, p_count);

    return 0;
}

static void run_ECB_loop(int p_count) 
{
    #define CHUNK 384 /* read 384 bytes at a time - this is 16*24 - as in, 16bytes (128 bits) times the max number of cores 24*/
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    char file_name[] = "1gb_lorem.txt";
    file = fopen(file_name, "r");

    //decryption loop
    if (file) {
        //read the whole buffer (384 bytes)
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //run decryption algorithm on each 16 byte (128bit) section at a time
            #pragma omp parallel for num_threads(p_count)
            for (int i=0; i<384; i+=16) {
                //create temporary buffer to send to decryption
                uint8_t minor_buf[16];
                for (int j=0; j<16; j++) 
                {
                    minor_buf[j] = buf[i+j];
                }
                //send to decryption function
                decrypt_ecb(minor_buf);
            }
        }
        if (ferror(file)) {
            printf("File Reading Error...");
        }
        fclose(file);
    }
}

static int decrypt_ecb(uint8_t *buf)
{
    //init key
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    //init aes struct
    struct AES_ctx ctx;

    //run ECB on AES instance
    AES_init_ctx(&ctx, key);
    AES_ECB_decrypt(&ctx, buf);

    return 1;
}

// appends test output to out.txt for storage/analysis
static void write_ECB_output(double time, int p_count) 
{
    FILE * out;
    out = fopen("out.txt", "a");
    fprintf(out, "ECB: %d THREAD: ", p_count);
    fprintf(out, "%f seconds\n\n", time);
    fclose(out);
    printf("ECB Done! Result saved to out.txt\n");
}