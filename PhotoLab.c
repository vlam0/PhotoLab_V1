/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2020                 */
/*                                                                   */
/* modifications:                                                    */
/* 09/27/17     RD      adjusted for lecture usage                   */
/* 10/1/18      MC      adjusted for F18                             */
/* 01/11/20     Wendy   adjusted for W20                             */
/*********************************************************************/
/* Enter your name, last name and UCInet ID below:
 * ***************************************************************** *
    * First Name: Vivian
    * Last Name: Lam
    * UCInet ID: vlam8
    */


/*** header files ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*** global definitions ***/

#define WIDTH  600              /* image width */
#define HEIGHT 400              /* image height */
#define SLEN    64              /* maximum length of file names */

/*** function declarations ***/

/* print a menu */
void PrintMenu(void);

/* read image from a file */
/* written for you; should not been modified */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
/* written for you; should not been modified */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* aging the photo */
/* sample function to get you started */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* sharpen the image */
void Sharpen(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* change the image hue */
void Hue(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int degree);

/* test all functions */
void AutoTest(const char fname[SLEN],unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int degree);


int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    FILE *bw, *sharpen, *hue;
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];
    char fname[SLEN], save[100];
    int selection = 0;
    int degree = 60;
    int x, y;

    printf("\n");

    do
    {
        PrintMenu();
        scanf("%d", &selection);
	printf("\n");

        switch (selection)
        {
            case 1: /* Load Image*/
                printf("Please input the file name to load: ");
		scanf("%s", fname);
                LoadImage(fname, R, G, B);
		printf("\n");
                break;

            case 2: /* Save Image*/
                printf("Please input the file name to save: ");
		scanf("%s", save);
                SaveImage(save, R, G, B);
		printf("\n");
                break;

            case 3: /* Change Image to black and white*/
		BlackNWhite(R, G, B);

		bw = fopen("bw", "w");
		for (y = 0; y < HEIGHT; y++)
   		{
       		    for (x = 0; x < WIDTH; x++)
      		    {
            		fputc(R[x][y], bw);
           		fputc(G[x][y], bw);
            		fputc(B[x][y], bw);
       		    }
		}
		fclose(bw);

		printf("\n");
                break;

            case 4: /* Sharpen Image */
		Sharpen(R, G, B);

                sharpen = fopen("sharpen", "w");
                for (y = 0; y < HEIGHT; y++)
                {
                    for (x = 0; x < WIDTH; x++)
                    {
                        fputc(R[x][y], sharpen);
                        fputc(G[x][y], sharpen);
                        fputc(B[x][y], sharpen);
                    }
                }
                fclose(sharpen);

		printf("\n");
                break;

            case 5: /* Change to Hue */
                Hue(R, G, B, degree);

                hue = fopen("hue", "w");
                for (y = 0; y < HEIGHT; y++)
                {
                    for (x = 0; x < WIDTH; x++)
                    {
                        fputc(R[x][y], hue);
                        fputc(G[x][y], hue);
                        fputc(B[x][y], hue);
                    }
                }
                fclose(hue);

		printf("\n");
                break;

            case 6: /* Test all functions */
                AutoTest(fname,R, G, B, degree);
                break;

            case 7: /* Exit */
                break;
 
           default:
                printf("Invalid input\n");

        }


    } while (selection != 7);

 
    return 0;
}

/* takes in a file name without extension */
/* writes file into R, G, B arrays */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File)
    {
        printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0)
    {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH)
    {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT)
    {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255)
    {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File))
    {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File))
    {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

/* takes R, G, B arrays and writes it in a file */
/* filename (fname) has no extension */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File)
    {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File))
    {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /* rename file to image.ppm, convert it to ~/public_html/<fname>.jpg and make it world readable */
    sprintf(SysCmd, "~eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_ext);
    if (system(SysCmd) != 0)
    {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/* age the image to make it look like old picture */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    int x, y;

    for( y = 0; y < HEIGHT; y++ )
    {
        for( x = 0; x < WIDTH; x++ )
        {
            B[x][y] = ( R[x][y] + G[x][y] + B[x][y] ) / 5;
            R[x][y] = (unsigned char) (B[x][y] * 1.6);
            G[x][y] = (unsigned char) (B[x][y] * 1.6);
        }
    }

    printf("\"Aging\" operation is done!\n");
}

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    int x, y;

    for( y = 0; y < HEIGHT; y++ )
    {
        for( x = 0; x < WIDTH; x++ )
        {
            B[x][y] = R[x][y] = G[x][y] = ( R[x][y] + G[x][y] + B[x][y] ) / 3;
        }
    }

    printf("\"Black & White\" operation is done!\n");
}

/* sharpen the image */
void Sharpen(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    int x, y, row, column;
    unsigned char r[WIDTH][HEIGHT], g[WIDTH][HEIGHT], b[WIDTH][HEIGHT];
    int red = 0, green = 0, blue = 0;    
    /* Set colors to temporary array of colors */
    for( y = 0; y < HEIGHT; y++ )
    {
        for( x = 0; x < WIDTH; x++ )
        {
   	    r[x][y] = R[x][y];
	    g[x][y] = G[x][y];
	    b[x][y] = B[x][y];
        }
    }
    
    /* Sharpening image of the temporary array colors */
    for( y = 0; y < HEIGHT; y++ )
    {
	for( x = 0; x < WIDTH; x++ )
	{	    
	    /* CENTER */
	    if ((y != 0 && y != HEIGHT-1) && (x != 0 && x != WIDTH))
	    {   
		red = green = blue = 0;

		for (row = -1; row <= 1; row++)
		{
		    for (column = -1; column <= 1; column++)
		    {
			if (row == 0 && column == 0)
			{
			red += 9 * r[x+row][y+column];
			green += 9 * g[x+row][y+column];
			blue += 9 * b[x+row][y+column];
			}
			else
			{
			red += -r[x+row][y+column];
			green += -g[x+row][y+column];
			blue += -b[x+row][y+column];
			}
		    }
		}

		if (red >= 255)
		   red = 255;
		if (red <= 0)
		   red = 0;

                if (green >= 255)
                   green = 255;
                if (green <= 0)
                   green = 0;

                if (blue >= 255)
                   blue = 255;
                if (blue <= 0)
                   blue = 0;

		R[x][y] = red;
		G[x][y] = green;
		B[x][y] = blue;
	    }
	} 
    }
    
    printf("\"Sharpen\" operation is done!\n");
}

/* change the image hue */
void Hue(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int degree)
{
    int x, y;
    double alpha, beta, gamma, radian;
    unsigned char R_Old[WIDTH][HEIGHT];
    unsigned char B_Old[WIDTH][HEIGHT];
    unsigned char G_Old[WIDTH][HEIGHT];

    radian = (degree * 3.14159) / 180;

    alpha = ((2 * cos(radian)) + 1) / 3;
    beta = ((1 - cos(radian)) / 3) - ((sin(radian)) / sqrt(3));
    gamma = ((1 - cos(radian)) / 3) + ((sin(radian)) / sqrt(3));

    for( y = 0; y < HEIGHT; y++ )
    {
        for( x = 0; x < WIDTH; x++ )
        {
            B_Old[x][y] =  (B[x][y]);
            R_Old[x][y] =  (R[x][y]);
            G_Old[x][y] =  (G[x][y]);
        }
    }

    for( y = 0; y < HEIGHT; y++ )
    {
        for( x = 0; x < WIDTH; x++ )
        {
            B[x][y] = ((R_Old[x][y] * beta) + (G_Old[x][y] * gamma) + (B_Old[x][y] * alpha));
            R[x][y] = ((R_Old[x][y] * alpha) + (G_Old[x][y] * beta) + (B_Old[x][y] * gamma));
            G[x][y] = ((R_Old[x][y] * gamma) + (G_Old[x][y] * alpha) + (B_Old[x][y] * beta));
        }
    }
   
    printf("\"Hue\" operation is done!\n");
}

/* Test all functions */
void AutoTest(const char fname[SLEN],unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int degree)
{  
    LoadImage(fname, R, G, B);
    BlackNWhite(R, G, B);
    SaveImage("bw", R, G, B);
    printf("Black and White tested!\n\n");

    LoadImage(fname, R, G, B);
    Sharpen(R, G, B);
    SaveImage("sharpen", R, G, B);
    printf("Sharpen tested!\n\n");

    LoadImage(fname, R, G, B);
    Hue(R, G, B, degree);
    SaveImage("hue", R, G, B);
    printf("Hue tested!\n\n");

}


/*Print Menu*/
void PrintMenu(void)
{
    printf("-------------------------------------------------\n"
           "1: Load a PPM image\n"
           "2: Save the image in PPM and JPEG format\n"
           "3: Change the color image to black and white\n"
           "4: Sharpen the image\n"
           "5: Change the hue of image\n"
           "6: Test all functions\n"
           "7: Exit\n"
           "-------------------------------------------------\n"
           "Please make you choice: ");
}
/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

/********************************************************************/
