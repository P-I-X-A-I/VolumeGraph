#include "draw_scratch.h"

draw_scratch::draw_scratch()
{

	for (int i = 0; i < 3600; i++)
	{
		double radian = (double)i * 0.1 * 0.0174532925;
		baseCircle[i][0] = cos(radian)*0.7;
		baseCircle[i][1] = sin(radian)*0.7;
		baseCircle[i][2] = 0.0;
		baseCircle[i][3] = 1.0;

		for (int k = 0; k < NUM_LAYER; k++)
		{
			vert[k][i][0] = (rand() % 100)*0.01;
			vert[k][i][1] = (rand() % 100)*0.01;
			vert[k][i][2] = 0.0;
			vert[k][i][3] = 1.0;
		}
	}

	this->randomize();
}

draw_scratch::~draw_scratch()
{

}


void draw_scratch::randomize()
{
	// base line rad
	double radius = (rand() % 100)*0.002 + 0.5;
	double baseCycle[5];
	double radiusCoef[5];

	for (int i = 0; i < 5; i++)
	{
		baseCycle[i] = rand() % 3 + 1;
	}


	for (int i = 0; i < 3600; i++)
	{
		double rad = (double)i * 0.1 * 0.0174532925;

		for (int k = 0; k < 5; k++)
		{
			double eachRad = rad * baseCycle[k];
			radiusCoef[k] = sin(eachRad);
		}

		double Rwave = (radiusCoef[0] * 
			radiusCoef[1] * 
			radiusCoef[2] * 
			radiusCoef[3] *
			radiusCoef[4])*0.05;


		baseCircle[i][0] = cos(rad)*(radius + Rwave);
		baseCircle[i][1] = sin(rad)*(radius + Rwave);
	}


	////////////////////////////////////////////////////////////////////////



	for (int k = 0; k < NUM_LAYER; k++)
	{
		double shiftRad = (rand() % 6283)*0.001;
		double CYCLE[NUM_SYNTH];
		int MODE[NUM_SYNTH];
		double R[NUM_SYNTH];
		double eachRad[NUM_SYNTH];
		double shift[NUM_SYNTH];

		//*********************
		double R_coef = 0.0003;
		//*********************
		for (int i = 0; i < NUM_SYNTH; i++)
		{
			CYCLE[i] = rand() % 7 + i;
			MODE[i] = rand() % 4; // 0-sin, 1-half sin
			R[i] = (rand() % 200 - 100)*R_coef;
			shift[i] = (rand() % 6283)*0.001;
		}


		double POS[2];
		for (int i = 0; i < 3600; i++)
		{
			double rad = (double)i * 0.1 * 0.0174532925 + shiftRad;
			POS[0] = 0.0;
			POS[1] = 0.0;

			for (int s = 0; s < NUM_SYNTH; s++)
			{
				eachRad[s] = (rad * CYCLE[s])+shift[s];

				if (MODE[s] == 0)
				{
					POS[0] += cos(eachRad[s])*R[s];
					POS[1] += sin(eachRad[s])*R[s];
				}
				else if(MODE[s] == 1)
				{
					POS[0] += pow(cos(eachRad[s]), 3.0)*R[s];
					POS[1] += pow(sin(eachRad[s]), 3.0)*R[s];
				}
				else if (MODE[s] == 2)
				{
					POS[0] += pow(cos(-eachRad[s]), 2.0)*R[s];
					POS[1] += pow(sin(-eachRad[s]), 2.0)*R[s];
				}
				else if (MODE[s] == 3)
				{
					POS[0] += pow(cos(-eachRad[s]), 1.0)*R[s];
					POS[1] += pow(sin(-eachRad[s]), 1.0)*R[s];
				}
			}

			// rotate POS
			double ROTPOS[2];
			ROTPOS[0] = cos(rad)*POS[0] - sin(rad)*POS[1];
			ROTPOS[1] = sin(rad)*POS[0] + cos(rad)*POS[1];


			vert[k][i][0] = baseCircle[i][0] + ROTPOS[0];
			vert[k][i][1] = baseCircle[i][1] + ROTPOS[1];
		}
	}

}






void draw_scratch::setup_gl()
{
	gl_obj->loadShaderSource_And_CompileShader("NORMAL_VS.txt", 0, &NORMAL_VS);
	gl_obj->loadShaderSource_And_CompileShader("NORMAL_FS.txt", 2, &NORMAL_FS);
	gl_obj->createProgram_And_AttachShader(&NORMAL_PRG, &NORMAL_VS, NULL, &NORMAL_FS);


	// gen VAO, VBO
	glGenVertexArrays(1, &VAO_scratch);
	glGenBuffers(1, &VBO_scratch);

	// enable vertex array
	glBindVertexArray(VAO_scratch);
	glEnableVertexAttribArray(0);

	// alloc buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_scratch);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3600 * 4, baseCircle, GL_DYNAMIC_DRAW);
	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat) * 1000 * 4, NULL, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);


	glBindVertexArray(0);
}


void draw_scratch::draw(int layer)
{

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(NORMAL_PRG);




	glLineWidth(5.0);

	glBindVertexArray(VAO_scratch);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_scratch);

	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3600 * 4, baseCircle);
	//glDrawArrays(GL_LINE_LOOP, 0, 3600);

	if (layer == 0)
	{
		for (int i = 0; i < NUM_LAYER; i++)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3600 * 4, &vert[i][0][0]);
			glDrawArrays(GL_LINE_LOOP, 0, 3600);
		}
	}
	else if ( layer >= 1 && layer <= 5 )
	{
		for (int i = 0; i < NUM_LAYER; i++)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3600 * 4, &vert[i][0][0]);

			if (i % 5 == (layer - 1))
			{
				glDrawArrays(GL_LINE_LOOP, 0, 3600);
			}
		}
	}

	glFlush();
}





void draw_scratch::unique_process(int layer)
{
	if (layer == 0)
	{
		printf("do nothing\n");
		return;
	}
	// save as illustrator script
	char filename_0[] = "\\temp0.js";
	char filename_1[] = "\\temp1.js";
	char filename_2[] = "\\temp2.js";
	char filename_3[] = "\\temp3.js";
	char filename_4[] = "\\temp4.js";
	char filename_5[] = "\\temp5.js";

	char* filename_ptr[6];
	filename_ptr[0] = filename_0;
	filename_ptr[1] = filename_1;
	filename_ptr[2] = filename_2;
	filename_ptr[3] = filename_3;
	filename_ptr[4] = filename_4;
	filename_ptr[5] = filename_5;

	TCHAR desktop_path_w[MAX_PATH];
	char desktop_path[MAX_PATH];

	SHGetSpecialFolderPath(NULL, desktop_path_w, CSIDL_DESKTOP, 0);
	WideCharToMultiByte(CP_ACP, 0, desktop_path_w, -1, desktop_path, MAX_PATH, NULL, NULL);

	// concat filepath
	strcat_s(desktop_path, sizeof(desktop_path), filename_ptr[layer]);

	// open file
	FILE* fp;
	fopen_s(&fp, desktop_path, "w");

	// add script
	std::string writeStr;
	writeStr += "docObj = documents.add(DocumentColorSpace.RGB, 300*2.8346, 300*2.8346);\n";
	writeStr += "var pathColor = new RGBColor();\n";
	writeStr += "pathColor.red = 255;\npathColor.green = 0;\npathColor.blue = 0;\n";
	writeStr += "docObj.defaultFilled = false;\n";
	writeStr += "docObj.defaultStroked = true;\n";
	writeStr += "docObj.defaultStrokeColor = pathColor;\n";
	writeStr += "docObj.defaultStrokeWidth = 0.1;\n";

	//add path
	for( int i = 0 ; i < NUM_LAYER ; i++ )
	{ 
		if (i % 5 == (layer - 1))
		{
			// add path
			writeStr += "pathObj = docObj.pathItems.add();\n";

			// add data array
			
			std::string pathName;
			pathName += "pathData";
			pathName += std::to_string(i);
			pathName += "Array";

			writeStr += (pathName + " = new Array();\n");
			
			int IDX = 0;


			for (int k = 0; k < 3600; k++)
			{
				//pathName_Array[k] = [1.0, 1.0];
				if (k % 4 == 0)
				{
					writeStr += pathName;
					writeStr += "[";
					writeStr += std::to_string(IDX);
					writeStr += "] = [";
					writeStr += std::to_string(vert[i][k][0] * 100.0);
					writeStr += ", ";
					writeStr += std::to_string(vert[i][k][1] * 100.0);
					writeStr += "];\n";

					IDX++;
				}


			} // k < 3600 / 4 = 900



			// end path
			writeStr += "pathObj.setEntirePath( " + pathName + " );\n";
			writeStr += "pathObj.filled = false;\n";
			writeStr += "pathObj.stroked = true;\n";
			writeStr += "pathObj.strokeWidth = 0.1;\n";
			writeStr += "pathObj.closed = true;\n";
			writeStr += "pathObj.strokeColor = pathColor;\n";
			writeStr += "pathObj.locked = false;\n";

		} // if i % 5
	}// i < NUM_LAYER


	// add frame
	writeStr += "pathObj = docObj.pathItems.add();\n";
	writeStr += "frameArray = new Array();\n";
	writeStr += "frameArray[0] = [-100.0, 100.0];\n";
	writeStr += "frameArray[1] = [-100.0, -100.0];\n";
	writeStr += "frameArray[2] = [100.0, -100.0];\n";
	writeStr += "frameArray[3] = [100.0, 100.0];\n";
	writeStr += "pathObj.setEntirePath(frameArray);\n";
	writeStr += "pathObj.filled = false;\n";
	writeStr += "pathObj.stroked = true;\n";
	writeStr += "pathObj.strokeWidth = 0.1;\n";
	writeStr += "pathObj.closed = true;\n";
	writeStr += "pathObj.strokeColor = pathColor;\n";
	writeStr += "pathObj.locked = false;\n";


	fwrite(writeStr.c_str(), writeStr.size(), 1, fp);




	// close file
	fclose(fp);
}

