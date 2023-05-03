#pragma once
#include <fstream>
#include <iostream>

class bitmapLoaderClass
{
public:
	 
	unsigned char* BMP_dataPtr;
	int BMP_width;
	int BMP_height;

	// function
	bitmapLoaderClass();
	~bitmapLoaderClass();

	bool get_BGRA_bitmapInformation(const char* bmpName);
	bool load_bitmap_from_path(const char* bmpPath);
};

