/*Name:Nandan Kumar T
Date:13/11/2025
Description:LSB IMAGE STEGANOGRAPHY*/

#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status do_decoding(char *argv[], DecodeInfo *decInfo)
{
    if (open_imag_files(decInfo) == e_success)
    {
        printf(YE1 "INFO:opening the files for decoding\n" END);
    }

    printf(YEL "\n==============INFO: ## decoding Procedure Started ##=================\n" END);

    if (skip_bmp_header(decInfo->fptr_enc_image) == e_success)
    {
        printf(MAG "INFO:skipping 54 bytes of data is done\n" END);
    }
    else
    {
        printf(RED "Error:while skipping  of 54 bytes of data is done\n" END);
        return e_failure;
    }
    if (decode_magic_string(decInfo) == e_success)
    {
        printf(MAG "INFO:decoding of magic string from image is done\n" END);
    }
    else
    {
        printf(RED "Error:while decoding of magic string from image \n" END);
        return e_failure;
    }
    // decoding the data of file extension size
    if (decode_file_extn_size(decInfo) == e_success)
    {

        printf(MAG "INFO:Decoding of file extension size from image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Decoding of file extension size to image\n" END);
        return e_failure;
    }

    // decoding the data of file extension
    if (decode_secret_file_extn(decInfo) == e_success)
    {
        printf(MAG "INFO:Encoding of file extension to image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Encoding of file extension  to image\n" END);
        return e_failure;
    }

    if (open_Secret_file(argv, decInfo) == e_success)
    {
        printf(MAG "INFO:opening the secret output files for decoding the data\n" END);
    }
    else
    {
        printf(RED "Error:while opening the secret output files for decoding the data\n" END);
        return e_failure;
    }
    // encoding the data of extension file size
    if (decode_secret_file_size(decInfo) == e_success)
    {
        printf(MAG "INFO:decoding of secret file size from image is done\n" END);
    }
    else
    {
        printf(RED "Error:while decoding of secret file size from image\n" END);
        return e_failure;
    }
    // encoding the data of extension secret file data
    if (decode_secret_file_data(decInfo) == e_success)
    {
        printf(MAG "INFO:decoding of secret data from image to output file is done\n" END);
    }
    else
    {
        printf(RED "Error:while decoding of secret data from image to output file is done\n" END);
        return e_failure;
    }
    return e_success;
}

Status open_imag_files(DecodeInfo *decInfo)
{
    // encoded Image file
    decInfo->fptr_enc_image = fopen(decInfo->enc_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_enc_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open Src Image file %s\n", decInfo->enc_image_fname);

        return e_failure;
    }
    return e_success;
}

Status skip_bmp_header(FILE *fptr_enc_image)
{
    fseek(fptr_enc_image, 54, SEEK_SET);
    return e_success;
}

char decode_byte_to_lsb(char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = ch | (image_buffer[i] & 1) << i;
    }
    return ch;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3];
    char image_buffer[8];
    int i;
    for (i = 0; i < 2; i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_enc_image);
        magic_string[i] = decode_byte_to_lsb(image_buffer);
    }
    magic_string[i] = '\0';
    // printf("%s\n",magic_string);

    // compare the decoded magic string  with the original magic string
    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        printf("INFO: Magic string not present, Image is not Stegged\n");
        return e_failure;
    }
}
Status decode_int_to_lsb(char *image_buffer, int *size)
{
    uint size1 = 0;
    for (int i = 0; i < 32; i++)
    {
        size1 = size1 | (image_buffer[i] & 1) << i;
    }
    // printf("%d\n",size1);
    *size = size1;
    return e_success;
}

Status decode_file_extn_size(DecodeInfo *decInfo)
{
    int size = 0;
    char size_buffer[32];
    fread(size_buffer, 32, 1, decInfo->fptr_enc_image);
    decode_int_to_lsb(size_buffer, &size);
    decInfo->size_secret_file_exn = size;

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    int i;
    for (i = 0; i < decInfo->size_secret_file_exn; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_enc_image);
        decInfo->extn_secret_file[i] = decode_byte_to_lsb(buffer);
    }
    decInfo->extn_secret_file[i] = '\0';
    // printf("%s\n", decInfo->extn_secret_file);
    return e_success;
}

Status open_Secret_file(char *argv[3], DecodeInfo *decInfo)
{
    char def_fname[100];
    if (argv[3] != NULL)
    {
        strcpy(def_fname, argv[3]);
    }
    else
    {
        printf(YE1 "INFO:Output file not provided. Using default name: output\n" END);
        strcpy(def_fname,"output");
    }

    strcat(def_fname, decInfo->extn_secret_file);
    decInfo->secret_fname = def_fname;

    // printf("%s\n",decInfo->secret_fname);

    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    // Do Error handling
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open Src Image file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    int file_size = 0;
    char secret_buffer[32];
    fread(secret_buffer, 32, 1, decInfo->fptr_enc_image);
    decode_int_to_lsb(secret_buffer, &file_size);
    decInfo->size_secret_file = file_size;
    // printf("%ld",decInfo->size_secret_file);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    long size = decInfo->size_secret_file;
    char image_buffer[8];
    char ch;
    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_enc_image);
        ch = decode_byte_to_lsb(image_buffer);

        fwrite(&ch, 1, 1, decInfo->fptr_secret);
    }
    return e_success;
}
