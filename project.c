#include <stdio.h>
#include <string.h>
#include <malloc.h>

//unsigned char *read_bmp(char *fname,int* _w, int* _h)
//{
//    unsigned char header[54];
//    FILE *fd = fopen(fname,"rb");
//	if (fd == NULL)
//	{
//		printf("Error: fopen failed\n");
//		return 1;
//	}
//	// Read header
//	fread(header, sizeof(unsigned char), 54, fd);
//	int width = (int)&header[18];
//	int height = (int)&header[22];
//	int padding;
//	// Calculate padding
//	while ((width * 3 + padding) % 4 != 0)
//	{
//		padding++;
//	}
//
//	// Compute new width, which includes padding
//	int widthnew = width * 3 + padding;
//	unsigned char *texels = (unsigned char *)malloc(width * height * sizeof(unsigned char));
//	if (texels == NULL)
//	{
//		printf("Error: Malloc failed. when you try to allocate bmp memory\n");
//		return 1;
//	}
//	// Allocate temporary memory to read widthnew size of data
//	unsigned char* data = (unsigned char *)malloc(widthnew * sizeof(unsigned int));
//	// Read row by row of data and remove padded data.
//	for (int i = 0; i < height; i++)
//	{
//		// Read widthnew length of data
//		fread(data, sizeof(unsigned char), widthnew, fd);
//
//		// Retain width length of data, and swizzle RB component.
//		// BMP stores in BGR format, my usecase needs RGB format
//		for (int j = 0; j < width * 3; j += 3)
//		{
//			int index = (i * width + j/3);
//			if (data[j + 0] == 0 && data[j + 1] == 0 && data[j + 2] == 0) {
//				int pixel = 0;
//				texels[index] = (unsigned char)pixel;
//			}
//			else {
//				int pixel = 1;
//				texels[index] = (unsigned char)pixel;
//			}
//		}
//	}
//	free(data);
//	fclose(fd);
//    *_w = width; *_h = height;
//    return texels;
//}

int main()
{
    /*int width, height, i, j;
    unsigned char* img = read_bmp("test1.bmp", &width, &height);
	printf("width = %d\nheight = %d\n",width,height);
    for(j = 0 ; j < height ; j++)
    {
       for(i = 0 ; i < width ; i++)
          printf("%c ", img[j * width + i]);
        printf("\n");
   }*/
	char* inputfilename = "test1.bmp";
	static unsigned char *texels;
	static int width, height;
	FILE *fd;
	fd = fopen(inputfilename, "rb");
	if (fd == NULL)
	{
		printf("Error: fopen failed\n");
		return 1;
	}

	unsigned char header[54];

	// Read header
	fread(header, sizeof(unsigned char), 54, fd);
	width = *(int*)&header[18];
	height = *(int*)&header[22];
	int padding;
	// Calculate padding
	while ((width * 3 + padding) % 4 != 0)
	{
		padding++;
	}

	// Compute new width, which includes padding
	int widthnew = width * 3 + padding;
	texels = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
	if (texels == NULL)
	{
		printf("Error: Malloc failed. when you try to allocate bmp memory\n");
		return 1;
	}
	// Allocate temporary memory to read widthnew size of data
	unsigned char* data = (unsigned char *)malloc(widthnew * sizeof(unsigned int));
	// Read row by row of data and remove padded data.
	for (int i = 0; i < height; i++)
	{
		// Read widthnew length of data
		fread(data, sizeof(unsigned char), widthnew, fd);

		// Retain width length of data, and swizzle RB component.
		// BMP stores in BGR format, my usecase needs RGB format
		for (int j = 0; j < width * 3; j += 3)
		{
			int index = (i * width * 3) + (j);
			texels[index + 0] = data[j + 0];//r -> B
			texels[index + 1] = data[j + 1];//g -> G
			texels[index + 2] = data[j + 2];//b -> R
		}
	}

	free(data);
	fclose(fd);
	
	FILE *fo;
	fo = fopen("test1output.bmp", "w");
	fwrite(header, sizeof(unsigned char), 54 * sizeof(unsigned char), fo);
	fwrite(texels, sizeof(unsigned char), width * height * 3 * sizeof(unsigned char), fo);

	fclose(fo);
    return 0;
}