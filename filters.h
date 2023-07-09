#ifndef FILTERS_H
#define FILTERS_H


// Function declarations
int detectImageFormat(const char *filename);
char* get_str();
void ChannelShiftFilter(unsigned char *image, int width, int height, int channels, char *filename);
void invert_colors(unsigned char *image, int width, int height, int channels, char *filename);
void SelectiveXFilter(unsigned char *image, int width, int height, int channels, int start, char *filename);
void SelectiveYFilter(unsigned char *image, int width, int height, int channels, int start, char *filename);
void HorizontalReflectionFilter(unsigned char *image, int width, int height, int channels, char *filename);
void VerticalReflectionFilter(unsigned char *image, int width, int height, int channels, char *filename);
void DoubleMirrorFilter(unsigned char *image, int width, int height, int channels, char *filename);
void PosterizationFilter(unsigned char *image, int width, int height, int levels, int channels, char *filename);
void HorizontalStripeFilter(unsigned char *image, int width, int height, int channels, char *filename);
void VerticalStripeFilter(unsigned char *image, int width, int height, int channels, char *filename);
void convertToGray(unsigned char *image, int width, int height, int channels, char *filename);
void convertToBlue(unsigned char *image, int width, int height, int channels, char *filename);
void ReddishToneFilter(unsigned char *image, int width, int height, int channels, char *filename);
void EnhancedBlurFilter(unsigned char *image, int width, int height, int channels, char *filename);
void ChessboardPatternFilter(unsigned char *image, int width, int height, int channels, char *filename);
void MirrorFilter (unsigned char *image, int width, int height, int channels, char *filename);
void MirrorHorizontalFilter(unsigned char *image, int width, int height, int channels, char *filename);
void DualMirrorFilter(unsigned char *image, int width, int height, int channels, char *filename);
void applySepia(unsigned char *image, int width, int height, int channels, char *filename);
void ColorBoostFilter(unsigned char *image, int width, int height,int channels,char *filename);
void convertToYellow(unsigned char *image, int width, int height, int channels,char *filename);
float hueToRgb(float p, float q, float t);
void saturate(unsigned char *data, int width, int height,int channels, float factor,char *filename);
void ThresholdFilter(unsigned char *image, int width, int height, int channels, int thresholther,char *filename);
void SelectiveColorFilter(unsigned char *image, int width, int height, int channels,float rw,float gw,float bw,char *filename);
void MorseCodeFilter (unsigned char *image, int width, int height, int channels, char *filename);
void CheckerboardFilter(unsigned char *image, int width, int height,int channels,char *filename);
void brightnessAdjustmentFilter (unsigned char* image, int width, int height, int channels, int brightness,char *filename) ;
void darknessAdjustmentFilter(unsigned char* image, int width, int height, int channels, float contrast,char *filename) ;
void rainbow(unsigned char *image, int width, int height,int channels,char *filename) ;
void resize_image(unsigned char* input, int input_width, int input_height,int channels, int output_width, int output_height,unsigned char* output);
void blendImages(unsigned char* image1,int width,int height,int channels, float alpha,char *filename,char* secondInput);
void twoimg(unsigned char* image1, int width, int height, int channels,char *filename,char* secondInput);
void frame(unsigned char* image1,int width,int height,int channels,char *filename,char* secondInput);
void BeforAfter(unsigned char* image,int width,int height,int channels,char *filename);
void RGBreplace(unsigned char *image, int width, int height,int channels);
#endif