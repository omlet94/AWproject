#include <stdio.h>
#include <string.h>
#include <malloc.h>

unsigned char *read_bmp(char *inputfilename, unsigned char header[], int* _width, int* _height, int* _padding)
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
			img[index + 0] = data[j + 0];//B
			img[index + 1] = data[j + 1];//G
			img[index + 2] = data[j + 2];//R
		}
	}

	free(data);
	fclose(fd);
	*_width = width;
	*_height = height;
	*_padding = padding;
	return img;
}
void write_bmp(char *outputfilename, unsigned char header[], unsigned char outputimg[], int* width, int* height, int* padding) {
	int widthnew = (*width) * 3 + (*padding);
	unsigned char* output_data = (unsigned char *)malloc(widthnew * (*height) * sizeof(unsigned char));
	printf("padding: %d", *padding);
	printf("\n");
	printf("Width: %d", *width);
	printf("\n");
	printf("Height: %d", *height);
	printf("\n");
	printf("widthnew: %d", widthnew);
	printf("\n");
	FILE *fo;
	fo = fopen(outputfilename, "w");
	fwrite(header, sizeof(unsigned char), 54 * sizeof(unsigned char), fo);
	for (int i = 0; i < *height; i++)
	{
		int index_data;
		for (int j = 0; j < ((*width) * 3); j += 3)
		{
			int index = (i * (*width) * 3) + (j);
			index_data = (i * widthnew) + (j);
			output_data[index_data + 0] = outputimg[index + 0];//B
			output_data[index_data + 1] = outputimg[index + 1];//G
			output_data[index_data + 2] = outputimg[index + 2];//R
		}
		for (int p = 0; p < *padding; p++) {
			output_data[index_data] = 0;
			index_data++;
		}
	}
	fwrite(outputimg, sizeof(unsigned char), widthnew * (*height) * sizeof(unsigned char), fo);
	free(output_data);
	fclose(fo);
}
int main(int argc, char *argv[])
{
	int width, height, i, j, padding;
	char* inputfilename = "test1.bmp";
	printf("%s\n", inputfilename);
	char* outputfilename = "test1_output.bmp";
	unsigned char header[54];
	unsigned char* input_img;
	input_img = read_bmp(inputfilename, header, &width, &height, &padding);
	write_bmp(outputfilename, header, input_img, &width, &height, &padding);
	free(input_img);
	return 0;
}