#pragma once
#include <ShlObj.h>

class Bitmap_Save_Class
{
public:

	char* WINDOWS_HEADER;

	Bitmap_Save_Class();
	~Bitmap_Save_Class();

	void save_RGBA_bmp_to_file(char* filePath, unsigned char* bmpPtr, int width, int height);
};

