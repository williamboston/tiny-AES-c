#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include "aes.h"

static bool verify_ECB_loop(int p_count)
{
    // SEQUENTIAL //

    //open the file
    FILE *f1 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f1, 0, SEEK_END);
    long fsize1 = ftell(f1);
    fseek(f1, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text1 = malloc(fsize1 + 1);
    int ret1 = fread(text1, 1, fsize1, f1);
    //close
    fclose(f1);

    //init key
    uint8_t key1[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

    //send each 16 byte block to decryption function
    for (int j=0; j<fsize1;j+=16) {
        //init aes struct
        struct AES_ctx ctx;
        //run ECB on AES instance
        AES_init_ctx(&ctx, key1);
        AES_ECB_decrypt(&ctx, &text1[j]);
    }

    // PARALLEL //

    //open the file
    FILE *f2 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f2, 0, SEEK_END);
    long fsize2 = ftell(f2);
    fseek(f2, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text2 = malloc(fsize2 + 1);
    int ret2 = fread(text2, 1, fsize2, f2);
    //close
    fclose(f2);

    //init key
    uint8_t key2[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

    //send each 16 byte block to decryption function
    #pragma omp parallel for num_threads(p_count)
    for (int j=0; j<fsize2;j+=16) {
        //init aes struct
        struct AES_ctx ctx;
        //run ECB on AES instance
        AES_init_ctx(&ctx, key2);
        AES_ECB_decrypt(&ctx, &text2[j]);
    }

    // COMPARE //

    //check same outcome for each buffer
    if (0 == memcmp(text1, text2, fsize1)){
        return true;
    }

    // FREE //

    free(text1);
    free(text2);
}


static bool verify_CBC_loop(int p_count) 
{
    // SEQUENTIAL //

    //open the file
    FILE *f1 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f1, 0, SEEK_END);
    long fsize1 = ftell(f1);
    fseek(f1, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text1 = malloc(fsize1 + 1);
    int ret1 = fread(text1, 1, fsize1, f1);
    //close
    fclose(f1);

    //init key
    uint8_t key1[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv1[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    //init aes struct
    struct AES_ctx ctx1;
    //run ECB on AES instance
    AES_init_ctx_iv(&ctx1, key1, iv1);
    AES_CBC_decrypt_buffer(&ctx1, text1, fsize1, 1);

    // PARALLEL //

    //open the file
    FILE *f2 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f2, 0, SEEK_END);
    long fsize2 = ftell(f2);
    fseek(f2, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text2 = malloc(fsize2 + 1);
    int ret2 = fread(text2, 1, fsize2, f2);
    //close
    fclose(f2);

    //init key
    uint8_t key2[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv2[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    //init aes struct
    struct AES_ctx ctx2;
    //run ECB on AES instance
    AES_init_ctx_iv(&ctx2, key2, iv2);
    AES_CBC_decrypt_buffer(&ctx2, text2, fsize2, p_count);

    // COMPARE //

    long counter = 0;
    for (int i=0;i<fsize1;i+=16){
        if (0 == memcmp(text1, text2, 16)){
            counter++;
        }
    }
    int errors = (fsize1/16) - counter;

    // FREE //

    free(text1);
    free(text2);

    //return true on 0 errors
    if (errors == 0){
        return true;
    }
}


static bool verify_CFB_loop(int p_count) 
{
    // SEQUENTIAL //

    //open the file
    FILE *f1 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f1, 0, SEEK_END);
    long fsize1 = ftell(f1);
    fseek(f1, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text1 = malloc(fsize1 + 1);
    int ret1 = fread(text1, 1, fsize1, f1);
    //close
    fclose(f1);

    //init key
    uint8_t key1[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv1[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f };

    //init aes struct
    struct AES_ctx ctx1;
    //run ECB on AES instance
    AES_init_ctx_iv(&ctx1, key1, iv1);
    AES_CFB_decrypt_buffer(&ctx1, text1, fsize1, 1);

    // PARALLEL //

    //open the file
    FILE *f2 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f2, 0, SEEK_END);
    long fsize2 = ftell(f2);
    fseek(f2, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text2 = malloc(fsize2 + 1);
    int ret2 = fread(text2, 1, fsize2, f2);
    //close
    fclose(f2);

    //init key
    uint8_t key2[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv2[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f };
    
    //init aes struct
    struct AES_ctx ctx2;
    //run ECB on AES instance
    AES_init_ctx_iv(&ctx2, key2, iv2);
    AES_CFB_decrypt_buffer(&ctx2, text2, fsize2, p_count);

    // COMPARE //

    long counter = 0;
    for (int i=0;i<fsize1;i+=16){
        if (0 == memcmp(text1, text2, 16)){
            counter++;
        }
    }
    int errors = (fsize1/16) - counter;

    // FREE //

    free(text1);
    free(text2);

    //return true on 0 errors
    if (errors == 0){
        return true;
    }
}

static bool verify_CTR_loop(int p_count) 
{
    // SEQUENTIAL //

    //open the file
    FILE *f1 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f1, 0, SEEK_END);
    long fsize1 = ftell(f1);
    fseek(f1, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text1 = malloc(fsize1 + 1);
    int ret1 = fread(text1, 1, fsize1, f1);
    //close
    fclose(f1);

    //init key
    uint8_t key1[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv1[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

    //send each 16 byte block to decryption function
    for (int j=0; j<fsize1;j+=16) {
        //init aes struct
        struct AES_ctx ctx;
        //run ECB on AES instance
        AES_init_ctx_iv(&ctx, key1, iv1);
        AES_CTR_xcrypt_buffer(&ctx, &text1[j], 16, p_count);
    }

    // PARALLEL //

    //open the file
    FILE *f2 = fopen("random_1gb.txt", "rb");
    //find size then rewind
    fseek(f2, 0, SEEK_END);
    long fsize2 = ftell(f2);
    fseek(f2, 0, SEEK_SET);

    //malloc enough memory for the whole text file (1gb)
    uint8_t *text2 = malloc(fsize2 + 1);
    int ret2 = fread(text2, 1, fsize2, f2);
    //close
    fclose(f2);

    //init key
    uint8_t key2[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t iv2[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

    //send each 16 byte block to decryption function
    #pragma omp parallel for num_threads(p_count)
    for (int j=0; j<fsize2;j+=16) {
        //init aes struct
        struct AES_ctx ctx;
        //run ECB on AES instance
        AES_init_ctx_iv(&ctx, key2, iv2);
        AES_CTR_xcrypt_buffer(&ctx, &text2[j], 16, p_count);
    }

    // COMPARE //

    //check same outcome for each buffer
    if (0 == memcmp(text1, text2, fsize1)){
        return true;
    }

    // FREE //

    free(text1);
    free(text2);
}

int main(int argc, char* argv[]) {

    //get process count
    int p_count = atoi(argv[1]);

    //execute ECB parallel and serial loop, then compare the result
    printf("\nRunning ECB Check...\n");
    if (verify_ECB_loop(p_count)){
        printf("All ECB encryption successful and verified!\n\n");
    }

    //execute CBC parallel and serial loop, then compare the result
    printf("Running CBC Check...\n");
    if (verify_CBC_loop(p_count)){
        printf("All CBC encryption successful and verified!\n\n");
    }

    //execute CFB parallel and serial loop, then compare the result
    printf("Running CFB Check...\n");
    if (verify_CFB_loop(p_count)){
        printf("All CFB encryption successful and verified!\n\n");
    }

    //execute CTR parallel and serial loop, then compare the result
    printf("Running CTR Check...\n");
    if (verify_CTR_loop(p_count)){
        printf("All CTR encryption successful and verified!\n\n");
    }
}