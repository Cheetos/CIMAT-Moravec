#include <stdio.h>
#include "image.h"

//Image *Create_Image(int w, int h, int c, int type);
//Image *Load_Image(const char *);

int main()
{
	//freopen("out.ppm","w",stdout);
	
	int i, j;
	Image *img = Load_PPM_Image("basilica.ppm");
	if(img == NULL)	return;
	
	//printf("ok\n");
	Image *imgFinal = Create_Image(img->width, img->height, img->nChannels, img->maxColor);
	
	Image *imgGrey = Color_To_Grey(img);
	//if(!Save_PGM_Image(imgGrey,"BasilicaGrey.pgm"))
	//	printf("No se pudo guardar la imagen\n");
		
	Image *imgSobel = Sobel_Filter(imgGrey);
	if(!Save_PGM_Image(imgSobel,"BasilicaSobel.pgm"))
		printf("No se pudo guardar la imagen\n");
		
	/*Image *imgDerivateX = DerivateX(img);
	if(!Save_PGM_Image(imgDerivateX,"BasilicadX.pgm"))
		printf("No se pudo guardar la imagen\n");
	
	Image *imgDerivateY = DerivateY(img);
	if(!Save_PGM_Image(imgDerivateY,"BasilicadY.pgm"))
		printf("No se pudo guardar la imagen\n");
			
	Emphasize_Red(img, imgFinal);
	if(!Save_PPM_Image(imgFinal,"BasilicaRed.ppm"))
		printf("No se pudo guardar la imagen\n");
	
	Invert_Image(img, imgFinal);
	if(!Save_PPM_Image(imgFinal,"BasilicaInvert.ppm"))
		printf("No se pudo guardar la imagen\n");
		
	Change_RG(img, imgFinal);
	if(!Save_PPM_Image(imgFinal,"BasilicaRedtoGreen.ppm"))
		printf("No se pudo guardar la imagen\n");*/
		
	Image *imgMoravec = Moravec(imgGrey,3);
	Cornerness(imgMoravec,img,500.0);
	
	if(!Save_PPM_Image(img,"BasilicaMoravec.ppm"))
		printf("No se pudo guardar la imagen\n");
		
	Release_Image(&img);
	Release_Image(&imgMoravec);
	//Release_Image(&imgSobel);
	//Release_Image(&imgFinal);
	Release_Image(&imgGrey);
	//Release_Image(&imgDerivateX);
	//Release_Image(&imgDerivateY);
	
	return 0;
}
