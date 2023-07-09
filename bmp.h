#include <stdio.h>
#include <stdlib.h>

#pragma pack(push)
#pragma pack(1)

typedef struct {
    char signature[2]; // bebine bmp hast ya na 
    int filesize;
    int reserved;  
    int data_offset; // image datat az koja shuru beshe (shuru az 0)
} bmp_file_header;

typedef struct {
    int header_size; //jayi ke header tamoom muishe lozooman jayi nis ke datat shuru mishe  kar ba data az offset shuru msie
    int width;
    int height;
    short planes;
    short bpp; // bit per pixel ( if 1 = ya sefid ya meshki)
    int compression;  
    int image_size;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    int colors_used;
    int colors_important;
} bmp_info_header;

#pragma pack(pop)


unsigned char* read_bmp(const char *filename, bmp_info_header *info_header,bmp_file_header *file_header);
int write_bmp(const char *output_filename, const char *input_filename , int data_offset,bmp_info_header *info_header, unsigned char *image_data);