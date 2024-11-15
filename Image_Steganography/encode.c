#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

/*-------------------------------get image size for bmp------------------------------*/

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

/*------------------------------------open files--------------------------------------*/

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/*---------------------------read and validate encode args---------------------------*/

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // get .bmp file -- ./a.out -e beutiful.bmp secret.txt
    if (argv[2] != NULL && strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        // fetch the name of image file
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    // get .txt file -- ./a.out -e beutiful.bmp secret.txt
    if (argv[2] != NULL && strcmp(strstr(argv[3], "."), ".txt") == 0)
    {
        // fetch the name of image file
        encInfo->secret_fname = argv[3];
    }
    else
    {
        return e_failure;
    }
    // Condition for optional output image name
    if (argv[4] != NULL)
    {
        // Optional output image name given by user
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        // Else by-default stego.bmp image name assign to output
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

/*---------------------------------get file size--------------------------------*/

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/*---------------------------------check capcity--------------------------------*/

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if (encInfo->image_capacity > (54 + 16 + 32 + 32 + 32 + (encInfo->size_secret_file * 8)))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*---------------------------------copy bmp header------------------------------*/

Status copy_bmp_header(FILE *fptr_src_img, FILE *fptr_dest_img)
{
    char str[100];
    fseek(fptr_src_img, 0, SEEK_SET);
    fread(str, sizeof(char), 54, fptr_src_img);
    fwrite(str, sizeof(char), 54, fptr_dest_img);
    return e_success;
}

/*-------------------------------encode byte to lsb-----------------------------*/

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    unsigned int mask = 1 << 7;

    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & mask) >> (7 - i));
        mask = mask >> 1;
    }
    return e_success;
}

/*------------------------------encode data to image---------------------------*/

Status encode_data_to_image(const char *data, int size, FILE *fptr_src, FILE *fptr_stego, EncodeInfo *encInfo)
{
    for (int i = 0; i < size; i++)
    {

        // read 8byte of rgb from beautiful.bmp
        fread(encInfo->image_data, 8, sizeof(char), encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo->image_data);
        fwrite(encInfo->image_data, 8, sizeof(char), encInfo->fptr_stego_image);
    }
    return e_success;
}

/*-------------------------------encode magic string----------------------------*/

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
    return e_success;
}

/*-------------------------------encode size to lsb-----------------------------*/

Status encode_size_to_lsb(char *buffer, int size)
{
    unsigned int mask = 1 << 31;
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | (size & mask) >> (31 - i);
        mask = mask >> 1;
    }
}

/*----------------------------------encode size----------------------------------*/

Status encode_size(int size, FILE *fptr_src_img, FILE *fptr_stego_img)
{
    char str[32];
    fread(str, 32, sizeof(char), fptr_src_img);
    encode_size_to_lsb(str, size);
    fwrite(str, 32, sizeof(char), fptr_stego_img);
    return e_success;
}

/*--------------------------encode secret file extension--------------------------*/

Status encode_secret_file_extn(const char *file_ext, EncodeInfo *encInfo)
{
    file_ext = ".txt";
    encode_data_to_image(file_ext, strlen(file_ext), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
    return e_success;
}

/*-----------------------------encode secret file size-----------------------------*/

Status encode_secret_file_size(long int file_size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, 32, sizeof(char), encInfo->fptr_src_image);
    encode_size_to_lsb(str, file_size);
    fwrite(str, 32, sizeof(char), encInfo->fptr_stego_image);
    return e_success;
}

/*------------------------------encode secret file data----------------------------*/

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    for (int i = 0; i < encInfo->size_secret_file; i++)
    {
        fread(encInfo->image_data, 8, sizeof(char), encInfo->fptr_src_image);
        fread(&ch, 1, sizeof(char), encInfo->fptr_secret);
        encode_byte_to_lsb(ch, encInfo->image_data);
        fwrite(encInfo->image_data, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

/*-----------------------------copy remaining image data----------------------------*/

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_stego)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_stego);
    }
    return e_success;
}

/*--------------------------------------do encoding---------------------------------*/

Status do_encoding(EncodeInfo *encInfo)
{
    // call the rest of function for encoding
    // open the file
    if (open_files(encInfo) == e_success)
    {
        printf("Opened all the file succesfully\n");
        printf("Started Encoding.....\n");
        if (check_capacity(encInfo) == e_success)
        {
            printf("Image is capable to hide the secret data\n");
            // Copy header of source image as it is into stego.bmp
            if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
            {
                printf("Succesfully copied the header\n");
                if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("Magic string encoded succesfully\n");
                    if (encode_size(strlen(".txt"), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                    {
                        printf("Encoding secret file extension size is successful\n");
                        if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("Successfully encoded secret file extension\n");
                            if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                printf("Encoded secret file size succesfully\n");
                                if (encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("Encoded secret data successfully\n");
                                    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("Copied remaining data successfully\n");
                                    }
                                    else
                                    {
                                        printf("Failed Copying remaining data\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Failed to Encode the secret data\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("Failed to encode secret file size\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Failed to encode secret file extension\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failure encoding secret extension size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Error while encoding Magic string\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to copy header\n");
                return e_failure;
            }
        }
        else
        {
            printf("Image size is not sufficient for encoding\n");
        }
    }
    else
    {
        printf("Failure: Opening the files\n");
        return e_failure;
    }
    return e_success;
}

/*---------------------------------------------------END ENCODING PART--------------------------------------------------*/