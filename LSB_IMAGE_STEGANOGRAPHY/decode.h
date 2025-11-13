/*Name:Nandan Kumar T
Date:13/11/2025
Description:LSB IMAGE STEGANOGRAPHY*/

#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *enc_image_fname;
    FILE *fptr_enc_image;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];

    uint size_secret_file_exn;
    long size_secret_file;
} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the Decoding */
Status do_decoding(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_imag_files(DecodeInfo *decInfo);

Status open_Secret_file(char *argv[], DecodeInfo *decInfo);

/* skipping bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file extenstionsize */
Status decode_file_extn_size(DecodeInfo *decInfo);

/* Decode a int into LSB of image data array */
Status decode_int_to_lsb(char *image_buffer, int *size);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
char decode_byte_to_lsb(char *image_buffer);

#endif
