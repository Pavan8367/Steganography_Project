/*  Documentation
Name        : G.V.Pavan Kumar
Date        : 25-07-2025
Description : Steganography
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc, char *argv[])
{
    // Declare structures
    EncodeInfo encodeInfo;
    DecodeInfo decodeInfo;

    // Check minimum arguments
    if (argc < 2)
    {
        // Print usage info
        printf("Encoding: ./a.out -e <src_image.bmp> <secret_file.txt|.c|.sh> [optional_image.bmp]\n");
        printf("Decoding: ./a.out -d <stego_image.bmp> [optional_secret_file]\n");
        return 1;
    }

    // Get operation type
    OperationType op_type = check_operation_type(argv);

    if (op_type == e_encode)
    {
        // Check encoding arguments
        if (argc >= 4 && argc <= 5)
        {
            // Validate encoding args
            if (read_and_validate_encode_args(argv, &encodeInfo) == e_failure)
            {
                fprintf(stderr, "Error:❌ Invalid encoding arguments.\n");
                return 1;
            }

            // Do encoding
            if (do_encoding(&encodeInfo) == e_failure)
            {
                close_encode_files(&encodeInfo);
                fprintf(stderr, "Error:❌ Encoding failed.\n");
                return 1;
            }
            else
            {
                close_encode_files(&encodeInfo);
                printf("--------------------------------------------------\n");
                printf("    ✅ INFO: ## Encoding Done Successfully ## \n");
                printf("--------------------------------------------------\n");
                return 0;
            }
        }
        else
        {
            // Invalid encoding argument count
            fprintf(stderr, "Error: Invalid number of arguments for encoding.\n");
            printf("Encoding: ./a.out -e <src_image.bmp> <secret.txt> [output_image.bmp]\n");
            return 1;
        }
    }
    else if (op_type == e_decode)
    {
        // Check decoding arguments
        if (argc >= 3 && argc <= 4)
        {
            // Validate decoding args
            if (read_and_validate_decode_args(argv, &decodeInfo) == e_failure)
            {
                fprintf(stderr, "Error:❌ Invalid decoding arguments.\n");
                return 1;
            }

            // printf("[INFO] Arguments validated.\n");

            // Do decoding
            if (do_decoding(&decodeInfo) == e_failure)
            {
                close_decode_files(&decodeInfo);
                fprintf(stderr, "Error:❌ Decoding failed.\n");
                return 1;
            }
            else
            {
                close_decode_files(&decodeInfo);
                printf("--------------------------------------------------\n");
                printf("    ✅ INFO: ## Decoding Done Successfully ##\n");
                printf("--------------------------------------------------\n");

                return 0;
            }
        }
        else
        {
            // Invalid decoding argument count
            fprintf(stderr, "Error:❌ Invalid number of arguments for decoding.\n");
            printf("Decoding: ./a.out -d <stego_image.bmp> [output_secret.txt]\n");
            return 1;
        }
    }
    else
    {
        // Invalid option
        fprintf(stderr, "Error:❌ Invalid operation type. Use -e or -d.\n");
        printf("Encoding: ./a.out -e <src_image.bmp> <secret.txt> [output_image.bmp]\n");
        printf("Decoding: ./a.out -d <stego_image.bmp> [output_secret.txt]\n");
        return 1;
    }
}