#pragma once
#include "drawBase.h"
#include "framework.h"

#define NUM_REP_POINT 3000

class repulsive_force :
	public drawBase
{

public:
	stopWatch* stopWatch_obj;

	GLfloat fCenter[5][4];
	GLfloat vert[5][NUM_REP_POINT][4];
	GLfloat pV[5][NUM_REP_POINT][2];
	GLfloat windRad[5];
	GLfloat windRad_add[5];
	GLfloat eCoef[5][NUM_REP_POINT];

	double windRadSpeed = 0.000003;

	GLuint VAO_name;
	GLuint VBO_name;



	repulsive_force();
	~repulsive_force();

	void animation();

	void setup_gl();
	void randomize();
	void unique_process(int layer);
	void draw(int layer);


};

