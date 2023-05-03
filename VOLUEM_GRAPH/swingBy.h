#pragma once
#include "drawBase.h"
#include "framework.h"

#define NUM_PLANET 8
#define SWING_PT 20
#define SWING_TAIL 400

class swingBy :
	public drawBase
{
public:

	double PNT_XY[NUM_PLANET][2];
	double PNT_WEIGHT[NUM_PLANET];

	GLdouble PT_POS[SWING_PT][4];
	GLdouble PT_STOCK[SWING_PT][2];
	GLdouble PT_TAIL[SWING_PT][SWING_TAIL][4];
	GLdouble PT_WEIGHT[SWING_PT];
	GLdouble PT_VELOCITY[SWING_PT][2];

	GLuint VAO_name;
	GLuint VBO_name;


	swingBy();
	~swingBy();

	void setup_gl();
	void randomize();
	void unique_process(int layer);
	void animation_loop();
	void draw(int layer);
};

