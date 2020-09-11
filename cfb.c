#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "aes.h"

static void run_CFB_loop(int p_count);
static int decrypt_cfb(uint8_t *buf);
static void write_CFB_output(double time, int p_count);


int main(int argc, char* argv[])
{
    printf("\nTesting AES128 in CFB Mode...\n");

    //timestamp start
    struct timeval *start = malloc(sizeof(struct timeval));
    struct timeval *stop = malloc(sizeof(struct timeval));
    double secs = 0;
    gettimeofday(start, NULL);

    //get process count
    int p_count = atoi(argv[1]);

    //executes decryption I/O loop
    run_CFB_loop(p_count);

    //timestamp end
    gettimeofday(stop, NULL);
    secs = (double)(stop->tv_usec - start->tv_usec) / 1000000 + (double)(stop->tv_sec - start->tv_sec);

    //write run time to file
    write_CFB_output(secs, p_count);

    return 0;
}

static void run_CFB_loop(int p_count) 
{
    #define CHUNK 1572864 // read 1.5mb at a time
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    char file_name[] = "random_1gb.txt";
    file = fopen(file_name, "r");

    //decryption loop
    if (file) {
        //read the whole buffer
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //run decryption algorithm on each 16 byte (128bit) section at a time
            #pragma omp parallel for num_threads(p_count)
            for (int i=0; i<1572864; i+=16) {
                //create temporary buffer to send to decryption
                uint8_t minor_buf[16];
                for (int j=0; j<16; j++) 
                {
                    minor_buf[j] = buf[i+j];
                }
                decrypt_cfb(minor_buf);
            }
        }
        fclose(file);
    }
}

static int decrypt_cfb(uint8_t *buf)
{
    //init key & init vector
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    //init aes struct
    struct AES_ctx ctx;
    
    //Run CFB on AES instance
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CFB_decrypt_buffer(&ctx, buf, 16);

    return 1;
}

// appends test output to out.txt for storage/analysis
static void write_CFB_output(double time, int p_count)
{
    FILE * out;
    out = fopen("out.txt", "a");
    fprintf(out, "CFB: %d THREAD: ", p_count);
    fprintf(out, "%f seconds\n\n", time);
    fclose(out);
    printf("CFB Done! Result saved to out.txt\n");
}