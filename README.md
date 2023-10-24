# LSBImageSteganography


This command-line application is designed for steganography and offers the following features:

## Requirements

- The application accepts an image file, specifically in the .bmp format.
- A text file containing the message to be steganographically embedded within the image.
- It analyzes the size of the message file to determine whether it can fit within the provided .bmp image.

## Steganography

- The application provides an option to perform steganography by hiding a message within the image.
- Additionally, it offers the option to steg a "magic string" within the image. This string can be useful for identifying whether the image has been steganographically altered.

## Decryption

- The application allows users to decrypt steganographically modified image files to extract the hidden message.
- This decryption feature can be used to reveal the original content of the image.

## Command-Line Application

- The application is a command-line tool, meaning it's operated via the terminal.
- Users provide input and specify the options as command-line arguments.

### Example Usage

To steganographically embed a message:

```shell
$ stego-app encode --image input.bmp --message secret.txt --output output.bmp
