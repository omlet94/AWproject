#include <stdio.h>
#include <string.h>
#include <malloc.h>

unsigned char *read_bmp(char *inputfilename, unsigned char header[], int* _width, int* _height)
{
	static unsigned char *img;
	int width, height;
	int padding;
	FILE *fd;
	fd = fopen(inputfilename, "rb");
	if (fd == NULL)
	{
		printf("Error: fopen failed\n");
		return NULL;
	}
	fread(header, sizeof(unsigned char), 54, fd);
	width = *(int*)&header[18];
	height = *(int*)&header[22];
	while ((width * 3 + padding) % 4 != 0)
	{
		padding++;
	}
	int widthnew = width * 3 + padding;
	img = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
	if (img == NULL)
	{
		printf("Error: Malloc failed. when you try to allocate bmp memory\n");
		return NULL;
	}
	unsigned char* data = (unsigned char *)malloc(widthnew * sizeof(unsigned int));
	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), widthnew, fd);
		for (int j = 0; j < width * 3; j += 3)
		{
			int index = (i * width * 3) + (j);
			img[index + 0] = data[j + 0];//r -> B
			img[index + 1] = data[j + 1];//g -> G
			img[index + 2] = data[j + 2];//b -> R
		}
	}

	free(data);
	fclose(fd);
    *_width = width;
	*_height = height;
    return img;
}
void write_bmp(char *outputfilename, unsigned char header[], unsigned char outputimg[], int* width, int* height) {
	FILE *fo;
	fo = fopen(outputfilename, "w");
	fwrite(header, sizeof(unsigned char), 54 * sizeof(unsigned char), fo);
	fwrite(outputimg, sizeof(unsigned char), (*width) * (*height) * 3 * sizeof(unsigned char), fo);
	fclose(fo);
}
int main()
{
    int width, height, i, j;
	char* inputfilename = "test1.bmp";
	char* outputfilename = "test1_output.bmp";
	unsigned char header[54];
	unsigned char* input_img;
	input_img = read_bmp(inputfilename, header, &width, &height);
	write_bmp(outputfilename, header, input_img, &width, &height);
    return 0;
}