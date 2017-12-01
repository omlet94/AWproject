#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

unsigned char *read_bmp(char *inputfilename, unsigned char header[], int* _width, int* _height)
{
	static unsigned char *img;
	static int width, height;
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
	*_width = width;
	*_height = height;
	free(data);
	fclose(fd);
	return img;
}
void write_bmp(char *outputfilename, unsigned char header[], unsigned char outputimg[], int* width, int* height) {
	FILE *fo;
	fo = fopen(outputfilename, "w");
	fwrite(header, sizeof(unsigned char), 54 * sizeof(unsigned char), fo);
	fwrite(outputimg, sizeof(unsigned char), (*width) * (*height) * 3 * sizeof(unsigned char), fo);
	fclose(fo);
}
void show_info(char *inputfilename, int* width, int* height, int* struct_size) {
	printf("Morphological Opening\n");
	printf("=====================================\n");
	printf("File: %s\n", inputfilename);
	printf("Width: %d px\n", *width);
	printf("Height: %d px\n", *height);
	printf("Structural element size: %d x %d\n", *struct_size, *struct_size);
	printf("=====================================\n");
}
unsigned char *dilation(unsigned char input_img[], int* width, int* height , int* struct_size)
{	
	static unsigned char *output_img;
	output_img = (unsigned char *)malloc((*width) * (*height) * 3 * sizeof(unsigned char));
	if (output_img == NULL)
	{
		printf("Error: Malloc failed. when you try to allocate bmp memory\n");
		return NULL;
	}
	int struct_margin = (*struct_size - 1) / 2;
	int index = 0;
	int centerpoint = 0;
	int skip = 0;
	for (int i = 0; i < *height; i++)
	{
		for (int j = 0; j < (*width) * 3; j += 3)
		{	
			centerpoint = (i * (*width) * 3) + (j);
			if ((i >= struct_margin) && (i < (*height - struct_margin)) && (centerpoint >= (i * ((*width) * 3) + struct_margin * 3)) && (centerpoint < ((i + 1) * ((*width) * 3) - 3 - struct_margin * 3))) {
				for (int k = -struct_margin; k <= struct_margin; k++) {		
					for (int l = -struct_margin; l <= struct_margin; l++) {							
						index = centerpoint + k * ((*width) * 3) + l * 3;
						if ((input_img[index] == 0) && (input_img[index + 1] == 0) && (input_img[index + 2] == 0)) {
							output_img[centerpoint + 0] = 0;//B
							output_img[centerpoint + 1] = 0;//G
							output_img[centerpoint + 2] = 0;//R
							skip = 1;
						}
						else {
							output_img[centerpoint + 0] = 255;//B
							output_img[centerpoint + 1] = 255;//G
							output_img[centerpoint + 2] = 255;//R
						}
						if (skip != 0) break;
					}
					if (skip != 0) break;
				}
				skip = 0;
			}
			else 
			{
				output_img[centerpoint + 0] = input_img[centerpoint + 0];//B
				output_img[centerpoint + 1] = input_img[centerpoint + 1];//G
				output_img[centerpoint + 2] = input_img[centerpoint + 2];//R
			}
		}
	}
	return output_img;
}
unsigned char *erosion(unsigned char input_img[], int* width, int* height, int* struct_size)
{
	static unsigned char *output_img;
	output_img = (unsigned char *)malloc((*width) * (*height) * 3 * sizeof(unsigned char));
	if (output_img == NULL)
	{
		printf("Error: Malloc failed. when you try to allocate bmp memory\n");
		return NULL;
	}
	int struct_margin = (*struct_size - 1) / 2;
	int index = 0;
	int centerpoint = 0;
	int skip = 0;
	for (int i = 0; i < *height; i++)
	{
		for (int j = 0; j < (*width) * 3; j += 3)
		{
			centerpoint = (i * (*width) * 3) + (j);
			if ((i >= struct_margin) && (i < (*height - struct_margin)) && (centerpoint >= (i * ((*width) * 3) + struct_margin * 3)) && (centerpoint < ((i + 1) * ((*width) * 3) - 3 - struct_margin * 3))) {
				for (int k = -struct_margin; k <= struct_margin; k++) {
					for (int l = -struct_margin; l <= struct_margin; l++) {
						index = centerpoint + k * ((*width) * 3) + l * 3;
						if ((input_img[index] == 255) && (input_img[index + 1] == 255) && (input_img[index + 2] == 255)) {
							output_img[centerpoint + 0] = 255;//B
							output_img[centerpoint + 1] = 255;//G
							output_img[centerpoint + 2] = 255;//R
							skip = 1;
						}
						else {
							output_img[centerpoint + 0] = input_img[centerpoint + 0];//B
							output_img[centerpoint + 1] = input_img[centerpoint + 1];//G
							output_img[centerpoint + 2] = input_img[centerpoint + 2];//R
						}
						if (skip != 0) break;
					}
					if (skip != 0) break;
				}
				skip = 0;
			}
			else
			{
				output_img[centerpoint + 0] = 255;
				output_img[centerpoint + 1] = 255;
				output_img[centerpoint + 2] = 255;
			}
		}
	}
	return output_img;
}
unsigned char *morph_open(unsigned char input_img[], int* width, int* height, int* struct_size) {
	static unsigned char *output_img;
	output_img = (unsigned char *)malloc((*width) * (*height) * 3 * sizeof(unsigned char));
	if (output_img == NULL)
	{
		printf("Error: Malloc failed. when you try to allocate bmp memory\n");
		return NULL;
	}
	output_img = dilation(erosion(input_img, width, height, struct_size), width, height, struct_size);
	return output_img;
}
int main(int argc, char** argv)
{
	int width, height, i, j,struct_size;
	char* inputfilename = "test1.bmp";
	char* outputfilename = "test1_output.bmp";
	unsigned char header[54];
	unsigned char* input_img;
	unsigned char* output_img;

	if (argc >= 2) {
		struct_size = strtol(argv[1], NULL, 10);
	}
	else if (argc == 1) {
		struct_size = 3;
	}

	input_img = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
	output_img = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));

	input_img = read_bmp(inputfilename, header, &width, &height);
	show_info(inputfilename, &width, &height, &struct_size);
	output_img = morph_open(input_img, &width, &height,&struct_size);
	write_bmp(outputfilename, header, output_img, &width, &height);
	printf("Created Output File: %s\n", outputfilename);

	free(input_img);
	free(output_img);

	return 0;
}