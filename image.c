#include <stdio.h>
#include <string.h>
#include <math.h>
#include "image.h"

Image *Create_Image(int w, int h, int nChannels, float maxColor)	//Crea una localidad de memoria para una imagen
{
	Image *img = malloc(sizeof(Image));
	
	img->width = w;
	img->height = h;
	img->nChannels = nChannels;
	
	img->maxColor = maxColor;
	img->minColor = 0.0;
	
	//img->data = NULL;
	
	return img;
}

Image *Load_PPM_Image(const char *fileName)	
{
	char cad[255], *str;
	FILE *f = fopen(fileName,"r");
	
	if(f == NULL)	//Verifica si el archivo existe
	{
		printf("Archivo %s no encontrado\n", fileName);
		return NULL;
	}
	
	Image *img;
	int line, w, h, color;
	int i, j, k;
	float r, g, b, maxColor;
	char buffer[255];
	
	color = 0;
	/*fscanf(f,"%s", buffer);
	fscanf(f,"%d%d", &w, &h);
	fscanf(f,"%d", &maxColor);
	
	img = Create_Image(w,h,3,maxColor);*/
	k = 0;
	color = 0;
	while(fgets(buffer,255,f))
	{
		Clean(buffer, '#');		//Elimina los comentarios
		if(strcmp(buffer,"") != 0)
		{
			if(k == 0)
				sscanf(buffer,"%s", &cad);
			else if(k == 1)
				sscanf(buffer,"%d %d", &w, &h);
			else if(k == 2)
			{
				//printf("%s\n", str);
				sscanf(buffer,"%f", &maxColor);
				img = Create_Image(w,h,2,maxColor);
				break;
			}
			
			k++;
		}
	}
	
	//Lee los datos de la imagen
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			fscanf(f,"%f%f%f",&r,&g,&b);
			img->data[0][i][j] = r;
			img->data[1][i][j] = g;
			img->data[2][i][j] = b;
		}
	}

	fclose(f);
	
	return img;
}

Image *Load_PGM_Image(const char *fileName)
{
	FILE *f = fopen(fileName,"r");
	
	if(f == NULL)
	{
		printf("Archivo %s no encontrado\n", fileName);
		return NULL;
	}
	
	Image *img;
	int line, w, h, color;
	int i, j, k;
	float gray, maxColor;
	char buffer[255], cad[255], *str;
	
	k = 0;
	color = 0;
	while(fgets(buffer,255,f))
	{
		Clean(buffer, '#');			//Eliminamos los comentarios
		if(strcmp(buffer,"") != 0)
		{
			if(k == 0)
				sscanf(buffer,"%s", &cad);
			else if(k == 1)
				sscanf(buffer,"%d %d", &w, &h);
			else if(k == 2)
			{
				sscanf(buffer, "%f", &maxColor);
				img = Create_Image(w,h,2,maxColor);
				break;
			}
			
			k++;
		}
	}
	/*fscanf(f,"%s", buffer);
	fscanf(f,"%d %d", &w, &h);
	fscanf(f,"%d", &maxColor);
	
	img = Create_Image(w,h,2,maxColor);*/
	
	//Lee los datos de la imagen
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			fscanf(f,"%f", &gray);
			img->data[0][i][j] = gray;
			img->data[1][i][j] = gray;
			img->data[2][i][j] = gray;
		}
	}

	fclose(f);	//cerramos el archivo
	
	return img;
}

void Clean(char *str, char key)		//Funcion utilizada para limpiar los comentarios
{
	int i, j, aux = 0;
	char temp[255];
	
	j = 0;
	for(i=0; str[i] != '\0'; i++)
	{
		if(str[i] != ' ')
			aux = 1;
		if(aux == 1)
		{
			if(str[i] == key)	break;
			temp[j++] = str[i];
		}
	}
	
	temp[j] = '\0';
	strcpy(str,temp);
}

int Save_PPM_Image(Image *img, const char *fileName)	//Funcion para guardar una imagen .ppm
{
	int i, j;
	char cad[255], *str;
	
	strcpy(cad,fileName);
	str = strtok(cad,".");
	str = strtok(NULL,"'\n'");
	if(strcmp(str,"ppm") != 0)	//Verifica la extension .ppm
		return 0;
	
	FILE *f = fopen(fileName, "w");
	
	fprintf(f,"P3\n");
	fprintf(f,"%d %d\n", img->width, img->height);
	fprintf(f,"%.0f\n",img->maxColor);
	for(i=0; i<img->height; i++)
		for(j=0; j<img->width; j++)
			fprintf(f,"%.0f\n%.0f\n%.0f\n", img->data[0][i][j], img->data[1][i][j], img->data[2][i][j]);
	
	fclose(f);
	
	return 1;
}

int Save_PGM_Image(Image *img, const char *fileName)
{
	int i, j;
	float p;
	char cad[255], *str;
	
	strcpy(cad,fileName);
	str = strtok(cad,".");
	str = strtok(NULL,"'\n'");
	if(strcmp(str,"pgm") != 0)		//Verifica la extension .pgm
		return 0;
	
	FILE *f = fopen(fileName, "w");
	
	fprintf(f,"P2\n");
	fprintf(f,"%d %d\n", img->width, img->height);
	fprintf(f,"%.0f\n",img->maxColor);
	for(i=0; i<img->height; i++)
	{
		for(j=0; j<img->width; j++)
		{
			p = (img->data[0][i][j] + img->data[1][i][j] + img->data[2][i][j])/3;
			fprintf(f,"%.0f\n", p);
		}
	}
	
	fclose(f);
	
	return 1;
}

Image *Color_To_Grey(const Image *src)
{
	int i, j, p;
	Image *img = Create_Image(src->width, src->height, src->nChannels, src->maxColor);
	
	for(i=0; i<src->height; i++)
	{
		for(j=0; j<src->width; j++)
		{
			p = (src->data[0][i][j] + src->data[1][i][j] + src->data[2][i][j])/3;	//Promedio de los 3 colores
			img->data[0][i][j] = p;
			img->data[1][i][j] = p;
			img->data[2][i][j] = p;
		}
	}
	
	//Float_To_Char(img);
	return img;
}

int Copy_Image(Image *imgSrc, Image *imgDst)	//Funcion para Copiar Imagen
{
	int i, j;
	
	if(imgSrc->width != imgDst->width || imgSrc->height != imgDst->height || imgSrc->nChannels != imgDst->nChannels)
		return 0;
		
	for(i=0; i<imgSrc->height; i++)
	{
		for(j=0; j<imgSrc->width; j++)
		{
			imgDst->data[0][i][j] = imgSrc->data[0][i][j];
			imgDst->data[1][i][j] = imgSrc->data[1][i][j];
			imgDst->data[2][i][j] = imgSrc->data[2][i][j];
		}
	}	
	
	return 1;
}

Image *Clone_Image(Image *imgSrc)	//Funcion para clonar una imagen
{
	Image *img = Create_Image(imgSrc->width, imgSrc->height, imgSrc->nChannels, imgSrc->maxColor);
	Copy_Image(imgSrc,img);
	
	return img;
}

Image *Sobel_Filter(Image *imgSrc)	//Funcion que aplica el filtro de Sobel sobre una imagen
{
	int i, j;
	float a, b, A[9], max, min;
	
	Image *img = Create_Image(imgSrc->width, imgSrc->height, imgSrc->nChannels, imgSrc->maxColor);
	
	if(img == NULL)
		return NULL;
	
	max = -100000.0;
	min = 100000.0;
	for(i=1; i<imgSrc->height-1; i++)
	{
		for(j=1; j<imgSrc->width-1; j++)
		{
			A[0] = (imgSrc->data[0][i-1][j-1] + imgSrc->data[1][i-1][j-1] + imgSrc->data[2][i-1][j-1])/3;
			A[1] = (imgSrc->data[0][i-1][j] + imgSrc->data[1][i-1][j] + imgSrc->data[2][i-1][j])/3;
			A[2] = (imgSrc->data[0][i-1][j+1] + imgSrc->data[1][i-1][j+1] + imgSrc->data[2][i-1][j+1])/3;
			
			A[3] = (imgSrc->data[0][i][j+1] + imgSrc->data[1][i][j+1] + imgSrc->data[2][i][j+1])/3;
			A[4] = (imgSrc->data[0][i+1][j+1] + imgSrc->data[1][i+1][j+1] + imgSrc->data[2][i+1][j+1])/3;
			
			A[5] = (imgSrc->data[0][i+1][j] + imgSrc->data[1][i+1][j] + imgSrc->data[2][i+1][j])/3;
			A[6] = (imgSrc->data[0][i+1][j-1] + imgSrc->data[1][i+1][j-1] + imgSrc->data[2][i+1][j-1])/3;
			A[7] = (imgSrc->data[0][i][j-1] + imgSrc->data[1][i][j-1] + imgSrc->data[2][i][j-1])/3;
			
			a = (A[2]+2*A[3]+A[4]) - (A[0]+2*A[7]+A[6]);
			b = (A[0]+2*A[1]+A[2]) - (A[6]+2*A[5]+A[4]);
			
			img->data[0][i][j] = sqrt(a*a + b*b);
			img->data[1][i][j] = sqrt(a*a + b*b);
			img->data[2][i][j] = sqrt(a*a + b*b);
			
			if(img->data[0][i][j] > max)
				max = img->data[0][i][j];
				
			if(img->data[0][i][j] < min)
				min = img->data[0][i][j];	
		}
	}
	
	img->maxColor = max;
	img->minColor = min;
	
	Float_To_Char(img);
	
	return img;
}

Image *DerivateX(Image *imgSrc)	//Funcion que obtiene la derivada en X
{
	int i, j;
	float a, b, A[9], max, min;
	
	Image *img = Create_Image(imgSrc->width, imgSrc->height, imgSrc->nChannels, imgSrc->maxColor);
	
	if(img == NULL)
		return NULL;
	
	max = -1000.0;
	min = 1000.0;
	for(i=1; i<imgSrc->height-1; i++)
	{
		for(j=1; j<imgSrc->width-1; j++)
		{
			A[0] = (imgSrc->data[0][i-1][j-1] + imgSrc->data[1][i-1][j-1] + imgSrc->data[2][i-1][j-1])/3;
			A[1] = (imgSrc->data[0][i-1][j] + imgSrc->data[1][i-1][j] + imgSrc->data[2][i-1][j])/3;
			A[2] = (imgSrc->data[0][i-1][j+1] + imgSrc->data[1][i-1][j+1] + imgSrc->data[2][i-1][j+1])/3;
			
			A[3] = (imgSrc->data[0][i][j+1] + imgSrc->data[1][i][j+1] + imgSrc->data[2][i][j+1])/3;
			A[4] = (imgSrc->data[0][i+1][j+1] + imgSrc->data[1][i+1][j+1] + imgSrc->data[2][i+1][j+1])/3;
			
			A[5] = (imgSrc->data[0][i+1][j] + imgSrc->data[1][i+1][j] + imgSrc->data[2][i+1][j])/3;
			A[6] = (imgSrc->data[0][i+1][j-1] + imgSrc->data[1][i+1][j-1] + imgSrc->data[2][i+1][j-1])/3;
			A[7] = (imgSrc->data[0][i][j-1] + imgSrc->data[1][i][j-1] + imgSrc->data[2][i][j-1])/3;
			
			a = (A[2]+2*A[3]+A[4]) - (A[0]+2*A[7]+A[6]);
			b = (A[0]+2*A[1]+A[2]) - (A[6]+2*A[5]+A[4]);
			
			img->data[0][i][j] = a;
			img->data[1][i][j] = a;
			img->data[2][i][j] = a;
			
			
			if(img->data[0][i][j] > max)
				max = img->data[0][i][j];
				
			if(img->data[0][i][j] < min)
				min = img->data[0][i][j];	
		}
	}

	img->maxColor = max;
	img->minColor = min;
		
	Float_To_Char(img);
		
	return img;
}

Image *DerivateY(Image *imgSrc)	//Funcion que obtiene la derivada en Y
{
	int i, j;
	float a, b, A[9], max, min;
	
	Image *img = Create_Image(imgSrc->width, imgSrc->height, imgSrc->nChannels, imgSrc->maxColor);
	
	if(img == NULL)
		return NULL;
	
	max = -1000.0;
	min = 1000.0;
	for(i=1; i<imgSrc->height-1; i++)
	{
		for(j=1; j<imgSrc->width-1; j++)
		{
			A[0] = (imgSrc->data[0][i-1][j-1] + imgSrc->data[1][i-1][j-1] + imgSrc->data[2][i-1][j-1])/3;
			A[1] = (imgSrc->data[0][i-1][j] + imgSrc->data[1][i-1][j] + imgSrc->data[2][i-1][j])/3;
			A[2] = (imgSrc->data[0][i-1][j+1] + imgSrc->data[1][i-1][j+1] + imgSrc->data[2][i-1][j+1])/3;
			
			A[3] = (imgSrc->data[0][i][j+1] + imgSrc->data[1][i][j+1] + imgSrc->data[2][i][j+1])/3;
			A[4] = (imgSrc->data[0][i+1][j+1] + imgSrc->data[1][i+1][j+1] + imgSrc->data[2][i+1][j+1])/3;
			
			A[5] = (imgSrc->data[0][i+1][j] + imgSrc->data[1][i+1][j] + imgSrc->data[2][i+1][j])/3;
			A[6] = (imgSrc->data[0][i+1][j-1] + imgSrc->data[1][i+1][j-1] + imgSrc->data[2][i+1][j-1])/3;
			A[7] = (imgSrc->data[0][i][j-1] + imgSrc->data[1][i][j-1] + imgSrc->data[2][i][j-1])/3;
			
			a = (A[2]+2*A[3]+A[4]) - (A[0]+2*A[7]+A[6]);
			b = (A[0]+2*A[1]+A[2]) - (A[6]+2*A[5]+A[4]);
			
			img->data[0][i][j] = b;
			img->data[1][i][j] = b;
			img->data[2][i][j] = b;
			
			
			if(img->data[0][i][j] > max)
				max = img->data[0][i][j];
				
			if(img->data[0][i][j] < min)
				min = img->data[0][i][j];	
		}
	}
	
	img->maxColor = max;
	img->minColor = min;
	
	Float_To_Char(img);
	
	return img;
}

int Float_To_Char(Image *img)
{
	int i, j;
	float p;
	//Image *img = Clone_Image(src);
	//Image *img = Create_Image(src->width, src->height, src->nChannels, src->maxColor);
	//Copy_Image(src, img);
	
	for(i=0; i<img->height; i++)
	{
		for(j=0; j<img->width; j++)
		{
			//p = 0;
			p = ((img->data[0][i][j] - img->minColor)*255.0)/(img->maxColor - img->minColor);
			img->data[0][i][j] = p;
			
			p = ((img->data[1][i][j] - img->minColor)*255.0)/(img->maxColor - img->minColor);
			img->data[1][i][j] = p;
			
			p = ((img->data[2][i][j] - img->minColor)*255.0)/(img->maxColor - img->minColor);
			img->data[2][i][j] = p;
		}
	}
	
	img->maxColor = 255.0;
	img->minColor = 0.0;
	
	return 1;
}

int Release_Image(Image **img)
{
	if(img == NULL)
		return 0;
	
	
	free(img);
	return 1;
}

int Invert_Image(Image *imgSrc, Image *imgDest)
{
	int i, j;
	
	for(i=0; i<imgSrc->height; i++)
	{
		for(j=0; j<imgSrc->width; j++)
		{
			imgDest->data[0][i][j] = 255.0 - imgSrc->data[0][i][j];
			imgDest->data[1][i][j] = 255.0 - imgSrc->data[1][i][j];
			imgDest->data[2][i][j] = 255.0 - imgSrc->data[2][i][j];
		}
	}
	
	return 1;
}

int Emphasize_Red(Image *imgSrc, Image *imgDest)
{
	int i, j;
	float p;
	
	for(i=0; i<imgSrc->height; i++)
	{
		for(j=0; j<imgSrc->width; j++)
		{
			p = 1.5*imgSrc->data[0][i][j];
			imgDest->data[0][i][j] = p;
			
			if(p > imgDest->maxColor)
				imgDest->maxColor = p;
				
			imgDest->data[1][i][j] = imgSrc->data[1][i][j]/1.5;
			imgDest->data[2][i][j] = imgSrc->data[2][i][j]/1.5;
			
			
		}
	}
	
	Float_To_Char(imgDest);
	return 1;
}

int Change_RG(Image *imgSrc, Image *imgDest)
{
	int i, j, p;
	
	for(i=0; i<imgSrc->height; i++)
	{
		for(j=0; j<imgSrc->width; j++)
		{
			imgDest->data[0][i][j] = imgSrc->data[1][i][j];	
			imgDest->data[1][i][j] = imgSrc->data[0][i][j];
			imgDest->data[2][i][j] = imgSrc->data[2][i][j];
		}
	}
	
	return 1;
}

Image *Moravec(Image *imgSrc, int v)
{
	int i, j, k, l;
	int x, y, x2, y2;
	double d, min, max, A[9];
	
	Image *img = Create_Image(imgSrc->width, imgSrc->height, imgSrc->nChannels, imgSrc->maxColor);
	
	for(i=4; i<imgSrc->height-3; i++)
	{
		for(j=4; j<imgSrc->width-3; j++)
		{	
			/*A[0] = imgSrc->data[i-1][j-1];
			A[1] = imgSrc->data[i-1][j];
			A[2] = imgSrc->data[i-1][j+1];
			A[3] = imgSrc->data[i][j-1];
			A[4] = imgSrc->data[i][j];
			A[5] = imgSrc->data[i][j+1];
			A[6] = imgSrc->data[i+1][j-1];
			A[7] = imgSrc->data[i+1][j];
			A[8] = imgSrc->data[i+1][j+1];*/
			
			min = 10000.0;
			for(k=i-1; k<=i+1; k++)
			{
				for(l=j-1; l<=j+1; l++)
				{
					if(!(k == i && l == j))
					{
						d = Pixel_Distance(imgSrc,i,j,k,l);
						if(d < min)
							min = d;
					}
				}
			}
			
			img->data[0][i][j] = min;
			img->data[1][i][j] = min;
			img->data[2][i][j] = min;
			
			//if(min > img->maxColor)
			//	img->maxColor = min;
		}
	}
	
	//Float_To_Char(img);
	return img;
}

double Pixel_Distance(Image *img, int r1, int c1, int r2, int c2)
{
	int i, j, r, c;
	double d;
	
	//printf("ok\n");
	r = r1-1;
	c = c1-1;
	d = 0.0;
	
	for(i=r2-1; i<r2+1; i++)
	{
		c = c1-1;
		for(j=c2-1; j<c2+1; j++)
		{
			if(!(c == c1 && r == r1))
				d = d + (img->data[0][i][j] - img->data[0][r][c])*(img->data[0][i][j] - img->data[0][r][c]);
			c++;
		}
			
		r++;
	}
	
	return d;
}

int Cornerness(Image *imgSrc, Image *imgDst, double umbrar)
{
	int i, j, k, l, x, y;
	double max;
	
	for(i=1; i<imgDst->height-1; i+=8)
	{
		for(j=1; j<imgDst->width-1; j+=8)
		{
			x = j;
			y = i;
			max = 0.0;
			for(k=i; k<i+8; k++)
			{
				for(l=j; l<j+8; l++)
				{
					if(imgSrc->data[0][k][l] > max)
					{
						max = imgSrc->data[0][k][l];
						x = l;
						y = k;
					}
				}
			}
			
			if(max > umbrar)
			{
				imgDst->data[0][y][x] = 255.0;
				imgDst->data[1][y][x] = 255.0;
				imgDst->data[2][y][x] = 0.0;
				
				imgDst->data[0][y][x+1] = 255.0;
				imgDst->data[1][y][x+1] = 255.0;
				imgDst->data[2][y][x+1] = 0.0;
				
				imgDst->data[0][y][x-1] = 255.0;
				imgDst->data[1][y][x-1] = 255.0;
				imgDst->data[2][y][x-1] = 0.0;
				
				imgDst->data[0][y+1][x] = 255.0;
				imgDst->data[1][y+1][x] = 255.0;
				imgDst->data[2][y+1][x] = 0.0;
				
				imgDst->data[0][y-1][x] = 255.0;
				imgDst->data[1][y-1][x] = 255.0;
				imgDst->data[2][y-1][x] = 0.0;
				
			}
		}
	}
	
	return 1;
}
