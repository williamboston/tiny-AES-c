#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "aes.h"

static void run_CFB_loop(uint8_t key[], uint8_t text[], long fsize, int p_count);
static void write_CFB_output(double time, int p_count);


int main(int argc, char* argv[])
{
    printf("\nTesting AES128 in CFB Mode...\n");

    //get process count
    int p_count = atoi(argv[1]);

    //open the file
    FILE *f = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text = malloc(fsize + 1);
    int ret = fread(text, 1, fsize, f);
    //close
    fclose(f);

    //init key
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

    //timestamp start
    struct timeval *start = malloc(sizeof(struct timeval));
    struct timeval *stop = malloc(sizeof(struct timeval));
    double secs = 0;
    gettimeofday(start, NULL);

    //executes decryption loop
    run_CFB_loop(key, text, fsize, p_count);

    //timestamp end
    gettimeofday(stop, NULL);
    secs = (double)(stop->tv_usec - start->tv_usec) / 1000000 + (double)(stop->tv_sec - start->tv_sec);

    //write run time to file
    write_CFB_output(secs, p_count);

    return 0;
}

static void run_CFB_loop(uint8_t key[], uint8_t text[], long fsize, int p_count) 
{
    //init aes struct
    struct AES_ctx ctx;
    //run ECB on AES instance
    AES_init_ctx(&ctx, key);
    AES_CFB_decrypt_buffer(&ctx, text, fsize, p_count);
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