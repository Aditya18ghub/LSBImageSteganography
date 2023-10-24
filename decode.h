#ifndef DECODE_H
#define DECODE_H

#include "types.h" // user defined types

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    int extn_size;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    int size_secret_file;

} DecodeInfo;




/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate the decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(int size, DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo, int *);

Status decode_size(DecodeInfo *decInfo, int * );

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo, int );


/* Decode a byte from LSB of the image data array */
Status decode_byte_from_lsb(char *image_buffer, char *data);

Status decode_size_from_lsb(char *image_buffer, int *data);

#endif
