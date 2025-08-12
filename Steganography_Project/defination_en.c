/*  Documentation
Name        : G.V.Pavan Kumar
Date        : 25-07-2025
Description : Steganography
*/
#include <stdio.h>
#include "encode.h"
#include <string.h>
#include "types.h"
#include "common.h"
#include <stdlib.h>

// Determine the operation type based on command-line argument
OperationType check_operation_type(char *argv[])
{
    if ((strcmp(argv[1], "-e") == 0))
    {
        return e_encode;
    }
    else if ((strcmp(argv[1], "-d") == 0))
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

// Read and validate arguments for encoding
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Initialize file pointers to NULL
    encInfo->fptr_src_image = NULL;
    encInfo->fptr_secret = NULL;
    encInfo->fptr_stego_image = NULL;

    // Validate source image (must be .bmp)
    char *ch = strchr(argv[2], '.');
    if ((ch != NULL) && (strcmp(ch, ".bmp")) == 0)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("❌ Error : source file must be a .bmp file\n");
        return e_failure;
    }

    // Validate and store secret file extension
    ch = strchr(argv[3], '.');
    if ((ch != NULL) && (strcmp(ch, ".txt")) == 0)
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".txt");
    }
    else if ((ch != NULL) && (strcmp(ch, ".c")) == 0)
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".c");
    }
    else if ((ch != NULL) && (strcmp(ch, ".sh")) == 0)
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".sh");
    }
    else
    {
        printf("❗ Secret file extn must be contain .txt/.c/.sh\n");
        return e_failure;
    }

    // Optional stego image name
    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    else
    {
        ch = strchr(argv[4], '.');
        if ((ch != NULL) && (strcmp(ch, ".bmp")) == 0)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("❗ Encoded file must be .bmp file\n");
            return e_failure;
        }
    }

    return e_success;
}

// Main function to perform the encoding process
Status do_encoding(EncodeInfo *encInfo)
{
    printf("------------------------------------------------\n");
    printf("    INFO: ## Encoding Procedure Started ## \n");
    printf("------------------------------------------------\n");

    // Open files for reading/writing
    if (open_files(encInfo) == e_failure)
    {
        printf("File is not open\n");
        return e_failure;
    }
    printf("✅ Opend Source file to read : beautiful.bmp\n");
    printf("✅ Opend secret file to read : secret.txt\n");
    printf("✅ Opend destination file for writing : stego.bmp\n");
    printf("✅ All files are open successfully\n");
    printf("✅ Done\n\n");

    // Check if image has enough capacity to hold data
    printf("🔍 Checking beautiful.bmp file for space to handle the secret file.txt\n");
    if (check_capacity(encInfo) == e_failure)
    {
        printf("❌ There is no enough space\n");
        return e_failure;
    }
    printf("✅ Image file has sufficient space\n");
    printf("✅ Done\n\n");

    // Copy 54-byte BMP header
    printf("📝 copying the bmp file header into dest file\n");
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("❌ Error : in copying bmp\n");
    }
    printf("✅ Header file copied successfully\n");
    printf("✅ Done\n\n");

    // Encode magic string
    printf("🔐 Encoding the Magic String into the dest\n");
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("❌ Error:magic string encode process\n");
        return e_failure;
    }
    printf("✅ Magic String copied successfully\n");
    printf("✅ Done\n\n");

    // Encode extension length
    printf("🔐 Encoding the secret file extn size into dest\n");
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("❌ File size can't copied successfully\n");
        return e_failure;
    }
    printf("✅ Secret file extn size Encoded successfully\n");
    printf("✅ Done\n\n");

    // Encode actual extension
    printf("🔐 Encoding the secret file extn into the dest\n");
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("❌ Error : in copying secret file extension\n");
        return e_failure;
    }
    printf("✅ Secret file extension copied to image successfully\n");
    printf("✅ Done\n\n");

    // Encode size of the secret file
    printf("🔐 Encodig the secret file size into dest\n");
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("❌ Error: in copying secret file size\n");
        return e_failure;
    }
    printf("✅ Secret file size Encoded successfully\n");
    printf("✅ Done\n\n");

    // Encode the contents of the secret file
    printf("🔐 Encode Secret file data into dest\n");
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("❌ Error: failed in copiying scret file data\n");
        return e_failure;
    }
    printf("✅ Encoded secret file data into image successfully\n");
    printf("✅ Done\n\n");

    // Copy remaining image data that wasn't used for encoding
    printf("🔐 Encodeing remaining data into dest\n");
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("❌ Can't copy the remaining data\n");
        return e_failure;
    }
    printf("✅ Copied remaining data from source to dest successfully\n");
    return e_success;
}

// Check if image has enough capacity to embed secret file
Status check_capacity(EncodeInfo *encInfo)
{
    int total_capacity;

    // Get total number of usable bytes in BMP image
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    // Get size of secret file
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    // Calculate required capacity
    total_capacity = 54 + ((strlen(MAGIC_STRING) + 4 + strlen(encInfo->extn_secret_file) + 4 + encInfo->size_secret_file) * 8);

    // Check if image can hold everything
    if (encInfo->image_capacity >= total_capacity)
    {
        return e_success;
    }

    return e_failure;
}

// Get size of the file
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // Go to end
    return ftell(fptr);       // Return size in bytes
}

// Copy BMP header (first 54 bytes) unchanged
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);
    return e_success;
}

// Encode predefined magic string into image
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if (encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        return e_success;
    else
        return e_failure;
}

// Encode any string (char-by-char) into LSBs of image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    int i;
    // printf("%s\n", data);
    for (i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr_src_image);    // Read 8 bytes from image
        encode_byte_to_lsb(data[i], buffer);    // Modify LSBs to hold 1 byte of data
        fwrite(buffer, 8, 1, fptr_stego_image); // Write modified data
    }
    return e_success;
}

// Encode a single byte into 8 LSBs of image buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i, j = 7, get = 0, clear = 0;

    for (i = 0; i < 8; i++)
    {
        get = (data >> j) & 1; // Get each bit from MSB to LSB
        j--;
        clear = (image_buffer[i] & (~1)); // Clear LSB
        image_buffer[i] = get | clear;    // Set new LSB
    }
    return e_success;
}

// Encode length of file extension into 32 LSBs
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encoInfo)
{
    int i;
    char buffer[32];

    if (fread(buffer, 32, 1, encoInfo->fptr_src_image) != 1)
        return e_failure;

    encode_int_to_lsb(extn_size, buffer);
    fwrite(buffer, 32, 1, encoInfo->fptr_stego_image);
    return e_success;
}

// Encode 32-bit integer into LSBs
Status encode_int_to_lsb(int data, char *image_buffer)
{
    int get, clear, i, j = 31;
    for (i = 0; i < 32; i++)
    {
        get = (data >> j) & 1;
        j--;

        clear = (image_buffer[i]) & (~1);
        image_buffer[i] = get | clear;
    }
    return e_success;
}

// Encode actual extension (.txt/.c/.sh) into image
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[strlen(file_extn)];
    encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

// Encode secret file size into image
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32]; // Note: unnecessary allocation for 32 bytes
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
        return e_failure;

    encode_int_to_lsb(file_size, buffer);
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);

    // printf("file size : %ld\n", encInfo->size_secret_file);
    return e_success;
}

// Embed actual content of secret file byte-by-byte
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    rewind(encInfo->fptr_secret); // Reset file pointer to start of secret file

    char buffer[encInfo->size_secret_file];

    if (fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret) != 1)
    {
        return e_failure;
    }

    buffer[encInfo->size_secret_file] = '\0';

    encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);

    return e_success;
}

// Copy remaining image data after encoding is complete
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
