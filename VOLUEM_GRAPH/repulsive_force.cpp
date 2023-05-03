#include "repulsive_force.h"


repulsive_force::repulsive_force()
{
	stopWatch_obj = new stopWatch();

	// constructor
	for (int i = 0; i < 5; i++)
	{
		fCenter[i][0] = (rand() % 200 - 100)*0.01;
		fCenter[i][1] = (rand() % 200 - 100)*0.01;
		fCenter[i][2] = 0.0;
		fCenter[i][3] = 1.0;

		windRad[i] = (rand()%628)*0.01;
		windRad_add[i] = (rand() % 1000 - 500)*windRadSpeed;

		for (int k = 0; k < NUM_REP_POINT; k++)
		{
			vert[i][k][0] = fCenter[i][0] + (rand() % 200 - 100)*0.01;
			vert[i][k][1] = fCenter[i][1] + (rand() % 200 - 100)*0.01;
			vert[i][k][2] = 0.0;
			vert[i][k][3] = 1.0;

			pV[i][k][0] = (rand() % 200 - 100)*0.0001;
			pV[i][k][1] = (rand() % 200 - 100)*0.0001;

			eCoef[i][k] = 0.9 + (rand() % 700)*0.0001; // 0.9 ~ 0.97
		}

	
	}
}


repulsive_force::~repulsive_force()
{
	// destructor
}


void repulsive_force::setup_gl()
{
	// gen VAO, VBO
	glGenVertexArrays(1, &VAO_name);
	glGenBuffers(1, &VBO_name);

	// bind, enable vertex attrib array
	glBindVertexArray(VAO_name);
	glEnableVertexAttribArray(0);
}


void repulsive_force::randomize()
{
	for (int i = 0; i < 5; i++)
	{
		fCenter[i][0] = (rand() % 200 - 100)*0.01;
		fCenter[i][1] = (rand() % 200 - 100)*0.01;
		fCenter[i][2] = 0.0;
		fCenter[i][3] = 1.0;

		windRad[i] = (rand() % 628)*0.01;
		windRad_add[i] = (rand() % 1000-500)*windRadSpeed;

		for (int k = 0; k < NUM_REP_POINT; k++)
		{
			vert[i][k][0] = fCenter[i][0] + (rand() % 200 - 100)*0.01;
			vert[i][k][1] = fCenter[i][1] + (rand() % 200 - 100)*0.01;
			vert[i][k][2] = 0.0;
			vert[i][k][3] = 1.0;

			pV[i][k][0] = (rand() % 200 - 100)*0.0001;
			pV[i][k][1] = (rand() % 200 - 100)*0.0001;
		}
	}
}


void repulsive_force::unique_process(int layer)
{
	// export as illustrator script
	char file_0[] = "\\repulsive0.js";
	char file_1[] = "\\repulsive1.js";
	char file_2[] = "\\repulsive2.js";
	char file_3[] = "\\repulsive3.js";
	char file_4[] = "\\repulsive4.js";

	char* filePtr[5];
	filePtr[0] = file_0;
	filePtr[1] = file_1;
	filePtr[2] = file_2;
	filePtr[3] = file_3;
	filePtr[4] = file_4;


	for (int F = 0; F < 5; F++)
	{
		// get desktop path
		TCHAR desktop_path_w[MAX_PATH];
		char desktop_path[MAX_PATH];

		SHGetSpecialFolderPath(NULL, desktop_path_w, CSIDL_DESKTOP, 0);

		WideCharToMultiByte(CP_ACP, 0, desktop_path_w, -1, desktop_path, MAX_PATH, NULL, NULL);

		// concat filepath
		strcat_s(desktop_path, sizeof(desktop_path), filePtr[F]);


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

		double scale = (152.0 / 2.0)*2.8346;
		for (int k = 0; k < NUM_REP_POINT; k++)
		{
			// add ellipse
			writeStr += "eObj = docObj.pathItems.ellipse(";
			writeStr += std::to_string(vert[F][k][0] * scale);
			writeStr += ", ";
			writeStr += std::to_string(vert[F][k][1] * scale);
			writeStr += ", 1.8, 1.8);\n";

			writeStr += "eObj.stroked = false;\n";
			writeStr += "eObj.filled = true;\n";
			writeStr += "eObj.fillColor = blackColor;\n";
		}

		// add frame
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

	} // for F < 5
}



void repulsive_force::animation()
{
	stopWatch_obj->start_stop_watch();

	for (int i = 0; i < 5; i++)
	{
		// wind
		windRad[i] += windRad_add[i];
		if (windRad[i] > 6.2831853)
		{
			windRad[i] -= 6.2831853;
		}
		else if (windRad[i] < -6.2831853)
		{
			windRad[i] += 6.2831853;
		}

		double wF[2];
		wF[0] = cos(windRad[i])*0.0001;
		wF[1] = sin(windRad[i])*0.0001;

#pragma omp parallel for
		for (int k = 0; k < NUM_REP_POINT; k++)
		{
			double rF[2] = {0.0, 0.0};
			double tempF[2] = { 0.0, 0.0 };

			// replsive force from fCenter
			for (int iter = 0; iter < 5; iter++)
			{
				tempF[0] = vert[i][k][0] - fCenter[iter][0];
				tempF[1] = vert[i][k][1] - fCenter[iter][1];


				double LEN = sqrt(tempF[0] * tempF[0] + tempF[1] * tempF[1]);
				
				// effective Radius
				double eR = 0.01;
				double effectR = (i)*eR + 0.15;

				if (LEN < effectR && LEN != 0.0)
				{
					rF[0] += (tempF[0]/LEN)*(effectR - LEN)*0.01;
					rF[1] += (tempF[1]/LEN)*(effectR - LEN)*0.01;
				}

			}


			// velocity
			pV[i][k][0] = pV[i][k][0] * eCoef[i][k] + rF[0] + wF[0];
			pV[i][k][1] = pV[i][k][1] * eCoef[i][k] + rF[1] + wF[1];

			// position
			vert[i][k][0] += pV[i][k][0];
			vert[i][k][1] += pV[i][k][1];

			// position loop
			if (vert[i][k][0] > 1.0) { vert[i][k][0] = -1.0; }
			else if (vert[i][k][0] < -1.0) { vert[i][k][0] = 1.0; }

			if (vert[i][k][1] > 1.0) { vert[i][k][1] = -1.0; }
			else if (vert[i][k][1] < -1.0) { vert[i][k][1] = 1.0; }
		}
	}

	stopWatch_obj->finish_stop_watch();
	//printf("%f\n", stopWatch_obj->SECOND);
}


void repulsive_force::draw(int layer)
{
	this->animation();

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use common shader
	glUseProgram(*COMMON_PRG);

	// bind vao
	glBindVertexArray(VAO_name);

	// bind, update buffer
	int size = sizeof(GLfloat)*5* NUM_REP_POINT * 4;
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name);
	glBufferData(GL_ARRAY_BUFFER, size, &vert[0][0][0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	if (layer < 5)
	{
		glDrawArrays(GL_POINTS, NUM_REP_POINT*layer, NUM_REP_POINT);
	}
	else
	{
		glDrawArrays(GL_POINTS, 0, 5 * NUM_REP_POINT);
	}
}