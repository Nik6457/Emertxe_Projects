#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    // call check_operation
   if (check_operation_type(argv) == e_encode)
   {
      printf("Selected Encoding\n");
      EncodeInfo encInfo;
      if(read_and_validate_encode_args(argv, &encInfo) == e_success)
      {
        printf("Read and validate encoding arguments is successful\n");
        if(do_encoding(&encInfo) == e_success)
        {
            printf("Encoding is successfull\n");
            
        }
        else
        {
            printf("Failure: Encoding is not done\n");
        }
      }
      else
      {
        printf("Failure: Read and validate encoding arguments is unsuccessful\n");
      }

   }

   else if(check_operation_type(argv) == e_decode)
   {
      printf("Selected Decoding\n");
      DecodeInfo decInfo;
      /*Read and validate decoding arguments*/
      if(read_and_validate_decode_args(argv, &decInfo) == 0)
      {
        printf("Read and validate decoding arguments is successfull\n");
        if(do_decoding(&decInfo) == e_success)
        {
            printf("Decoding is successfull\n");
        }
        else
        {
            printf("Failure: Decoding is not done\n");
        }
      }
      else
      {
        printf("Failure: Read and validate decoding arguments\n");
      }
   }
   else
   {
     printf("Invalid Option\n");
     printf("*********************Usage**********************\n");
     printf("Encoding: ./a.out -e beautiful.bmp secrect.txt stego.bmp\n");
     printf("Decoding: ./a.out -d stego.bmp output.txt\n");
   }
   
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }

}

