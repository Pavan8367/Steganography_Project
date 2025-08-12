/*  Documentation
Name        : G.V.Pavan Kumar
Date        : 26-07-2025
Description : Steganography
*/
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

// Validates command-line decode arguments and stores file names
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = NULL;
    decInfo->fptr_secret = NULL;

    // Validate that the input image is a .bmp file
    char *bmp = strstr(argv[2], ".bmp");
    if ((bmp != NULL) && strcmp(bmp, ".bmp") == 0)
    {
        printf("\n✅ Input file has .bmp extension\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("❌ Entered file name is wrong \n");
        return e_failure;
    }

    // Use default secret file name if not provided
    if (argv[3] == NULL)
    {
        printf("✅ INFO: Output File not mentioned. Creating secret_file as default\n");
        strcpy(decInfo->secret_fname, "secret_file");
    }
    else
    {
        if (strchr(argv[3], '.') != NULL)
        {
            printf("Error: ❌ Do not include file extension in output filename\n");
            printf("[INFO] Provide filename without extension <output_file>\n");
            return e_failure;
        }
        strcpy(decInfo->secret_fname, argv[3]);
    }

    return e_success;
}

// Main decoding function for full decoding process
Status do_decoding(DecodeInfo *decInfo)
{
    printf("--------------------------------------------------------\n");
    printf("        INFO: ## Decoding Procedure Started ## \n");
    printf("---------------------------------------------------------\n\n");
    printf("✅ INFO: Opening required files\n");

    // Open the stego image for reading
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR:❌ Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    printf("✅ INFO: Opened %s\n", decInfo->stego_image_fname);
    printf("✅ INFO: Done\n\n");

    // Decode and verify magic string
    printf("🔓 Info: Decoding the Magic String\n");
    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("❌ Magic string is not present\n");
        return e_failure;
    }
    printf("✅ INFO: Done\n\n");

    // Decode size of the secret file extension
    printf("🔓 INFO: Decoding Output File Extension size\n");
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("❌ Failed to get extension size\n");
        return e_failure;
    }
    printf("✅ INFO: Done\n\n");

    // Decode the actual secret file extension (like .txt/.c/.sh)
    printf("🔓 INFO: Decoding Output File Extension\n");
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("❌ Falied to get extension\n");
        return e_failure;
    }
    printf("✅ INFO: Done\n\n");

    // Open output file to store the recovered secret
    printf("🔓 INFO: Opening  %s\n", decInfo->secret_fname);
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL) // Bug: Should check decInfo->fptr_secret instead
    {
        perror("fopen");
        fprintf(stderr, "ERROR:❌ Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    printf("✅ INFO: Opened all required files\n");
    printf("✅ INFO: Done\n\n");

    // Decode the size of the secret file
    printf("🔓 INFO: Decoding %s File Size\n", decInfo->secret_fname);
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("❌ Falied to get file size\n");
        return e_failure;
    }
    printf("✅ INFO: Done\n\n");

    // Decode the actual content of the secret file
    printf("🔓 INFO: Decoding the secret file data\n");
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("❌ Failed at decoding file data\n");
        return e_failure;
    }
    printf("✅ INFO: Done\n\n");

    return e_success;
}

// Decodes the embedded magic string from the image
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET); // Skip BMP header
    int i;
    char buffer[8], ch, magicString[strlen(MAGIC_STRING) + 1];

    // Decode each character of the magic string
    for (i = 0; i < strlen(MAGIC_STRING); i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("ERROR:❌ Failed to read  %s while decoding magic string\n", decInfo->stego_image_fname);
            return e_failure;
        }
        decode_byte_from_lsb(&ch, buffer);
        magicString[i] = ch;
    }
    magicString[i] = '\0';

    // Compare decoded magic string with predefined one
    if (strcmp(MAGIC_STRING, magicString) == 0)
    {
        return e_success;
    }
    return e_failure;
}

// Decodes a single byte from LSBs of 8 bytes
Status decode_byte_from_lsb(char *ch, char *buffer)
{
    int i, get, clear, k, j = 7;
    *ch = 0;
    for (i = 0; i <= 7; i++)
    {
        get = buffer[i] & 1;    // Extract LSB
        *ch = *ch | (get << j); // Set bit in result
        j--;
    }
    return e_success;
}

// Decodes the size of the secret file's extension (e.g. 4 for ".txt")
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("ERROR:❌ Failed to read %s while decoding extension size\n", decInfo->stego_image_fname);
        return e_failure;
    }
    decode_int_from_lsb(&(decInfo->secret_file_extn_size), buffer);
    return e_success;
}

// Decodes a 32-bit integer (size) from LSBs
Status decode_int_from_lsb(int *size, char *image_buffer)
{
    *size = 0;
    int i, get, j = 31;
    for (i = 0; i <= 31; i++)
    {
        get = image_buffer[i] & 1;
        *size |= (get << j);
        j--;
    }
    return e_success;
}

// Decodes the actual file extension (like .txt/.sh/.c)
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extension[decInfo->secret_file_extn_size + 1];
    char buffer[8], ch;
    int i;

    for (i = 0; i < decInfo->secret_file_extn_size; i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("ERROR:❌ Failed to read %s while decoding file extension \n", decInfo->stego_image_fname);
            return e_failure;
        }
        decode_byte_from_lsb(&ch, buffer);
        extension[i] = ch;
    }
    extension[i] = '\0';

    // Append extension to the output file name
    strcat(decInfo->secret_fname, extension);

    return e_success;
}

// Decodes the total size of the secret file
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("ERROR:❌ Failed to read %s while decoding secret file size\n", decInfo->stego_image_fname);
        return e_failure;
    }
    decode_int_from_lsb(&(decInfo->size_secret_file), buffer);
    return e_success;
}

// Decodes the actual content of the secret file
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8], ch;
    int i;

    for (i = 0; i < decInfo->size_secret_file; i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("ERROR:❌ Failed to read %s while decoding file data\n", decInfo->stego_image_fname);
            return e_failure;
        }

        decode_byte_from_lsb(&ch, buffer);

        // Write the decoded byte into the secret output file
        if (fwrite(&ch, 1, 1, decInfo->fptr_secret) != 1)
        {
            printf("ERROR:❌ Failed to write %s into %s decoding data\n", decInfo->stego_image_fname, decInfo->secret_fname);
            return e_failure;
        }
    }
    return e_success;
}

// Closes the opened stego and secret output files
void close_decode_files(DecodeInfo *decInfo)
{
    int flag = 0;

    if (decInfo->fptr_stego_image != NULL)
    {
        flag = 1;
        fclose(decInfo->fptr_stego_image);
    }

    if (decInfo->fptr_secret != NULL)
    {
        flag = 1;
        fclose(decInfo->fptr_secret);
    }

    if (flag)
    {
        printf("✅ INFO: Successfully closed files\n");
    }
}
