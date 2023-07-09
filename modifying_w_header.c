
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#ifndef MAIN_H
#define MAIN_H
#include "filters.h"
#include "bmp.h"
#include "./stb_image/stb_image.h"
#include "./stb_image/stb_image_write.h"
#include "url_function.h"
#include "email2_test.h"

#include "url_function.h"
#include "email2_test.h"

int get_int(){
    int res=0;
    int i=0;
    char c;
    for(;(c=getchar())!='\n' && c!=',' && c!=EOF && c!=' ';i++){
        res=res*10 + (c-'0');
    }
    return res;
}



int main() {
    //printf("How do you want to read the image? \n1.from local repository\n2.from the internet\n3.from email\n");
    char c;
    int choice=0;
    int answer;
    char* input_name;
    char* server;
    char* path;
    int n;
    int rw,gw,bw,ThresholdFilterint,levels,XF,YF;
    float factor;
    float alpha;
    char* secondInput;
    unsigned char* img2;
    unsigned char* image_data;
    unsigned char *image;
    int width, height, channels;
    bmp_file_header file_header;
    bmp_info_header info_header;
    unsigned char *output;
    unsigned char *out1;

    while (1) {
        printf("How do you want to read the image?\n1. From local repository\n2. From the internet\n3. From email\n");
        printf("Enter 0 to exit: ");
        scanf(" %d", &answer);
        getchar();

        if(answer<0 || answer>3){
            printf("invalid choice!\nbye!");
            return 1;
        }

        if (answer == 0) {
            printf("You just exited the program! Bye!");
            break;
        }

        if (answer == 1) {
            printf("File input name: ");
            input_name = get_str();
        }
        else if (answer == 2) {
            char* server;
            char* path;
            printf("Enter the server: ");
            server = get_str();
            printf("Enter the path: ");
            path = get_str();
            printf("Enter the downloaded_img_name: ");
            input_name = get_str();
            download_image_ssl(server, path, input_name);
        }
        else if (answer == 3) {
            printf("Enter the downloaded_img_name: ");
            input_name = get_str();
            download_email_attachment(input_name);
        }
        else {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

    printf("Which filter you would like to apply?\n1.ChannelShiftFilter\n2.invert_colors\n3.SelectiveXFilter\n4.SelectiveYFilter\n5.HorizontalReflectionFilter\n6.VerticalReflectionFilter\n");
    printf("7.DoubleMirrorFilter\n8.PosterizationFilter\n9.HorizontalStripeFilter\n10.convertToGray\n11.convertToBlue\n12.ReddishToneFilter\n");
    printf("13.EnhancedBlurFilter\n14.ChessboardPatternFilter\n15.MirrorFilter \n16.MirrorHorizontalFilter\n17.DualMirrorFilter\n18.applySepia\n19.ColorBoostFilter\n");
    printf("20.saturate\n21.ThresholdFilter\n22.VerticalStripeFilter\n23.convertToYellow(ONLY SUPPORTING JPG AND PNG FORMAT!)\n24.SelectiveColorFilter(ONLY SUPPORTING JPG AND PNG FORMAT!)\n25.codemorse\n26.checkerBoardFilter\n27.brightnessAdjustmentFilte\n28.darknessAdjustmentFilter\n29.rainbow\n");
    printf("30.blendImages(ONLY SUPPORTING JPG AND PNG FORMAT!)\n31.twoimg(PAY ATTENTION! you can't use this mode for bmp images!)\n32.frame(PAY ATTENTION! you can only use this mode if the image format entered is png!)\n33.BeforAfter(PAY ATTENTION! you can't use this filter for bmp images!)\n");

     while (1) {
            printf("Enter the filter number (0 to exit)\n: ");
            
            scanf("%d", &choice);
            if(choice>33 || choice <0){
                        printf("invalid number!\n");
                    }
            getchar();

            if (choice == 0) {
                printf("you just exited the program! bye!");
                break; // Exit the inner loop and prompt for image selection again
            }
    image_data = read_bmp(input_name, &info_header, &file_header);



    if (!image_data) {
        // printf( "Failed to read BMP image\n");
        image = stbi_load(input_name, &width, &height, &channels, 0);
        if (image == NULL) {
            printf("Failed to load the image.\n");
            return 1;
        }
        switch (choice) {
            case 0:
                printf("you just exited the program! bye!");
                exit(0);
            case 1:
                ChannelShiftFilter(image, width, height, channels, input_name);
                break;
            case 2:
                invert_colors(image, width, height, channels, input_name);
                break;
            case 3:
                printf("write an integer number from '2' '3' '4' '5':\n ");
                scanf("%d",&XF);
                SelectiveXFilter(image, width, height, channels,XF, input_name);
                break;
            case 4:
                printf("write an integer number from '2' '3' '4' '5':\n ");
                scanf("%d",&YF);
                SelectiveYFilter(image, width, height, channels,YF, input_name);
                break;
            case 5:
                HorizontalReflectionFilter(image, width, height, channels, input_name);
                break;
            case 6:
                VerticalReflectionFilter(image, width, height, channels, input_name);
                break;
            case 7:
                DoubleMirrorFilter(image, width, height, channels, input_name);
                break;
            case 8:
                printf("write an integer number between 1-10 for Posterize level:\n");
                scanf("%d",&levels);
                PosterizationFilter(image, width, height, levels, channels, input_name);
                break;
            case 9:
                HorizontalStripeFilter(image, width, height, channels, input_name);
                break;
            case 10:
                convertToGray(image, width, height, channels, input_name);
                break;
            case 11:
                convertToBlue(image, width, height, channels, input_name);
                break;
            case 12:
                ReddishToneFilter(image, width, height, channels, input_name);
                //printf("tamoommm");
                break;
            case 13:
            //printf("133333");
                EnhancedBlurFilter(image, width, height, channels, input_name);
                break;
            case 14:
                ChessboardPatternFilter(image, width, height, channels, input_name);
                break;
            case 15:
                MirrorFilter (image, width, height, channels, input_name);
                break;
            case 16:
                MirrorHorizontalFilter(image, width, height, channels, input_name);
                break;
            case 17:
                DualMirrorFilter(image, width, height, channels, input_name);
                break;
            case 18:
                applySepia(image, width, height, channels, input_name);
                break;
            case 19:
                ColorBoostFilter(image, width, height, channels, input_name);
                break;
            case 20:
                printf("write an float number between 1-10 for saturation factor:\n");
                scanf("%f",&factor);
                saturate(image, width, height, channels, factor, input_name);
                break;
            case 21:
                printf("write an integer between 0-255:\n");
                scanf("%d",&ThresholdFilterint);
                ThresholdFilter(image, width, height, channels, ThresholdFilterint, input_name);
                break;
            case 22:
                VerticalStripeFilter(image, width, height, channels, input_name);
                break;
            case 23:
                convertToYellow(image,width,height,channels,input_name);
                break;
            case 24:
                printf("choose image color ( write an integer between 0-100 for each R G B ):\n");
                printf("R:");
                scanf("%d",&rw);
                printf("G:");
                scanf("%d",&gw);
                printf("B:");
                scanf("%d",&bw);
                SelectiveColorFilter(image,width,height,channels,rw/100.0,gw/100.0,bw/100.0,input_name);
                break;
            case 25:
                MorseCodeFilter (image,width,height,channels,input_name);
                break;
            case 26:
                 CheckerboardFilter (image,width,height,channels,input_name);

             case 27:
                 brightnessAdjustmentFilter (image,width,height,channels,40,input_name); // brightness chi bzr? va farghesh  ba payini che nam avaz konam/
                break;
            case 28:
                darknessAdjustmentFilter (image,width,height,channels,40,input_name); // contrast chi bzrm?
                break;
             case 29:
                rainbow (image,width,height,channels,input_name);
                break;
            case 30:
                printf("enter the second picture as an input:\n(PAY ATTENTION ! if the first image you entered is png, the second one should also be png format!\n)");
                secondInput = get_str();
                printf("enter a float number between 0 and 1 to determine how much you want to blend (0 means the first image and 1 means the seacond one\n)");
                scanf("%f",&alpha);
                blendImages(image, width, height, channels, alpha,input_name,secondInput);
                break;
            case 31:
                printf("enter the second picture as an input\nPAY ATTENTION! if your first image was png, the second should also be png format!\n");
                secondInput = get_str();
                twoimg(image, width, height, channels, input_name,secondInput);
                break;
            case 32:
                printf("enter the second picture as an input\nPAY ATTENTION! enter either jpg or bmp format here!\n");
                secondInput = get_str();
                frame(image, width, height, channels, input_name,secondInput);
                break;

            case 33:
                //BeforAfter(image, width, height, channels, input_name);
                    output= malloc(width*height*channels);
                    out1= malloc(width/2*height*channels);
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
                    //convertToGray(out1,width/2,height,channels,input_name);
                    //=========================================================================================================================
                    printf("enter a filter to compare with your image:\n");
                    printf("Enter the filter number (0 to exit): \n");
                    scanf("%d", &choice);
                    if(choice>33 || choice <0){
                        printf("invalid number!\n");
                    }
                    getchar();

                            if (choice == 0) {
                                printf("you just exited the program! bye!");
                                exit(0);
                                //break; // Exit the inner loop and prompt for image selection again
                            }
                        switch (choice) {
                            case 0:
                                printf("you just exited the program! bye!");
                                exit(0);
                            case 1:
                                ChannelShiftFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 2:
                                invert_colors(out1, width/2, height, channels, input_name);
                                break;
                            case 3:
                                printf("write an integer number from '2' '3' '4' '5':\n ");
                                scanf("%d",&XF);
                                SelectiveXFilter(out1, width/2, height, channels,XF, input_name);
                                break;
                            case 4:
                                printf("write an integer number from '2' '3' '4' '5':\n ");
                                scanf("%d",&YF);
                                SelectiveYFilter(out1, width/2, height, channels,YF, input_name);
                                break;
                            case 5:
                                HorizontalReflectionFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 6:
                                VerticalReflectionFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 7:
                                DoubleMirrorFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 8:
                                printf("write an integer number between 1-10 for Posterize level:\n");
                                scanf("%d",&levels);
                                PosterizationFilter(out1, width/2, height, levels, channels, input_name);
                                break;
                            case 9:
                                HorizontalStripeFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 10:
                                convertToGray(out1, width/2, height, channels, input_name);
                                break;
                            case 11:
                                convertToBlue(out1, width/2, height, channels, input_name);
                                break;
                            case 12:
                                ReddishToneFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 13:
                                EnhancedBlurFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 14:
                                ChessboardPatternFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 15:
                                MirrorFilter (out1, width/2, height, channels, input_name);
                                break;
                            case 16:
                                MirrorHorizontalFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 17:
                                DualMirrorFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 18:
                                applySepia(out1, width/2, height, channels, input_name);
                                break;
                            case 19:
                                ColorBoostFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 20:
                                printf("write an float number between 1-10 for saturation factor:\n");
                                scanf("%f",&factor);
                                saturate(out1, width/2, height, channels, factor, input_name);
                                break;
                            case 21:
                                printf("write an integer between 0-255:\n");
                                scanf("%d",&ThresholdFilterint);
                                ThresholdFilter(out1, width/2, height, channels, ThresholdFilterint, input_name);
                                break;
                            case 22:
                                VerticalStripeFilter(out1, width/2, height, channels, input_name);
                                break;
                            case 23:
                                convertToYellow(out1, width/2,height,channels,input_name);
                                break;
                            case 24:
                                printf("choose image color ( write an integer between 0-100 for each R G B ):\n");
                                printf("R:");
                                scanf("%d",&rw);
                                printf("G:");
                                scanf("%d",&gw);
                                printf("B:");
                                scanf("%d",&bw);
                                SelectiveColorFilter(out1, width/2,height,channels,rw/100.0,gw/100.0,bw/100.0,input_name);
                                break;
                            case 25:
                                MorseCodeFilter (out1, width/2,height,channels,input_name);
                                break;
                            case 26:
                                CheckerboardFilter (out1, width/2,height,channels,input_name);

                            case 27:
                                brightnessAdjustmentFilter (out1, width/2,height,channels,40,input_name); // brightness chi bzr? va farghesh  ba payini che nam avaz konam/
                                break;
                            case 28:
                                darknessAdjustmentFilter (out1, width/2,height,channels,40,input_name); // contrast chi bzrm?
                                break;
                            case 29:
                                rainbow (out1, width/2,height,channels,input_name);
                                break;
                            case 30:
                                printf("enter the second picture as an input:\n(PAY ATTENTION ! you can only use png format with another png format here!)\n");
                                secondInput = get_str();
                                printf("enter a float number between 0 and 1 to determine how much you want to blend (0 means the first image and 1 means the seacond one)\n");
                                scanf("%f",&alpha);
                                blendImages(out1, width/2, height, channels, alpha,input_name,secondInput);
                                break;
                            case 31:
                                printf("enter the second picture as an input\nPAY ATTENTION! if your first image was png, the second should also be png format!\n");
                                secondInput = get_str();
                                twoimg(out1, width/2, height, channels, input_name,secondInput);
                                break;
                            case 32:
                                printf("enter the second picture as an input\nPAY ATTENTION! enter either jpg or bmp format here!\n");
                                secondInput = get_str();
                                frame(out1, width/2, height, channels, input_name,secondInput);
                                break;
                            case 33:
                            printf("you have this filter already! choose another one.\n");
                            break;
                            default:
                                break;
                        }
                        //=========================================================================================================================

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

                            if (detectImageFormat(input_name) == 1) {
                                int success = stbi_write_jpg("output.jpg", width, height, channels, image, width * channels);
                                if (success == 0) {
                                    printf("Failed to save the image.\n");
                                }
                            } else if (detectImageFormat(input_name) == 2) {
                                int success = stbi_write_png("output.png", width, height, channels, image, width * channels);
                                if (success == 0) {
                                    printf("Failed to save the image.\n");
                                }
                            }
                            free(out1);

                        break;
                    default:
                        break;
                }
    } else {

        switch (choice) {
            case 1:
                ChannelShiftFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 2:
                invert_colors(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 3:
                printf("write an integer number from '2' '3' '4' '5':\n ");
                scanf("%d",&XF);
                SelectiveXFilter(image_data, info_header.width, info_header.height, 3,XF, input_name);
                break;
            case 4:
                printf("write an integer number from '2' '3' '4' '5':\n ");
                scanf("%d",&YF);
                SelectiveYFilter(image_data, info_header.width, info_header.height, 3,YF, input_name);
                break;
            case 5:
                HorizontalReflectionFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 6:
                VerticalReflectionFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 7:
                DoubleMirrorFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 8:
                printf("write an integer number between 1-10 for Posterize level:\n");
                scanf("%d",&levels);
                PosterizationFilter(image_data, info_header.width, info_header.height, levels, 3, input_name);
                break;
            case 9:
                HorizontalStripeFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 10:
                convertToGray(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 11:
                //RGBreplace(image_data, info_header.width, info_header.height, 3);
                ReddishToneFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 12:
                
            convertToBlue(image_data, info_header.width, info_header.height, 3, input_name);
                
                break;
            case 13:
                EnhancedBlurFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 14:
                ChessboardPatternFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 15:
                MirrorFilter (image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 16:
                MirrorHorizontalFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 17:
                DualMirrorFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 18:
                applySepia(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 19:
                ColorBoostFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 20:
                printf("write an float number between 1-10 for saturation factor:\n");
                scanf("%f",&factor);
                saturate(image_data, info_header.width, info_header.height, 3, factor, input_name);
                break;
            case 21:
                printf("write an integer between 0-255:\n");
                scanf("%d",&ThresholdFilterint);
                ThresholdFilter(image_data, info_header.width, info_header.height, 3, ThresholdFilterint, input_name);
                break;
            case 22:
                VerticalStripeFilter(image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 23:
                RGBreplace(image_data, info_header.width, info_header.height, 3);
                convertToYellow(image_data, info_header.width, info_header.height, 4, input_name);
                break;
            case 24:
            printf("FORMAT NOT SUPPORTED FOR THIS FILTER!\n");
             //img2 = stbi_load("bad_sample.bmp", &width, &height, &channels, 0);
                //printf("choose image color ( write an integer between 0-100 for each R G B )\n");
               // printf("R:");
                //scanf("%d",&rw);
                //printf("G:");
                //scanf("%d",&gw);
                //printf("B:");
                //scanf("%d",&bw);
                //SelectiveColorFilter(image_data, info_header.width, info_header.height, channels,rw/100.0,gw/100.0,bw/100.0, input_name);
                //stbi_write_bmp("bmp_write.bmp",info_header.width,info_header.height,3,image_data);

                break;
            case 25:
                MorseCodeFilter (image_data, info_header.width, info_header.height, 3, input_name);
                break;

              case 26:
                 CheckerboardFilter (image_data, info_header.width, info_header.height, 3, input_name);

             case 27:
                 brightnessAdjustmentFilter (image_data, info_header.width, info_header.height, 3,40, input_name); // brightness chi bzr?
                break;
            case 28:
                darknessAdjustmentFilter (image_data, info_header.width, info_header.height, 3,40, input_name); // contrast chi bzrm?
                break;
             case 29:
                rainbow (image_data, info_header.width, info_header.height, 3, input_name);
                break;
            case 30:
            printf("ERROR! the format is not supported for this filter!\n");
                //printf("enter the second picture as an input:\n(PAY ATTENTION ! if the first image you entered is png, the second one should also be png format!\n)");
                //secondInput = get_str();
                //printf("enter a float number between 0 and 1 to determine how much you want to blend (0 means the first image and 1 means the seacond one\n)");
                //scanf("%f",&alpha);
                //blendImages(image_data, info_header.width, info_header.height, channels, alpha,input_name,secondInput);
                break;
            case 31:
                printf("you can't use this filter for this format!\n");
                //secondInput = get_str();
                //twoimg(image_data, info_header.width, info_header.height, channels, input_name,secondInput);
                break;
            case 32:
                 printf("you can't use this filter for this format!\n");
                 //printf("enter the second picture as an input\nPAY ATTENTION! enter either jpg or bmp format here!\n");
                //secondInput = get_str();
                //frame(image_data, info_header.width, info_header.height, channels, input_name,secondInput);
                break;

             case 33:
             printf("you can't use this filter for this format!\n"); /////////checkkkkk
                //BeforAfter(image_data, width, height, channels, input_name);
                break;

            default:
                break;
        }
        if (write_bmp("outout.bmp", input_name, file_header.data_offset, &info_header,image_data) != 0) {
            printf("Failed to write BMP image\n");
            free(image_data);
            return -1;
        }
    }}
    return 0;
#endif // MAIN_H
}}