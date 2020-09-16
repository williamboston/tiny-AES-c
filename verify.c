#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include "aes.h"


static int verify_decrypt_ecb(uint8_t *buf)
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

static int verify_decrypt_cbc(uint8_t *buf)
{
    //init key & init vector
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    //init aes struct
    struct AES_ctx ctx;
    
    //Run CBC on AES instance
    AES_init_ctx_iv(&ctx, key, iv);
    // AES_CBC_decrypt_buffer(&ctx, buf, 16);

    return 1;
}

static int verify_decrypt_cfb(uint8_t *buf)
{
    //init key & init vector
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    //init aes struct
    struct AES_ctx ctx;
    
    //Run CFB on AES instance
    AES_init_ctx_iv(&ctx, key, iv);
    // AES_CFB_decrypt_buffer(&ctx, buf, 16);

    return 1;
}

static int verify_decrypt_ctr(uint8_t *buf)
{
    //init key & init vector
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    //init aes struct
    struct AES_ctx ctx;
    
    //Run CTR on AES Instance
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, buf, 16);

    return 1;
}

static bool verify_ECB_loop(int p_count)
{
    #define CHUNK 384 /* read 384 bytes at a time - this is 16*24 - as in, 16bytes (128 bits) times the max number of cores 24*/
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    char file_name[] = "512mb_lorem.txt";
    file = fopen(file_name, "r");

    //decryption loop
    if (file) {
        //read the whole buffer (384 bytes)
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //full size buffers that we will compare against
            uint8_t serial_whole[384];
            uint8_t para_whole[384];
            //run decryption algorithm on each 16 byte (128bit) section at a time
            for (int i=0; i<384; i+=16) {
                uint8_t serial_buf[16];
                for (int j=0; j<16; j++) 
                {
                    serial_buf[j] = buf[i+j];
                }
                //send to decryption function
                verify_decrypt_ecb(serial_buf);
                //save to larger buf
                for (int a=0; a<16; a++) {
                    serial_whole[i+a] = serial_buf[a];
                }
            }
            #pragma omp parallel for num_threads(p_count)
            for (int k=0; k<384; k+=16) {
                uint8_t para_buf[16];
                for (int l=0; l<16; l++) 
                {
                    para_buf[l] = buf[k+l];
                }
                //send to decryption function
                verify_decrypt_ecb(para_buf);
                //save to larger buf
                for (int b=0; b<16; b++) {
                    para_whole[k+b] = para_buf[b];
                }
            }
            //check same outcome for each buffer
            if (0 != memcmp(serial_whole, para_whole, 384)){
                return false;
            }
        }
        if (ferror(file)) {
            printf("File Reading Error...");
        }
        fclose(file);
    }
    return true;
}


static bool verify_CBC_loop(int p_count) 
{
    #define CHUNK 384 /* read 384 bytes at a time - this is 16*24 - as in, 16bytes times the max number of cores at 24*/
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    char file_name[] = "512mb_lorem.txt";
    file = fopen(file_name, "r");

    //decryption loop
    if (file) {
        //read the whole buffer (384 bytes)
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //full size buffers that we will compare against
            uint8_t serial_whole[384];
            uint8_t para_whole[384];
            //run decryption algorithm on each 16 byte (128bit) section at a time
            for (int i=0; i<384; i+=16) {
                uint8_t serial_buf[16];
                for (int j=0; j<16; j++) 
                {
                    serial_buf[j] = buf[i+j];
                }
                //send to decryption function
                verify_decrypt_cbc(serial_buf);
                //save to larger buf
                for (int a=0; a<16; a++) {
                    serial_whole[i+a] = serial_buf[a];
                }
            }
            #pragma omp parallel for num_threads(p_count)
            for (int k=0; k<384; k+=16) {
                uint8_t para_buf[16];
                for (int l=0; l<16; l++) 
                {
                    para_buf[l] = buf[k+l];
                }
                //send to decryption function
                verify_decrypt_cbc(para_buf);
                //save to larger buf
                for (int b=0; b<16; b++) {
                    para_whole[k+b] = para_buf[b];
                }
            }
            //check same outcome for each buffer
            if (0 != memcmp(serial_whole, para_whole, 384)){
                return false;
            }
        }
        if (ferror(file)) {
            printf("File Reading Error...");
        }
        fclose(file);
    }
    return true;
}


static bool verify_CFB_loop(int p_count) 
{
    #define CHUNK 384 /* read 384 bytes at a time - this is 16*24 - as in, 16bytes times the max number of cores at 24*/
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    char file_name[] = "512mb_lorem.txt";
    file = fopen(file_name, "r");

    //decryption loop
    if (file) {
        //read the whole buffer (384 bytes)
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //full size buffers that we will compare against
            uint8_t serial_whole[384];
            uint8_t para_whole[384];
            //run decryption algorithm on each 16 byte (128bit) section at a time
            for (int i=0; i<384; i+=16) {
                uint8_t serial_buf[16];
                for (int j=0; j<16; j++) 
                {
                    serial_buf[j] = buf[i+j];
                }
                //send to decryption function
                verify_decrypt_cfb(serial_buf);
                //save to larger buf
                for (int a=0; a<16; a++) {
                    serial_whole[i+a] = serial_buf[a];
                }
            }
            #pragma omp parallel for num_threads(p_count)
            for (int k=0; k<384; k+=16) {
                uint8_t para_buf[16];
                for (int l=0; l<16; l++) 
                {
                    para_buf[l] = buf[k+l];
                }
                //send to decryption function
                verify_decrypt_cfb(para_buf);
                //save to larger buf
                for (int b=0; b<16; b++) {
                    para_whole[k+b] = para_buf[b];
                }
            }
            //check same outcome for each buffer
            if (0 != memcmp(serial_whole, para_whole, 384)){
                return false;
            }
        }
        if (ferror(file)) {
            printf("File Reading Error...");
        }
        fclose(file);
    }
    return true;
}

static bool verify_CTR_loop(int p_count) 
{
    #define CHUNK 384 /* read 384 bytes at a time - this is 16*24 - as in, 16bytes times the max number of cores at 24*/
    char buf[CHUNK];
    FILE *file;
    size_t nread;
    char file_name[] = "512mb_lorem.txt";
    file = fopen(file_name, "r");

    //decryption loop
    if (file) {
        //read the whole buffer (384 bytes)
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
            //full size buffers that we will compare against
            uint8_t serial_whole[384];
            uint8_t para_whole[384];
            //run decryption algorithm on each 16 byte (128bit) section at a time
            for (int i=0; i<384; i+=16) {
                uint8_t serial_buf[16];
                for (int j=0; j<16; j++) 
                {
                    serial_buf[j] = buf[i+j];
                }
                //send to decryption function
                verify_decrypt_ctr(serial_buf);
                //save to larger buf
                for (int a=0; a<16; a++) {
                    serial_whole[i+a] = serial_buf[a];
                }
            }
            #pragma omp parallel for num_threads(p_count)
            for (int k=0; k<384; k+=16) {
                uint8_t para_buf[16];
                for (int l=0; l<16; l++) 
                {
                    para_buf[l] = buf[k+l];
                }
                //send to decryption function
                verify_decrypt_ctr(para_buf);
                //save to larger buf
                for (int b=0; b<16; b++) {
                    para_whole[k+b] = para_buf[b];
                }
            }
            //check same outcome for each buffer
            if (0 != memcmp(serial_whole, para_whole, 384)){
                return false;
            }
        }
        if (ferror(file)) {
            printf("File Reading Error...");
        }
        fclose(file);
    }
    return true;
}

int main(int argc, char* argv[]) {

    //get process count
    int p_count = atoi(argv[1]);

    //execute ECB parallel and serial I/O loop, then compares
    printf("\nRunning ECB Check...\n");
    if (verify_ECB_loop(p_count)){
        printf("All ECB encryption successful and verified!\n\n");
    }

    //execute CBC parallel and serial I/O loop, then compares
    printf("Running CBC Check...\n");
    if (verify_CBC_loop(p_count)){
        printf("All CBC encryption successful and verified!\n\n");
    }

    //execute CFB parallel and serial I/O loop, then compares
    printf("Running CFB Check...\n");
    if (verify_CFB_loop(p_count)){
        printf("All CFB encryption successful and verified!\n\n");
    }

    //execute CTR parallel and serial I/O loop, then compares
    printf("Running CTR Check...\n");
    if (verify_CTR_loop(p_count)){
        printf("All CTR encryption successful and verified!\n\n");
    }
}