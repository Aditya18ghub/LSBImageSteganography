/* Documentation
Name : Aditya Venkata Sheshu
Project : LSB Image Steganography
Date : 02/01/23
*/ 
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if ( check_operation_type(argv) == e_encode)
    {
	printf("Encode\n");
	if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	{
	    printf("ENCODING...\n");
	    if(do_encoding(&encInfo) == e_success)
	    {
		printf("Encoding success\n");

	    }
	    else
	    {
	       	printf("Encoding failed\n");
	    }

	}
	else
	    printf("input valid arguments and files\n");
    }
    else if( check_operation_type(argv) == e_decode)
    {
	printf("DECODING...\n");
	if(read_and_validate_decode_args(argv, &decInfo) == e_success)
	{
	    if(do_decoding(&decInfo) == e_success)
	    {
		printf("Decoding Success\n");
	    }
	    else
		printf("Decoding failed\n");
	}
	else
	    printf("input valid arguments and files\n");

    }
    else
    	printf("Invalid input \n");
}

//Checking the operation type
OperationType check_operation_type( char *argv[])
{
    if ( strcmp(argv[1], "-e") == 0)
    {
	return e_encode;
    }
    else if ( strcmp(argv[1], "-d") == 0)
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}
    	

