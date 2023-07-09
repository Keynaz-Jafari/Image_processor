
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

#include "bmp.h"
#include "url_function.h"

#define STB_IMAGE_IMPLEMENTATION

#include "./stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "./stb_image/stb_image_write.h"


// detect format

int detectImageFormat(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }

    unsigned char header[4];
    size_t bytesRead = fread(header, sizeof(unsigned char), 4, file);
    fclose(file);

    if (bytesRead < 4) {
        printf("Error reading file: %s\n", filename);
        return -1;
    }

    if (header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) {
        return 2; // png
    } else if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return 1; // jpg
    } else if (header[0] == 0x42 && header[1] == 0x4D) {
        return 3; // bmp
    } else {
        return -1;
    }
}

// strget//

char *get_str() {
    int c;
    int i = 0;
    char *arr = malloc(10000 * sizeof(char));
    while ((c = getchar()) != EOF && c != '\n') {
        arr[i] = c;
        ++i;
        arr[i] = '\0';
    }
    int len = 0;
    for (int j = 0; arr[j] != '\0'; ++j)
        len++;

    char *arr_out = malloc((len + 1) * sizeof(char));
    int k;
    for (k = 0; arr[k] != '\0'; ++k) {
        arr_out[k] = arr[k];
        arr_out[k + 1] = '\0';
    }

    return arr_out;
}

void ChannelShiftFilter(unsigned char *image, int width, int height, int channels, char *filename) { // har pixel ro 30 ta shift mikone jelo
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            for (int rgb = 0; rgb < channels; ++rgb) {
                image[pixelIndex] = image[pixelIndex + rgb + 30];
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void invert_colors(unsigned char *image, int width, int height, int channels, char *filename) { //mokamel mikone range pixel haro 
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex]; //R
            unsigned char *g = &image[pixelIndex + 1]; //G
            unsigned char *b = &image[pixelIndex + 2]; //B
            *r = 255 - *r;
            *g = 255 - *g;
            *b = 255 - *b;
            if (channels == 4) { // ba shape kari ndre , ba rang kar dre
                unsigned char *a = &image[pixelIndex + 3];
                *a = *a;
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void SelectiveXFilter(unsigned char *image, int width, int height, int channels, int start, char *filename) { // az yejayi be baad hame dataha sabet mishe 
    int constant = ((height / start) * width) * channels;
    for (int i = height / start; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelIndex = (i * width + j)*channels;
            image[pixelIndex] = image[constant + channels * j];
            image[pixelIndex + 1] = image[constant + channels * j + 1];
            image[pixelIndex + 2] = image[constant + channels * j + 2];
            if(channels==4){
                image[pixelIndex + 3] = image[constant + channels * j  + 3];
            }
        }
    }


    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}

void SelectiveYFilter(unsigned char *image, int width, int height, int channels, int start, char *filename) {
    
   
        for (int i = 0; i < height; ++i) {
        for (int j = width / start; j < width; ++j) {
            int pixelIndex = (i * width + j) * channels;

            image[pixelIndex] = image[((i * width + width / start)) * channels];
            image[pixelIndex + 1] = image[((i * width + width / start)) * channels + 1];
            image[pixelIndex + 2] = image[((i * width + width / start)) * channels + 2];
            if(channels==4){
                image[pixelIndex + 3]=image[((i * width + width / start)) * channels + 3];
            }
        }
    }


    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}

void HorizontalReflectionFilter(unsigned char *image, int width, int height, int channels, char *filename) { // rast to mese chap mikone
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            int left = width * i + j;
            int right = width * (i + 1) - j;
            for (int rgb = 0; rgb < channels; ++rgb) {
                image[(right) * channels + rgb] = image[(left) * channels + rgb];
            }
        }
    }

    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void VerticalReflectionFilter(unsigned char *image, int width, int height, int channels, char *filename) { //bala ro mese payin mikone
    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width; ++j) {
            int up = width * i + j;
            int down = (height - i) * width + j;
            for (int rgb = 0; rgb < channels; ++rgb) {
                image[(down) * channels + rgb] = image[(up) * channels + rgb];
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void DoubleMirrorFilter(unsigned char *image, int width, int height, int channels, char *filename) {
    VerticalReflectionFilter(image, width, height, channels, filename);
    HorizontalReflectionFilter(image, width, height, channels, filename);

    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void PosterizationFilter(unsigned char *image, int width, int height, int levels, int channels, char *filename) { //
    int step = 255 / levels; // level tedad range ke gharare too ax estefade she , harchi level bishtar keifiat balatar mire 
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];

            *r = (*r / step) * step;
            *g = (*g / step) * step;
            *b = (*b / step) * step;
            if (channels == 4) {
                unsigned char *a = &image[pixelIndex + 3];
                *a = *a;
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void HorizontalStripeFilter(unsigned char *image, int width, int height, int channels, char *filename) { // rah rah ofoghi
   
    for (int i = 0; i <height ; ++i) {
        for (int j = 0; j < width; ++j) {
            if(j%5==0){ // khoone mazrabe 5 ro sefr krde = siah 
                for (int rgb = 0; rgb < 3; ++rgb) {
                    image[(i*width+j)*channels+rgb]=0;
                }
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

 
}

void VerticalStripeFilter(unsigned char *image, int width, int height, int channels, char *filename) { // rah rah amoodi
    // Allocate memory for the pixel data
   
    for (int i = 0; i <height ; ++i) {
        for (int j = 0; j < width; ++j) {
            if(i%5==0){
                for (int rgb = 0; rgb < 3; ++rgb) {
                    image[(i*width+j)*channels+rgb]=0;
                }
            }
        }
    }



    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void convertToGray(unsigned char *image, int width, int height, int channels, char *filename) { 
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];

            // Convert to grayscale using the luminosity method
            unsigned char gray = (unsigned char) ((*r * 0.3) + (*g * 0.59) + (*b * 0.11));

            // Set the pixel values to the grayscale value
            *r = gray;
            *g = gray;
            *b = gray;
        }}
    
        if (detectImageFormat(filename) == 1) {
            int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
            if (success == 0) {
                printf("Failed to save the image.\n");
            }
        } else if (detectImageFormat(filename) == 2) {
            int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
            if (success == 0) {
                printf("Failed to save the image.\n");
            }
        }
       
    
}

void convertToYellow(unsigned char *image, int width, int height, int channels, char *filename) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *b = *b * 0.0;
            *r = *r * 1.0;
            *g = *g * 1.0;

        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void convertToBlue(unsigned char *image, int width, int height, int channels, char *filename) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];

            // Convert to grayscale using the luminosity method
            *b = *b * 1.0;
            *r = *r * 0.0;
            *g = *g * 0.0;
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void ReddishToneFilter(unsigned char *image, int width, int height, int channels, char *filename) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];

            // Increase blue channel
            *b = *b * 0.0;
            *r = *r * 1;
            *g = *g * 0.0;
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}
void SelectiveColorFilter(unsigned char *image, int width, int height, int channels,float rw,float gw,float bw,char* filename) {
   for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];

            // Convert to grayscale using the luminosity method
            *b = *b * bw;
            *r = *r * rw;
            *g = *g * gw;
        }
    }
     if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}

void EnhancedBlurFilter(unsigned char *image, int width, int height, int channels, char *filename) { 
    unsigned char *BlureImage = (unsigned char *) malloc(width * height * channels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char *now = &image[(i * width + j) * channels];
            unsigned char *up = &image[((i - 1) * width + j) * channels];
            unsigned char *up1 = &image[((i - 2) * width + j) * channels];
            unsigned char *up2 = &image[((i - 1) * width + j - 1) * channels];
            unsigned char *up3 = &image[((i - 1) * width + j + 1) * channels];
            unsigned char *down = &image[((i + 1) * width + j) * channels];
            unsigned char *down1 = &image[((i + 2) * width + j) * channels];
            unsigned char *down2 = &image[((i + 1) * width + j - 1) * channels];
            unsigned char *down3 = &image[((i + 1) * width + j + 1) * channels];
            unsigned char *left = &image[(i * width + (j - 1)) * channels];
            unsigned char *left1 = &image[(i * width + (j - 2)) * channels];
            unsigned char *right = &image[(i * width + (j + 1)) * channels];
            unsigned char *right1 = &image[(i * width + (j + 2)) * channels];

            unsigned char *blurePixle = &BlureImage[(i * width + j) * channels];
            for (int rgb = 0; rgb < 3; ++rgb) {
                blurePixle[rgb] =
                        (now[rgb] + up[rgb] + down[rgb] + right[rgb] + left[rgb] + up1[rgb] + down1[rgb] + right1[rgb] +
                         left1[rgb] + down3[rgb] + down2[rgb] + up2[rgb] + up3[rgb]) / 13;
            }
            if (channels == 4) {
                blurePixle[3] = now[3];
            }
        }
    }
    for (int i = 0; i < width * height * channels; ++i) {
        image[i] = BlureImage[i];
    }

    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

    // Clean up
    free(BlureImage);
}

void ChessboardPatternFilter(unsigned char *image, int width, int height, int channels, char *filename) {
    unsigned char *BlureImage = (unsigned char *) malloc(width * height * channels);
     for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(i%5==0 || j%5==0){
                for (int rgb = 0; rgb < 3; ++rgb) {
                    image[(i*width+j)*channels+rgb]=0;
                }
            }
        }

    }    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
    // Clean up
    free(BlureImage);
}

void MirrorFilter (unsigned char *image, int width, int height, int channels, char *filename) { // harchi chape mibare rast harchi raste mire chap
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            int right = width * i + j;
            int left = width * i + width - j - 1;
            for (int rgb = 0; rgb < channels; ++rgb) {
                unsigned char temp = image[(right) * channels + rgb];
                image[(right) * channels + rgb] = image[(left) * channels + rgb];
                image[(left) * channels + rgb] = temp;
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void MirrorHorizontalFilter(unsigned char *image, int width, int height, int channels, char *filename) {
    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width; ++j) {
            int up = width * i + j;
            int down = (height - i - 1) * width + j;
            for (int rgb = 0; rgb < channels; ++rgb) {
                unsigned char temp = image[up * channels + rgb];
                image[up * channels + rgb] = image[down * channels + rgb];
                image[down * channels + rgb] = temp;
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void DualMirrorFilter(unsigned char *image, int width, int height, int channels, char *filename) {
    MirrorFilter (image, width, height, channels, filename);
    MirrorHorizontalFilter(image, width, height, channels, filename);
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
    
}

void applySepia(unsigned char *image, int width, int height, int channels, char *filename) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelImage = (width * i + j) * channels;
            unsigned char r = image[pixelImage];
            unsigned char g = image[pixelImage + 1];
            unsigned char b = image[pixelImage + 2];


            unsigned char NRed = (unsigned char) fmin((0.393 * r) + (0.769 * g) + (0.189 * b), 255);
            unsigned char NGreen = (unsigned char) fmin(0.349 * r + 0.686 * g + 0.168 * b, 255);
            unsigned char NBlue = (unsigned char) fmin(0.272 * r + 0.534 * g + 0.131 * b, 255);

            image[pixelImage] = NRed;
            image[pixelImage + 1] = NGreen;
            image[pixelImage + 2] = NBlue;

            if (channels == 4) {
                image[pixelImage + 3] = image[pixelImage + 3];
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void ColorBoostFilter(unsigned char *image, int width, int height, int channels, char *filename) { // rang ro tire tar mikone 
    double MaxAvrage = 0.0;
    double avrage = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            for (int k = 0; k < channels; ++k) {
                avrage += image[pixelIndex + k];
            }
            avrage = avrage / channels;
            if (avrage > MaxAvrage)MaxAvrage = avrage;
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            double r1 = (double) ((*r / MaxAvrage) * 255);
            double g1 = (double) ((*g / MaxAvrage) * 255);
            double b1 = (double) ((*b / MaxAvrage) * 255);
            *r = (unsigned char) r1;
            *g = (unsigned char) g1;
            *b = (unsigned char) b1;
            if (channels == 4) {
                unsigned char *a = &image[pixelIndex + 3];
                double a1 = (double) ((*a / MaxAvrage) * 255);
                *a = (unsigned char) a1;
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }


}

float hueToRgb(float p, float q, float t) {
    if (t < 0.0f) {
        t += 1.0f;
    } else if (t > 1.0f) {
        t -= 1.0f;
    }
    if (t < 1.0f / 6.0f) {
        return p + (q - p) * 6.0f * t;
    } else if (t < 0.5f) {
        return q;
    } else if (t < 2.0f / 3.0f) {
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    } else {
        return p;
    }
}

void saturate(unsigned char* data, int width, int height, int channels, float factor,char *filename)
{
    // Iterate over each pixel in the image
    for (int i = 0; i < width * height; i++)
    {
        // Calculate the position of the current pixel in the data array
        int pos = i * channels;

        // Convert the RGB/RGBA color of the pixel to HSL
        float r = data[pos] / 255.0f;
        float g = data[pos + 1] / 255.0f;
        float b = data[pos + 2] / 255.0f;

        float max = fmaxf(r, fmaxf(g, b));
        float min = fminf(r, fminf(g, b));
        float h, s, l = (max + min) / 2.0f;

        if (max == min)
        {
            h = s = 0; // achromatic
        }
        else
        {
            float d = max - min;
            s = l > 0.5 ? d / (2.0f - max - min) : d / (max + min);
            if (max == r)
            {
                h = (g - b) / d + (g < b ? 6.0f : 0.0f);
            }
            else if (max == g)
            {
                h = (b - r) / d + 2.0f;
            }
            else
            {
                h = (r - g) / d + 4.0f;
            }
            h /= 6.0f;
        }

        // Modify the saturation value by the specified factor
        s *= factor;
        if (s > 1.0f)
        {
            s = 1.0f;
        }

        // Convert the modified HSL color back to RGB/RGBA
        float q = l < 0.5 ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        float rgba[4] = { r, g, b, 1.0f };

        // Compute hueToRgb for each channel
        for (int c = 0; c < 3; c++)
        {
            float channel_h = h + (c / (float)channels) * (1.0f / 3.0f);
            rgba[c] = hueToRgb(p, q, channel_h);
        }

        // Assign the new RGB/RGBA values to the pixel in the data array
        for (int c = 0; c < 3; c++)
        {
            data[pos + c] = (unsigned char)(rgba[c] * 255.0f);
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, data, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, data, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}

void ThresholdFilter(unsigned char *image, int width, int height, int channels, int thresholther, char *filename) { // harchi bishtar az vurudi bashe mikone 255, kamtar ro mikne 0
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelIndex = (width * i + j) * channels;
            double avrage = (image[pixelIndex] + image[pixelIndex + 1] + image[pixelIndex + 2]) / 3.0; // average of rgb ha 
            if (thresholther < avrage) {
                for (int k = 0; k < 3; ++k) {
                    image[pixelIndex + k] = 255;
                }
            } else {
                for (int k = 0; k < 3; ++k) {
                    image[pixelIndex + k] = 0;
                }
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

}

void MorseCodeFilter (unsigned char *image, int width, int height, int channels, char *filename) {
    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {
            if (j % 4 == 0) {
                for (int rgb = 0; rgb < 3; ++rgb) {
                    image[(i * j * channels) + rgb] = 0;
                }
            }
        }
    }
     if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}
void CheckerboardFilter(unsigned char *image, int width, int height,int channels,char *filename) {// khoone ha yeki dar mioon siah sefid mikone
    int fac=1;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(fac==1){
                for (int rgb = 0; rgb < 3; ++rgb) {
                    image[(i*width+j)*channels+rgb]=0;
                }
            }fac*=-1;
        }fac*=-1;
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}
void brightnessAdjustmentFilter (unsigned char* image, int width, int height, int channels, int brightness,char *filename) {
    int i, j;

    // Adjust brightness for each pixel
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            // Get the current pixel
            int imageIndex = (width * i + j) * channels;

            // Adjust the brightness level
            int nr = (image[imageIndex] + brightness);
            int ng = (image[imageIndex + 1] + brightness);
            int nb = (image[imageIndex + 2] + brightness);

            // Make sure the values are within the valid range (0-255)
            image[imageIndex] = (nr > 255) ? 255 : ((nr < 0) ? 0 : nr);
            image[imageIndex + 1] = (ng > 255) ? 255 : ((ng < 0) ? 0 : ng);
            image[imageIndex + 2] = (nb > 255) ? 255 : ((nb < 0) ? 0 : nb);
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}
void darknessAdjustmentFilter(unsigned char* image, int width, int height, int channels, float contrast,char *filename) {
    float factor = (259.0f * (contrast + 255.0f)) / (255.0f * (259.0f - contrast));

    // Adjust contrast for each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < 3; c++) {
                int imageIndex = (i * width + j) * channels + c;
                int newValue = (int)(factor * (image[imageIndex] - 128) + 128);

                // Clamp the pixel value to the valid range [0, 255]
                image[imageIndex] = (unsigned char)(newValue > 255 ? 255 : (newValue < 0 ? 0 : newValue));
            }
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}
void rainbow(unsigned char *image, int width, int height,int channels,char *filename) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width/7; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 1.0;
            *g = *g * 0.0;
            *b = *b * 0.0;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = width/7; j < 2*width/7; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 1.0;
            *g = *g * 165.0/256.0;
            *b = *b * 0.0;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 2*width/7; j < 3*width/7; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 1.0;
            *g = *g * 1.0;
            *b = *b * 0.0;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 3*width/7; j < 4*width/7; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 0.0;
            *g = *g * 0.5;
            *b = *b * 0.0;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 4*width/7; j < 5*width/7; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 0.0;
            *g = *g * 0.0;
            *b = *b * 1.0;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 5*width/7; j < 6*width/7; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 75.0/256.0;
            *g = *g * 0.0;
            *b = *b * 130.0/256.0;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 6*width/7; j < width; ++j) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];
            *r = *r * 148.0/256.0;
            *g = *g * 0.0/256.0;
            *b = *b * 211.0/256.0;
        }
    }
    if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
}


void resize_image(unsigned char* input, int input_width, int input_height,int channels, int output_width, int output_height,unsigned char* output) {
    double width_ratio = (double)input_width / (double)output_width;
    double height_ratio = (double)input_height / (double)output_height;

    for (int y = 0; y < output_height; ++y) {
        for (int x = 0; x < output_width; ++x) {
            int input_x = (int)(x * width_ratio);
            int input_y = (int)(y * height_ratio);

            int index1 = (input_y * input_width + input_x) * channels;
            int index2 = ((input_y + 1) * input_width + input_x) * channels;
            int index3 = (input_y * input_width + input_x + 1) * channels;
            int index4 = ((input_y + 1) * input_width + input_x + 1) * channels;

            double x_ratio = (x * width_ratio) - input_x;
            double y_ratio = (y * height_ratio) - input_y;

            for (int i = 0; i < channels; ++i) {
                double top = input[index1 + i] + (x_ratio * (input[index3 + i] - input[index1 + i]));
                double bottom = input[index2 + i] + (x_ratio * (input[index4 + i] - input[index2 + i]));
                output[(y * output_width + x) * channels + i] = (unsigned char)(top + (y_ratio * (bottom - top)));
            }
        }
    }
}

void blendImages(unsigned char* image1,int width,int height,int channels, float alpha,char *filename,char* secondInput) {
    int width2, height2, channels2;
    unsigned char *image2 = stbi_load(secondInput, &width2, &height2, &channels2, 0);
    if (image2 == NULL) {
        printf("Failed to load the image.\n");
        return;
    }
    unsigned char *out2 = (unsigned char *) malloc(width  * height * channels);
    resize_image(image2, width2, height2, channels2, width, height, out2);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; ++j) {
            int pixcleIndex=(width*i+j)*channels;
            for (int k = 0; k < channels; ++k) {
                image1[pixcleIndex+k] = (unsigned char)((1 - alpha) * image1[pixcleIndex+k] + alpha * out2[pixcleIndex+k]);
            }
        }
    }
        if (detectImageFormat(filename) == 1) {
        int success = stbi_write_jpg("output.jpg", width, height, channels, image1, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image1, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
    free(out2);
}

void twoimg(unsigned char* image1, int width, int height, int channels,char *filename,char* secondInput) {
    int width2, height2, channels2;
    unsigned char *out1= malloc(width/2*height*channels);
    resize_image(image1,width,height,channels,width/2,height,out1);
    unsigned char *image2=stbi_load(secondInput, &width2, &height2, &channels2, 0);
    if (image2 == NULL) {
        printf("Failed to load the image.\n");
        return;
    }
    unsigned char *out2 = malloc(width/2*height*channels2);
    resize_image(image2,width2,height2,channels2,width/2,height,out2);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width/2; ++j) {
            int pixcelindex=((width*i)+j)*channels;
            int pixcelindex2=((width/2*i)+(j))*channels;
            for (int k = 0; k < channels; ++k) {
                image1[pixcelindex+k]=out1[pixcelindex2+k];
            }
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = width/2; j < width; ++j) {
            int pixcelindex=((width*i)+j)*channels;
            int pixcelindex2=((width/2*i)+(j-width/2))*channels2;
            for (int k = 0; k < channels; ++k) {
                image1[pixcelindex+k]=out2[pixcelindex2+k];
            }
            /*if(channels>channels2){
                image1[pixcelindex+3]=out2[pixcelindex2+3];
            }*/
        }
    }
        if (detectImageFormat(filename) == 1) {
       int success = stbi_write_jpg("output.jpg", width, height, channels, image1, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image1, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
    free(out1);
    free(out2);
}

void frame(unsigned char* image1,int width,int height,int channels,char *filename,char* secondInput){
    int width2, height2, channels2;
    unsigned char *image2 = stbi_load(secondInput, &width2, &height2, &channels2, 0);
    if (image2 == NULL) {
        printf("Failed to load the image.\n");
        return;
    }//image2 is the frame image
   unsigned char *out2 = (unsigned char *) malloc(width  * height * channels);
    resize_image(image2, width2, height2, channels2, width, height, out2);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixcelindex=((width*i)+j)*channels;
            int pixcelindex2=((width*i)+(j))*channels2;
            for (int k = 0; k < channels2; ++k) {
                image1[pixcelindex+k]=out2[pixcelindex2+k];
            }
        }
    }
        if (detectImageFormat(filename) == 1) {
          int success = stbi_write_jpg("output.jpg", width, height, channels, image1, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image1, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }

    free(out2);
}

void BeforAfter(unsigned char* image,int width,int height,int channels,char *filename){
    unsigned char *output= malloc(width*height*channels);
    unsigned char *out1= malloc(width/2*height*channels);
    resize_image(image,width,height,channels,width/2,height,out1);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width/2; ++j) {
            int pixcelindex=((width*i)+j)*channels;
            int pixcelindex2=((width/2*i)+(j))*channels;
            for (int k = 0; k < channels; ++k) {
                output[pixcelindex+k]=out1[pixcelindex2+k];
            }
        }
    }
    convertToGray(out1,width/2,height,channels,filename);


    for (int i = 0; i < height; ++i) {
        for (int j = width/2; j < width; ++j) {
            int pixcelindex=((width*i)+j)*channels;
            int pixcelindex2=((width/2*i)+(j-width/2))*channels;
            for (int k = 0; k < channels; ++k) {
                output[pixcelindex+k]=out1[pixcelindex2+k];
            }
        }
    }
    for (int i = 0; i < width*height*channels; ++i) {
        image[i]=output[i];
    }

    if (detectImageFormat(filename) == 1) {
          int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    } else if (detectImageFormat(filename) == 2) {
        int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
        if (success == 0) {
            printf("Failed to save the image.\n");
        }
    }
    free(out1);
}

void RGBreplace(unsigned char *image, int width, int height,int channels) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelIndex = (i * width + j) * channels;
            unsigned char *r = &image[pixelIndex];
            unsigned char *g = &image[pixelIndex + 1];
            unsigned char *b = &image[pixelIndex + 2];

            unsigned char *temp=&image[pixelIndex];
            *r = *b;
            *g = *g ;
            *b = *temp;
}}}