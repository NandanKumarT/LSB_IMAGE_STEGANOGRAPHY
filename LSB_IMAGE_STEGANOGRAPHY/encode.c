/*Name:Nandan Kumar T
Date:13/11/2025
Description:LSB IMAGE STEGANOGRAPHY*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

Status do_encoding(EncodeInfo *encInfo)
{
    // checking the opening of the all files
    if (open_files(encInfo) == e_success)
    {
        printf(YE1 "INFO:opening the files for Encoding\n" END);
    }
    printf(YEL "\n==============INFO: ## Encoding Procedure Started ##=================\n" END);
    // checking the capacity of image to encode the data
    if (check_capacity(encInfo) == e_success)
    {
        printf(MAG "INFO:Capacity of image is suffieciant\n" END);
    }
    else
    {
        printf(RED "Error:Capacity of image is not suffieciant\n" END);
        return e_failure;
    }

    // copying the src.bmp header to output.bmp
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf(MAG "INFO:Copying of 54 byte data is done\n" END);
    }
    else
    {
        printf(RED "Error:while Copying of 54 byte data \n" END);
        return e_failure;
    }

    // encoding the data of magic string
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
        printf(MAG "INFO:Encoding of magic string to image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Encoding of magic string to image \n" END);
        return e_failure;
    }

    // encoding the data of file extension size
    if (encode_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
    {

        printf(MAG "INFO:Encoding of file extension size to image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Encoding of file extension size to image\n" END);
        return e_failure;
    }

    // encoding the data of file extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
    {
        printf(MAG "INFO:Encoding of file extension to image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Encoding of file extension  to image\n" END);
        return e_failure;
    }

    // encoding the data of extension file size
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
    {
        printf(MAG "INFO:Encoding of secret file size to image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Encoding of secret file size to image\n" END);
        return e_failure;
    }

    // encoding the data of extension secret file data
    if (encode_secret_file_data(encInfo) == e_success)
    {
        printf(MAG "INFO:Encoding of secret data to image is done\n" END);
    }
    else
    {
        printf(RED "Error:while Encoding of data size to image\n" END);
        return e_failure;
    }

    // COPYING THE REMAING DATA OF SRC IMAGE DATA TO OUTPUT IMAGE
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf(MAG "INFO:copying the remaining the data  of src image is done\n" END);
    }
    else
    {
        printf(RED "Error:while copying the remaining the data  of src image\n" END);
        return e_failure;
    }
    // RETURNING SUCCESS AFTER ENCODING
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    return e_success;
}
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
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
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open Src Image file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open Secret file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open Stego Image file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

// GETTING THE SECRET FILE SIZE
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // MOVING END OF FILE
    return ftell(fptr);       // RETURNING  THE POSITION MEANS SIZE OF FILE
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    printf(CYA "total image size =%d\n" END, encInfo->image_capacity);
    int magic_string = strlen(MAGIC_STRING);
    printf(CYA "magic string size= %d\n" END, magic_string);
    int extention = strlen(encInfo->extn_secret_file);
    printf(CYA "secret file extension size=%d\n" END, extention);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf(CYA "size of secret file = %ld\n" END, encInfo->size_secret_file);
    int encoding_things = (magic_string + sizeof(int) + extention + sizeof(int) + encInfo->size_secret_file) * 8;
    printf(CYA "Total size of encoding data = %d\n" END, encoding_things);
    if (encoding_things <= encInfo->image_capacity)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // move offset to 0th index
    fseek(fptr_src_image, 0, SEEK_SET);
    // read 54 bytes
    char buffer[54];
    fread(buffer, 54, 1, fptr_src_image);
    // write 54 bytes into the dest image
    fwrite(buffer, 54, 1, fptr_dest_image);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        // bitwise logic to encoding the secret file data to the image
        image_buffer[i] = (data >> i & 1) | (image_buffer[i] & ~1);
    }
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        // read 8 bytes from the src imge
        fread(image_buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        // write 8 bytes to dest the dest image
        fwrite(image_buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    int len = strlen(magic_string);
    encode_data_to_image(magic_string, len, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_int_to_lsb(int size, char *image_buffer)
{
    // logic
    for (int i = 0; i < 32; i++)
    {
        image_buffer[i] = (size >> i & 1) | (image_buffer[i] & ~1);
    }

    return e_success;
}

Status encode_file_extn_size(int size, EncodeInfo *encInfo)
{
    // read 32 bytes from the src_image
    char image_buffer[32];
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    encode_int_to_lsb(size, image_buffer);
    // write 32 bytes to the dest_image
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    if (encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_size[32];
    fread(image_size, 32, 1, encInfo->fptr_src_image);
    encode_int_to_lsb(file_size, image_size);
    fwrite(image_size, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // extract the secret msg from the secret file
    char secret_file_data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(secret_file_data, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    secret_file_data[encInfo->size_secret_file] = '\0';
    // printf("secret msg = %s\n", secret_file_data);
    if (encode_data_to_image(secret_file_data, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success) // if
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) == 1)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
