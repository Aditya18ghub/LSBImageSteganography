/*
   Name - Aditya Venkata Sheshu
   Date - 2/1/2023
   Project- LSB Image Steganography
   */


#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

Status encode_size_to_lsb(int data, char *image_buffer);


// Read and validation phase 
Status read_and_validate_encode_args( char *argv[], EncodeInfo *encInfo)
{
    if ( argv[2] != NULL)
    {
	if ( strcmp(strstr(argv[2],"."), ".bmp") == 0 )
	{
	    encInfo->src_image_fname = argv[2];
	    if ( argv[3] != NULL)
	    {
		encInfo->secret_fname = argv[3];
		strcpy(encInfo->extn_secret_file, strstr(argv[3],"."));

		if (argv[4] != NULL)
		{
		    encInfo->stego_image_fname = argv[4];
		}
		else
		    encInfo->stego_image_fname = "output.bmp";

	    }
	    else
	    {
		printf("input the secret file\n");
		return e_failure;
	    }
	    return e_success;
	}
	else
	{

	    printf("input a valid image file\n");
	    e_failure;
	}
    }
    else
	return e_failure;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    
    fseek(fptr_image, 18, SEEK_SET);  //get the 18th byte

    fread(&width, sizeof(int), 1, fptr_image); // read the width
    printf("width : %u\n", width);

    
    fread(&height, sizeof(int), 1, fptr_image);  // Read the height
    printf("height : %u\n", height);

    
    return width * height * 3;   // Returning the total image capacity
}


Status open_files(EncodeInfo *encInfo)
{
    
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    
    if (encInfo->fptr_src_image == NULL)    // perform Error handling
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open the file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open the file %s\n", encInfo->secret_fname);

    	return e_failure;
    }


    
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w"); // Stego Image file
    
    if (encInfo->fptr_stego_image == NULL)  // Performing Error handling
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open the file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // return e_success
    return e_success;
}



//Checking the image capacity
Status check_capacity(EncodeInfo *encInfo)
{
    int sec_len = strlen(encInfo->extn_secret_file);
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
  
    if(encInfo->image_capacity >= (54+(2+4+ sec_len + 4 + encInfo->size_secret_file)*8))
	return e_success;
    else
	return e_failure;
}

// Checking the file size
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

// Copying image file header i.e bmp header information
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(arr, sizeof(char), 54, fptr_src_image);
    fwrite(arr, sizeof(char),54, fptr_dest_image);
    return e_success;
}

//Encode the magic string
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image( magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    return e_success;
    else
	return e_failure;
}

// Encode the secret file extension size and extension
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    int data = strlen(file_extn);
    char buffer[32];
    fread(buffer, sizeof(char), 32, encInfo->fptr_src_image);
    encode_size_to_lsb(data, buffer);
    fwrite(buffer, sizeof(char) ,32, encInfo->fptr_stego_image);

   if(encode_data_to_image(file_extn, data, encInfo->fptr_src_image,  encInfo->fptr_stego_image) == e_success)
	return e_success;
   else
       return e_failure;
}

//Encoding the secret file size
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, sizeof(char), 32, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buffer);
    fwrite(buffer, sizeof(char) ,32, encInfo->fptr_stego_image);
    return e_success;
}

//Encoding the data to the image
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    for (int i=0; i < size; i++)
    {
	char buffer[8];
	fread(buffer, 1, 8, fptr_src_image);
	encode_byte_to_lsb(data[i], buffer);
	fwrite(buffer, 1 ,8, fptr_stego_image);
    }
    return e_success;
}

//Encoding the secret file data
Status encode_secret_file_data( EncodeInfo *encInfo)
{
    char buffer_secret[encInfo->size_secret_file];
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    fread(buffer_secret, sizeof(char) , encInfo->size_secret_file, encInfo->fptr_secret);
   if(encode_data_to_image(buffer_secret, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success) 
       return e_success;
   else
       return e_failure;
}

//Encoding the integer  information in 32 bytes of the image file
Status encode_size_to_lsb(int data, char *image_buffer)
{
    for (int i=0; i < 32 ; i++)
    {
	image_buffer[i] = (image_buffer[i] & (0xfe))  | ((data & 1 << i) >> i);
    }
    return e_success;
}

// Encoding each byte of the information in 8 bytes of image (1:8 ratio) 
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i=0; i < 8 ; i++)
    {
	image_buffer[i] = (image_buffer[i] & (0xfe) ) | ((data & (1 << i)) >> i);
    }
    return e_success;
}

//Copying the remaining part of image data to the output image 
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while((fread(&ch, 1, 1,fptr_src)) > 0)
    {
	fwrite( &ch , 1, 1, fptr_dest);
    }
    return e_success;
}


Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_success)
    {
	if(check_capacity(encInfo) == e_success)
	{
	    if(copy_bmp_header( encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	    {
		if(encode_magic_string( MAGIC_STRING,encInfo) == e_success)
		{
		    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
		    {
			if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
			{
			    if(encode_secret_file_data(encInfo) == e_success)
			    {
				printf("secret file data is encoded sucessfully\n");
				if(copy_remaining_img_data( encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
				{
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
	}
	else
	    return e_failure;
    }
    else
	return e_failure;
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

}

