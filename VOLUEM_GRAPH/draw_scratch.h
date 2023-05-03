#pragma once
#include "drawBase.h"
#include "framework.h"
#include <ShlObj.h>

#define NUM_LAYER 20
#define NUM_SYNTH 5

class draw_scratch :
	public drawBase
{

public:

	GLuint NORMAL_VS;
	GLuint NORMAL_FS;
	GLuint NORMAL_PRG;

	GLuint VAO_scratch;
	GLuint VBO_scratch;

	GLfloat baseCircle[3600][4];

	GLfloat vert[NUM_LAYER][3600][4];


	draw_scratch();
	~draw_scratch();

	void setup_gl();
	
	void randomize();
	void unique_process(int layer); // export illustrator script
	void draw(int layer);
};

