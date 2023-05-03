#include "framework.h"
#include "bitmapLoaderClass.h"


bitmapLoaderClass::bitmapLoaderClass()
{
	std::cout << "bitmapLoaderClass is initialized" << std::endl;

	BMP_width = 0;
	BMP_height = 0;
	BMP_dataPtr = NULL;
}


bitmapLoaderClass::~bitmapLoaderClass()
{
	if(BMP_dataPtr != NULL)
	{
		free(BMP_dataPtr);
	}
}


bool bitmapLoaderClass::load_bitmap_from_path(const char* bmpPath)
{
	return this->get_BGRA_bitmapInformation(bmpPath);
}

bool bitmapLoaderClass::get_BGRA_bitmapInformation(const char* bmpName)
{
	std::ifstream sFile;
	FILE* fh;
	errno_t errorStatus;

	// check pointer
	if(BMP_dataPtr != NULL)
	{
		free(BMP_dataPtr);
		BMP_dataPtr = NULL;
	}


	// open file
	errorStatus = fopen_s(&fh, bmpName, "rb"); // read file as binary data

	if (errorStatus != 0 )
	{
		std::cout << "BMP file can't be opened" << std::endl;
		return false;
	}


	// set file's position to end of the file
	fseek(fh, 0, SEEK_END );

	// get file length
	fpos_t fLength = ftell(fh);
	if (fLength == 0)
	{
		std::cout << "this BMP file has no data..." << std::endl;
		fclose(fh);
		return false;
	}
	else
	{
		std::cout << bmpName << " file length : " << fLength << std::endl;
	}


	// reset file's position to the beginning
	fseek(fh, 0, SEEK_SET);



	// get BMP data
	// read file data to memory
	void* dataPtr = malloc(fLength);
	if (dataPtr == NULL)
	{
		std::cout << "malloc fail... error" << std::endl;
		return false;
	}

	

	// Bitmap File Header [14 byte] *********************************
	// filetype [2byte] ( is always 'BM' )

	unsigned short temp_fileType;
	fread_s(&temp_fileType, sizeof(unsigned short), sizeof(unsigned short), 1, fh);

	if( temp_fileType == 0x4d42 )
	{
		std::cout << "file type is BM " << std::endl;
	}
	else
	{
		std::cout << "this file is not BMP file... error" << std::endl;
		return false;
	}


	// filesize [4byte]
	unsigned long temp_fileSize;
	fread_s(&temp_fileSize, sizeof(unsigned long), sizeof(unsigned long), 1, fh);
	std::cout << "BMP filesize : " << temp_fileSize << std::endl;


	// skip unused area [4byte] ( go to 10th position )
	fseek(fh, 10, SEEK_SET);


	// offset to image data [4byte]
	unsigned long temp_offsetToImage;
	fread_s(&temp_offsetToImage, sizeof(unsigned long), sizeof(unsigned long), 1, fh);

	std::cout << "offset to BMP image data : " << temp_offsetToImage << std::endl;



	// Bitmap Information Header [12 or 40byte]***************************
	/// header size
	fseek(fh, 14, SEEK_SET);

	unsigned long temp_infoHeaderSize;
	fread_s(&temp_infoHeaderSize, sizeof(unsigned long), sizeof(unsigned long), 1, fh);

	if (temp_infoHeaderSize == 40)
	{
		std::cout << "This BMP is Windows format" << std::endl;
	}
	else
	{
		std::cout << "File format should be Windows format.... error" << std::endl;
		return false;
	}


	// pixel width and height
	long temp_width;
	long temp_height;
	bool isMinusHeight;

	fread_s(&temp_width, sizeof(long), sizeof(long), 1, fh);
	fread_s(&temp_height, sizeof(long), sizeof(long), 1, fh);
	

	std::cout << "BMP width : " << temp_width << std::endl;

	if (temp_height < 0)
	{
		isMinusHeight = true;
		temp_height = -temp_height;
		std::cout << "BMP height : "<< temp_height << " from left top" << std::endl;
	}
	else if (temp_height > 0)
	{
		isMinusHeight = false;
		std::cout << "BMP height : " << temp_height << " from left bottom" << std::endl;
	}

	BMP_width = temp_width;
	BMP_height = temp_height;



	// get bitcount
	fseek(fh, 28, SEEK_SET );
	unsigned int temp_bitCount;
	fread_s(&temp_bitCount, sizeof(unsigned int), sizeof(unsigned int), 1, fh);

	std::cout << "BMP bit count : " << temp_bitCount << std::endl;


	//****************** NEW ****************************

	if (temp_bitCount == 32)
	{
	}
	else if ( temp_bitCount == 24 )
	{
	}
	else
	{
		std::cout << "BMP should be saved in 24 or 32 bit format..... error" << std::endl;
		return false;
	}


	// set filepointer to Image data position
	fseek(fh, temp_offsetToImage, SEEK_SET);


	// check data alignment ( should be multiple of 4byte in 1-line )
	int byteInLine = BMP_width * (temp_bitCount / 8);
	int numOfPaddingByte = byteInLine % 4;

	if (numOfPaddingByte != 0)
	{
		numOfPaddingByte = 4 - numOfPaddingByte;
	}



	// alloc memory
	long lineSize = byteInLine + numOfPaddingByte;
	long memsize = lineSize * BMP_height;
	unsigned char* temp_texPtr = (unsigned char*)malloc(memsize);



	// read image data
	fread_s(temp_texPtr, memsize, sizeof(unsigned char), memsize, fh);


	// copy BMP data
	BMP_dataPtr = (unsigned char*)malloc(BMP_width * BMP_height * 4); // read as RGBA bitmap
	unsigned char* restoreBMP_Ptr = BMP_dataPtr;
	unsigned char* getPtr = temp_texPtr;
	unsigned char temp[4];

	if (isMinusHeight == true)
	{
		for (int y = 0; y < BMP_height; y++)
		{
			//restore pointer
			getPtr = temp_texPtr;

			// set ptr to line head
			getPtr += lineSize * y;

			for (int x = 0; x < BMP_width; x++)
			{
				if (temp_bitCount == 32)
				{
					temp[0] = *getPtr;	getPtr++; // B
					temp[1] = *getPtr;	getPtr++; // G
					temp[2] = *getPtr;	getPtr++; // R
					temp[3] = *getPtr;	getPtr++; // A

					*BMP_dataPtr = temp[2]; BMP_dataPtr++; //R
					*BMP_dataPtr = temp[1]; BMP_dataPtr++; //G
					*BMP_dataPtr = temp[0]; BMP_dataPtr++; //B
					*BMP_dataPtr = temp[3];	BMP_dataPtr++; //A
				}
				else if (temp_bitCount == 24)
				{
					temp[0] = *getPtr; getPtr++; // B
					temp[1] = *getPtr; getPtr++; // G
					temp[2] = *getPtr; getPtr++; // R

					*BMP_dataPtr = temp[2]; BMP_dataPtr++; //R
					*BMP_dataPtr = temp[1]; BMP_dataPtr++; //G
					*BMP_dataPtr = temp[0]; BMP_dataPtr++; //B
					*BMP_dataPtr = 255; BMP_dataPtr++; //A

				}
			}
		}
	}
	else
	{
		for (int y = 0; y < BMP_height; y++)
		{
			//restore pointer
			getPtr = temp_texPtr;

			// set ptr to line head
			getPtr += lineSize*((temp_height - 1) - y);


			for (int x = 0; x < BMP_width; x++)
			{
				if (temp_bitCount == 32)
				{
					temp[0] = *getPtr;	getPtr++; //B
					temp[1] = *getPtr; getPtr++;  //G
					temp[2] = *getPtr; getPtr++;  //R
					temp[3] = *getPtr;	getPtr++; //A

					*BMP_dataPtr = temp[2]; BMP_dataPtr++; //R
					*BMP_dataPtr = temp[1]; BMP_dataPtr++; //G
					*BMP_dataPtr = temp[0]; BMP_dataPtr++; //B
					*BMP_dataPtr = temp[3]; BMP_dataPtr++; //A
				}
				else if (temp_bitCount == 24)
				{
					temp[0] = *getPtr;	getPtr++; //B
					temp[1] = *getPtr; getPtr++;  //G
					temp[2] = *getPtr; getPtr++;  //R

					*BMP_dataPtr = temp[2]; BMP_dataPtr++; //R
					*BMP_dataPtr = temp[1]; BMP_dataPtr++; //G
					*BMP_dataPtr = temp[0]; BMP_dataPtr++; //B
					*BMP_dataPtr = 255; BMP_dataPtr++; //A
					
				}
			}
		}
	}// isMinusHeight

	BMP_dataPtr = restoreBMP_Ptr;

	free(temp_texPtr);
	fclose(fh);

	//****************** NEW ****************************



	/*
	if( temp_bitCount != 32 )
	{
		std::cout << "BMP should be stored in BGRA format... error" << std::endl;
		return false;
	}
	



	// set filepointer to Image data position
	fseek(fh, temp_offsetToImage, SEEK_SET);
 
	
	long memsize = temp_width * temp_height * 4;
	unsigned char* temp_texPtr = (unsigned char*)malloc(memsize);



	// read image data
	fread_s(temp_texPtr, memsize, sizeof(unsigned char), memsize, fh);


	// get BMP data array
	BMP_dataPtr = (unsigned char*)malloc(memsize);
	unsigned char* restoreBMP_Ptr = BMP_dataPtr;
	unsigned char* getPtr = temp_texPtr;

	if( isMinusHeight == true )
	{
		unsigned char temp[4];

		for (int i = 0; i < (temp_width * temp_height); i++)
		{
			temp[0] = *getPtr;	getPtr++; //B
			temp[1] = *getPtr; getPtr++;  //G
			temp[2] = *getPtr; getPtr++;  //R
			temp[3] = *getPtr;	getPtr++; //A

			*BMP_dataPtr = temp[2]; BMP_dataPtr++; //R
			*BMP_dataPtr = temp[1]; BMP_dataPtr++; //G
			*BMP_dataPtr = temp[0]; BMP_dataPtr++; //B
			*BMP_dataPtr = temp[3]; BMP_dataPtr++; //A
		}
	}
	else // flip the BMP data
	{
		unsigned char temp[4];

		for (int i = 0; i < temp_height; i++)
		{
			//restore pointer
			getPtr = temp_texPtr;

			// set ptr to line head
			getPtr += (temp_width * 4)*((temp_height - 1) - i);

			for (int j = 0; j < temp_width; j++)
			{
				temp[0] = *getPtr;	getPtr++; //B
				temp[1] = *getPtr; getPtr++;  //G
				temp[2] = *getPtr; getPtr++;  //R
				temp[3] = *getPtr;	getPtr++; //A

				*BMP_dataPtr = temp[2]; BMP_dataPtr++; //R
				*BMP_dataPtr = temp[1]; BMP_dataPtr++; //G
				*BMP_dataPtr = temp[0]; BMP_dataPtr++; //B
				*BMP_dataPtr = temp[3]; BMP_dataPtr++; //A
			}
		}
	}



	BMP_dataPtr = restoreBMP_Ptr;

	free(temp_texPtr);
	fclose(fh);

	*/

	return true;

}
