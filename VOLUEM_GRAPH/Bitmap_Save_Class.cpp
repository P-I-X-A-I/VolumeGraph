#include "framework.h"
#include "Bitmap_Save_Class.h"


Bitmap_Save_Class::Bitmap_Save_Class()
{
	// setup default header

	 // 14byte + Windows header40 byte
	WINDOWS_HEADER = (char*)malloc(54);
	char* writePtr = WINDOWS_HEADER;


	// bfType
	*writePtr = 'B'; writePtr++;
	*writePtr = 'M'; writePtr++;
	
	// bfSize
	unsigned long fileSize = (50*50*4) + 54;
	memcpy(writePtr, &fileSize, sizeof(unsigned long)); // 4byte
	writePtr += 4;


	// bfReserved1
	unsigned int reserved = 0;
	memcpy(writePtr, &reserved, sizeof(unsigned int));// 2byte
	writePtr += 2;

	// bfReserved2
	memcpy(writePtr, &reserved, sizeof(unsigned int));// 2byte
	writePtr += 2;

	// bfOffBits
	unsigned long offset = 54;
	memcpy(writePtr, &offset, sizeof(unsigned long)); // 4byte
	writePtr += 4;


	///////////// Windows header ///////////////
	// Windows header size
	unsigned long headerSize = 40;
	memcpy(writePtr, &headerSize, sizeof(unsigned long));
	writePtr += 4;

	// biWidth
	long bmpWidth = 50;
	memcpy(writePtr, &bmpWidth, sizeof(long));
	writePtr += 4;

	// biHeight
	long bmpHeight = 50;
	memcpy(writePtr, &bmpHeight, sizeof(long));
	writePtr += 4;

	// biPlane
	unsigned int plane = 1;
	memcpy(writePtr, &plane, sizeof(unsigned int));
	writePtr += 2;

	// biBitCount
	unsigned int bitCount = 32;
	memcpy(writePtr, &bitCount, sizeof(unsigned int));
	writePtr += 2;

	// biCompressiong
	unsigned long comp = 0;
	memcpy(writePtr, &comp, sizeof(unsigned long));
	writePtr += 4;

	// biSizeImage
	unsigned long sizeImage = 50 * 50 * 4;
	memcpy(writePtr, &sizeImage, sizeof(unsigned long));
	writePtr += 4;

	// horizontal dot/m
	long dpm_w = 3780; // 96dpi
	memcpy(writePtr, &dpm_w, sizeof(long));
	writePtr += 4;

	// vertical dot/m
	long dpm_h = 3780; // 96dpi
	memcpy(writePtr, &dpm_h, sizeof(long));
	writePtr += 4;

	// palette
	unsigned long palette = 0;
	memcpy(writePtr, &palette, sizeof(unsigned long));
	writePtr += 4;

	//col
	unsigned long impCol = 0;
	memcpy(writePtr, &impCol, sizeof(unsigned long));
	writePtr += 4;

	///////////////////////////////////////////////
}


Bitmap_Save_Class::~Bitmap_Save_Class()
{

}

void Bitmap_Save_Class::save_RGBA_bmp_to_file(char* filePath, unsigned char* bmpPtr, int width, int height)
{
	// setup header
	char* writeHeader = WINDOWS_HEADER;

	// write filesize
	writeHeader += 2;
	unsigned long fileSize = (width * height * 4) + 54;
	memcpy(writeHeader, &fileSize, sizeof(unsigned long));
	writeHeader += 16;


	// write width
	long W = width;
	memcpy(writeHeader, &W, sizeof(long));
	writeHeader += 4;

	// write height
	long H = height;
	memcpy(writeHeader, &H, sizeof(long));
	writeHeader += 12;

	// write image size
	unsigned long imgSize = width * height * 4;
	memcpy(writeHeader, &imgSize, sizeof(unsigned long));


	///////////////////////////////////////////////////

	// save to file
	char* savePtr = (char*)malloc(fileSize);
	char* restorePtr = savePtr;


	// copy header
	memcpy(savePtr, WINDOWS_HEADER, 54);
	savePtr += 54;

	// copy data
	char* readPtr = (char*)bmpPtr;

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			char val[4];
			val[0] = *readPtr; readPtr++;
			val[1] = *readPtr; readPtr++;
			val[2] = *readPtr; readPtr++;
			val[3] = *readPtr; readPtr++;

			*savePtr = val[2]; savePtr++;
			*savePtr = val[1]; savePtr++;
			*savePtr = val[0]; savePtr++;
			*savePtr = val[3]; savePtr++;
		}
	}


	savePtr = restorePtr;

	// save to file
	FILE* fp;
	errno_t error = fopen_s(&fp, filePath, "wb");

	fwrite(savePtr, fileSize, 1, fp);

	fclose(fp);

	// free
	free(savePtr);
}