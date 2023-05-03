#include "dotPattern.h"


dotPattern::dotPattern()
{
	double WX, WY;
	WX = 2.0;
	WY = 2.0;
	double unitX = WX / (double)(DOT_X - 1) / 2.0;

	for (int i = 0; i < 5; i++)
	{
		double pX, pY;
		for (int x = 0; x < DOT_X; x++)
		{
			pX = (double)x / (double)(DOT_X - 1);
			pX = (pX - 0.5)*WX;

			for (int y = 0; y < DOT_Y; y++)
			{
				double shiftX;
				if (y % 2 == 0)
				{	
					shiftX = 0.0;
				}
				else
				{
					shiftX = unitX;
				}

				pY = (double)y / (double)(DOT_Y - 1);
				pY = (pY - 0.5)*WY;

				vert[i][x][y][0] = pX + shiftX;
				vert[i][x][y][1] = pY;
				vert[i][x][y][2] = 0.0;
				vert[i][x][y][3] = 1.0;

				base[x][y][0] = pX + shiftX;
				base[x][y][1] = pY;
			}
		}
	}
}


dotPattern::~dotPattern()
{

}


void dotPattern::setup_gl()
{
	// gen VAO, VBO
	glGenVertexArrays(1, &VAO_name);
	glGenBuffers(1, &VBO_name);

	// bind and enable attribarray
	glBindVertexArray(VAO_name);
	glEnableVertexAttribArray(0);
}
void dotPattern::randomize()
{
	int LOOP_BASE = rand() % 5 + 38;
	int RAD_LOOP_BASE = rand() % 5 + 4;
	double COEF = (rand() % 200)*0.1;

	for (int i = 0; i < 5; i++)
	{
		for (int x = 0; x < DOT_X; x++)
		{ 
			for (int y = 0; y < DOT_Y; y++)
			{
				int LOOP = (x + y) % LOOP_BASE;
				double radius = (double)((x + y) % RAD_LOOP_BASE);
				radius /= 350.0; // 0.0 ~ 0.01
				radius += 0.01; // 0.01 ~ 0.02

				double rad = (base[x][y][0] + base[x][y][1])*COEF;
				double rad_add = i * ((double)LOOP * 0.01 + 0.1);
				rad += rad_add;
				double sX, sY;

				sX = cos(rad) * radius;
				sY = sin(rad) * radius;
				vert[i][x][y][0] = base[x][y][0] + sX;
				vert[i][x][y][1] = base[x][y][1] + sY;
			}
		}
	}
}
void dotPattern::unique_process(int layer)
{
	// export as illustrator script
	if (layer > 4)
	{
		printf("do nothing\n");
		return;
	}

	char file_0[] = "\\dot0.js";
	char file_1[] = "\\dot1.js";
	char file_2[] = "\\dot2.js";
	char file_3[] = "\\dot3.js";
	char file_4[] = "\\dot4.js";

	char* filePtr[5];
	filePtr[0] = file_0;
	filePtr[1] = file_1;
	filePtr[2] = file_2;
	filePtr[3] = file_3;
	filePtr[4] = file_4;

	// get desktop path
	TCHAR desktop_path_w[MAX_PATH];
	char desktop_path[MAX_PATH];

	SHGetSpecialFolderPath(NULL, desktop_path_w, CSIDL_DESKTOP, 0);

	WideCharToMultiByte(CP_ACP, 0, desktop_path_w, -1, desktop_path, MAX_PATH, NULL, NULL);

	// concat filepath
	strcat_s(desktop_path, sizeof(desktop_path), filePtr[layer]);

	// open file
	FILE* fp;
	fopen_s(&fp, desktop_path, "w");

	// add script
	std::string writeStr;
	
	writeStr += "docObj = documents.add(DocumentColorSpace.RGB, 300*2.8346, 300*2.8346);\n";

	writeStr += "var pathColor = new RGBColor();\n";
	writeStr += "pathColor.red = 255;\npathColor.green = 0;\npathColor.blue = 0;\n";

	writeStr += "var blackColor = new RGBColor();\n";
	writeStr += "blackColor.red = 0;\nblackColor.green = 0;\nblackColor.blue = 0;\n";

	writeStr += "docObj.defaultFilled = false;\n";
	writeStr += "docObj.defaultStroked = true;\n";
	writeStr += "docObj.defaultStrokedWidth = 0.1;\n";


	// add path
	double scale = (152.0 / 2.0)*2.8346;
	for (int x = 0; x < DOT_X; x++)
	{
		for (int y = 0; y < DOT_Y; y++)
		{
			writeStr += "eObj = docObj.pathItems.ellipse(";
			writeStr += std::to_string(vert[layer][x][y][0]*scale);
			writeStr += ", ";
			writeStr += std::to_string(vert[layer][x][y][1]*scale);
			writeStr += ", 1.8, 1.8);\n";

			writeStr += "eObj.stroked = false;\n";
			writeStr += "eObj.filled = true;\n";
			writeStr += "eObj.fillColor = blackColor;\n";
		}
	}



	// add outer frame
	// add frame
	// acryl plate size = 152mm x 130mm
	writeStr += "pathObj = docObj.pathItems.add();\n";
	writeStr += "frameArray = new Array();\n";
	writeStr += "frameArray[0] = [-76.0*2.8346, 65.0*2.8346];\n";
	writeStr += "frameArray[1] = [-76.0*2.8346, -65.0*2.8346];\n";
	writeStr += "frameArray[2] = [76.0*2.8346, -65.0*2.8346];\n";
	writeStr += "frameArray[3] = [76.0*2.8346, 65.0*2.8346];\n";
	writeStr += "pathObj.setEntirePath(frameArray);\n";
	writeStr += "pathObj.filled = false;\n";
	writeStr += "pathObj.stroked = true;\n";
	writeStr += "pathObj.strokeWidth = 0.1;\n";
	writeStr += "pathObj.closed = true;\n";
	writeStr += "pathObj.strokeColor = pathColor;\n";
	writeStr += "pathObj.locked = false;\n";


	// write, close file
	fwrite(writeStr.c_str(), writeStr.size(), 1, fp);
	fclose(fp);
}







void dotPattern::draw(int layer)
{
	int NUM = layer;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use common shader
	glUseProgram(*COMMON_PRG);

	//bind vao
	glBindVertexArray(VAO_name);

	// bind & update buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name);

	if (NUM < 5)
	{
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * DOT_X * DOT_Y * 4,
			&vert[NUM][0][0][0],
			GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

		//glLineWidth(10.0);
		glPointSize(10.0);
		//glDrawArrays(GL_LINES, 0, DOT_X * DOT_Y);
		glDrawArrays(GL_POINTS, 0, DOT_X * DOT_Y);
	}
	else
	{
		for (int k = 0; k < 5; k++)
		{
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(GLfloat) * DOT_X * DOT_Y * 4,
				&vert[k][0][0][0],
				GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

			//glLineWidth(10.0);
			glPointSize(10.0);
			glDrawArrays(GL_POINTS, 0, DOT_X * DOT_Y);
		}
	}

}
