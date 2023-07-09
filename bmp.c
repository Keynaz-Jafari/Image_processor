
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push)
#pragma pack(1)   // used to make sure that the file header & info header are packed tightly , no padding bytes between structure memebers

typedef struct {
    char signature[2]; // bebine bmp hast ya na 
    int filesize;
    int reserved; 
    int data_offset; // image datat az koja shuru beshe (shuru az 0)
} bmp_file_header;

typedef struct {
    int header_size; //jayi ke header tamoom muishe lozooman jayi nis ke data shuru mishe ,kar ba data az offset shuru msie
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

// Read BMP image from file
unsigned char* read_bmp(const char *filename, bmp_info_header *info_header,bmp_file_header *file_header) {
    // Open input file
    FILE *in_file = fopen(filename, "rb");
    if (!in_file) {
        //printf( "Failed to open input file\n");
        return NULL;
    }

    // Read BMP file headers
   
    fread(file_header, sizeof(bmp_file_header), 1, in_file);
    fread(info_header, sizeof(bmp_info_header), 1, in_file);

    // Check if file is valid BMP format
    if (file_header->signature[0] != 'B' || file_header->signature[1] != 'M') {
        //printf( "Invalid BMP file format\n");
        return NULL;
    }

    // Check if image is 24bpp
    if (info_header->bpp != 24) { // 24/3 = 8  each color channel (red, green, blue) is represented using 8 bits
        //printf( "Image must be 24bpp\n");
        return NULL;
    }

    // Allocate memory for image data
    int image_size = info_header->width * info_header->height * 3; //*3 Cuz each pixel requires 3 bytes of storage, with one byte dedicated to each color channel
    unsigned char *image_data = (unsigned char*)malloc(image_size);
    if (!image_data) { // no storage 
        printf( "Failed to allocate memory for image data\n");
        return NULL;
    }

    // Read image data
    fseek(in_file, file_header->data_offset, SEEK_SET); // seek_set=0  , be andazeye offset mibare jelo ke baadesh fread data ro bekhoone
    int bytes_read = 0;
    while (bytes_read < image_size) {
        int bytes_to_read = image_size - bytes_read;
        if (bytes_to_read > 4096) {
            bytes_to_read = 4096;
        }
        int result = fread(image_data + bytes_read, bytes_to_read, 1, in_file);  //fread function returns the number of elements successfully read from the file, avali: memory location where the data read from the file will be stored
        if (result != 1) {
            printf("Failed to read image data\n");
            free(image_data);
            return NULL;
        }
        bytes_read += bytes_to_read;
    }

    // Close input file
    fclose(in_file);

    return image_data;
}


// Write BMP image to file
int write_bmp(const char *output_filename, const char *input_filename , int data_offset,bmp_info_header *info_header, unsigned char *image_data) {
    // Open output file
    FILE *out_file = fopen(output_filename, "wb");
    if (!out_file) {
        printf( "Failed to open output file\n");
        return -1;
    }

    FILE *inp_file = fopen(input_filename,"rb");

    // Write BMP headers
    uint8_t headertw[data_offset];// nemitoonim mostaghin az file bekhoonim berizim tu file, pas arr tarif krdim az byte ha 
    fread(headertw,data_offset,1,inp_file); // az array khundm rikhtm too file 
    fwrite(headertw,data_offset,1,out_file);
 
    // Write image data
    fwrite(image_data, info_header->width*info_header->height*3, 1, out_file); // pixel haro write mikone

    // Close output file
    fclose(inp_file);
    //fclose(out_file); // akhare barname beband 

    return 0;
}




