#ifndef IMAGE_H
#define IMAGE_H

#define MAX_CHANNELS 3
#define MAX_WIDTH 640
#define MAX_HEIGHT 480

typedef struct
{
	int width;
	int height;
	int nChannels;
	int widthBytes;
	float maxColor;
	float minColor;
	float data[MAX_CHANNELS][MAX_HEIGHT][MAX_WIDTH];	//Se utiliza una matriz de memoria estatica puesto que el 
}Image;															//manejo de apuntadores ocasino conflictos en la lectura
																//El uso de triple apuntador se queda pendiente
/*typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}rgbPixel;*/

Image *Create_Image(int w, int h, int c, float maxColor);
Image *Load_PPM_Image(const char *);
Image *Load_PGM_Image(const char *);
Image *Clone_Image(Image *);
Image *Sobel_Filter(Image *);
//Image *DerivateX(Image *);
//Image *DerivateY(Image *);
Image *Color_To_Grey(const Image *);
Image *Moravec(Image *imgSrc, int v);

double Pixel_Distance(Image *, int, int, int, int);
int Cornerness(Image *, Image *, double);
int Float_To_Char(Image *);
int Invert_Image(Image *imgSrc, Image *imgDest);
int Emphasize_Red(Image *imgSrc, Image *imgDest);
int Change_RG(Image *imgSrc, Image *imgDest);
int Save_PPM_Image(Image *, const char *);
int Save_PGM_Image(Image *, const char *);
int Copy_Image(Image *, Image *);
int Release_Image(Image **);
void Clean(char *, char);


#endif
