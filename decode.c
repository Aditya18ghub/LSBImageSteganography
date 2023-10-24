/*
   Name - Aditya Venkata Sheshu
   Date - 3/1/2023
   Project- LSB Image Steganography
   */



#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"


Status open_file(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to the open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    else
	return e_success;
}

// read and validations check
Status read_and_validate_decode_args( char *argv[], DecodeInfo *decInfo)
{
    if ( argv[2] != NULL)
    {
	if ( strcmp(strstr(argv[2],"."), ".bmp") == 0 )
	{
	    decInfo->src_image_fname = argv[2];
	    if ( argv[3] != NULL)
	    {

		decInfo->secret_fname =strtok(argv[3],".");
	    }
	    else
		decInfo->secret_fname = "output";
	    return e_success;
	}
	else
	    return e_failure;
    }
    else
	return e_failure;
}

// Decoding the magic string and verify if it matches with the original
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);
    int size = strlen(MAGIC_STRING);
    char buffer[3]={0}, arr[8];
    int i;
    for( i=0; i < size; i++)
    {
	fread(arr, 1,8, decInfo->fptr_src_image);
	decode_byte_from_lsb(arr, &buffer[i]);
	printf("%c\n", buffer[i]);
    }
    buffer[i] = '\0';
    printf(" MAGIC STRING  MATCH CHECKING \n");
    printf(" magic string is - %s\n",buffer);
    if(strcmp(buffer, MAGIC_STRING) == 0)
    {
	return e_success;
    }
    else
	return e_failure;
}

// Decoding the secret file extension and opening the file
Status decode_secret_file_extn( int size, DecodeInfo *decInfo )
{
    char buffer[8];
    char extn[size+1];
    int i;
    for (i=0; i < size; i++)
    {
	char var=0;
	fread(buffer, 1, 8, decInfo->fptr_src_image);
	decode_byte_from_lsb(buffer, &var);
	extn[i] = var;
    }
    extn[i] = '\0';

    char secret[20];
    strcpy(secret, decInfo->secret_fname);
    strcat(secret, extn);

    decInfo->secret_fname = secret;
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open the file %s\n", decInfo->secret_fname);
    	return e_failure;
    }
    return e_success;
}

// decoding the size of extension 
Status decode_size(DecodeInfo *decInfo, int *size)
{
    char arr[32];
    fread(arr, 1,32, decInfo->fptr_src_image);
    decode_size_from_lsb(arr, size);
    printf("size of extn  %d\n", *size);
    return e_success;
}

// Decoding the size of secret file size
Status decode_secret_file_size(DecodeInfo *decInfo, int *size)
{
    char arr[32];
    fread(arr, 1, 32, decInfo->fptr_src_image);
    decode_size_from_lsb( arr, size);
    return e_success;
}

// Decoding the secret file data and storing to the out put file
Status decode_secret_file_data(DecodeInfo *decInfo, int data_size)
{
    char buffer[8];
    for(int i=0; i < data_size; i++)
    {
	char var=0;
	fread(buffer,1, 8, decInfo->fptr_src_image);
	decode_byte_from_lsb( buffer, &var);
	fwrite( &var, 1, 1, decInfo->fptr_secret);
    }
    return e_success;
}

// Main Decoding the size of extension stored in lsb of each pixel
Status decode_size_from_lsb( char *arr, int *size)
{
    *size = 0;
    for ( int i=0; i< 32; i++)
    {
	*size = ((arr[i] & 1) << i) | *size;
    }
    return e_success;
}

// Decoding the information stored in the LSB of image file 
Status decode_byte_from_lsb(char *arr, char *data)
{
    for (int i=0; i < 8; i++)
    {
	*data = *data | ((arr[i] & 1) << i);
    }
    return e_success;
}

// Start of decoding process
Status do_decoding( DecodeInfo *decInfo)
{
    if( open_file(decInfo) == e_success)
    {
	printf("File opened ...\n");
	if( decode_magic_string(decInfo) == e_success)
	{
	    printf("magic string matched...\n");
	    if( decode_size(decInfo, &decInfo->extn_size) == e_success)
	    {
		printf("size of extension successfully decoded...\n");
		if ( decode_secret_file_extn(decInfo->extn_size, decInfo) == e_success)
		{
		    printf("secrete file extension decoded success...\n");
		    if ( decode_secret_file_size(decInfo, &decInfo->size_secret_file) == e_success)
		    { 
			printf("secret file size decoded...\n");

			if( decode_secret_file_data(decInfo, decInfo->size_secret_file) == e_success)
			{
			    printf("secret file data decoded success...\n");
			    return e_success;
			}
			else
			    return e_failure;
		    }
		    else
			return e_failure;
		}
		else
		    return e_failure;
	    }
	    else
		return e_failure;
	}
	else
	    return e_failure;
    }
    else
	return e_failure;

    fclose(decInfo->fptr_src_image);
    fclose(decInfo->fptr_secret);
}



