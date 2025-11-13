/*Name:Nandan Kumar T
Date:13/11/2025
Description:LSB IMAGE STEGANOGRAPHY*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"

#include "types.h"
#include "common.h"
#include <string.h>

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;

    if (check_operation_type(argv) == e_encode)
    {
        if (argc > 5 || argc == 1)
        {
            printf(CYA "\nFor Encoding : ./a.out -e <.bmp_file> <secret_file> [output file]\n" END);
            return 0;
        }
        printf(CYA "\nINFO:Operation is Encoding\n" END);

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                printf(YEL "------------------------Encoding Operation Done-------------------------\n" END);
            }
            else
            {
                printf(RED "\nINFO:Encoding Failed!\n" END);
            }
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        if (argc > 4 || argc == 1)
        {
            printf(CYA "\nFor decoding : ./a.out -d <stego.bmp_file> <secret_file_name> \n" END);
            return 0;
        }
        printf(GRE "\nINFO:Operation is decoding\n" END);
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(argv, &decInfo) == e_success)
            {
                printf(YEL "------------------------Decoding Operation Done-------------------------\n" END);
            }
            else
            {
                printf(RED "\nINFO:Decoding Failed!\n" END);
            }
        }
    }
    else
    {
        printf(RED "Invalid Arguments choose correctly.for help pass -help" END);
    }
    return 0;
}
OperationType check_operation_type(char *argv[1])
{
    if (argv[1] == NULL)
    {
        return e_unsupported;
    }
    if (strcmp(argv[1], "-help") == 0)
    {
        printf(RED "\nINFO:Encoding : ./a.out -e <.bmp_file> <.text_file> [output file]\nINFO:Decoding: ./a.out  -d <.bmp_file> [output file]\n" END);
        return 0;
    }
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Validate source image
    char *ret1 = strstr(argv[2], ".bmp");
    if (ret1 == NULL || strcmp(ret1, ".bmp") != 0)
    {
        printf(RED "INFO:Invalid source image extension. Please choose a .bmp file.\n" END);
        return e_failure;
    }
    encInfo->src_image_fname = argv[2];

    // Validate secret text file
    char extn[5][5] = {".cpp", ".txt", ".c", ".sh", ".py"};
    int flag = 0;
    for (int i = 0; i < 5; i++)
    {
        char *ret2 = strstr(argv[3], extn[i]);

        if (ret2 != NULL && strcmp(ret2, extn[i]) == 0)
        {
            encInfo->secret_fname = argv[3];
            strcpy(encInfo->extn_secret_file, extn[i]);
            // printf("%s", encInfo->extn_secret_file);
            flag = 1;
            break;
        }
    }
    if (!flag)
    {
        printf(RED "INFO:Ivalid secret text file extention\n" END);
        return e_failure;
    }

    // Validate output stego image
    if (argv[4] != NULL)
    {
        char *ret3 = strstr(argv[4], ".bmp");
        if (ret3 == NULL || strcmp(ret3, ".bmp") != 0)
        {
            printf(RED "INFO:Invalid output image extension. Please choose a .bmp file.\n" END);
            return e_failure;
        }
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        printf(RED "INFO:Output image not provided. Using default name: stego.bmp\n" END);
        encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Validate source image
    char *ret1 = strstr(argv[2], ".bmp");
    if (ret1 == NULL || strcmp(ret1, ".bmp") != 0)
    {
        printf(RED "INFO:Invalid source image extension. Please choose a .bmp file.\n" END);
        return e_failure;
    }
    decInfo->enc_image_fname = argv[2];

    return e_success;
}
