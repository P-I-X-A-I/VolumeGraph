#pragma once
#include "drawBase.h"
#include "framework.h"

#define DOT_X 50
#define DOT_Y 50
class dotPattern :
	public drawBase
{
public:
	
	GLfloat base[DOT_X][DOT_Y][2];
	GLfloat vert[5][DOT_X][DOT_Y][4];

	GLuint VAO_name;
	GLuint VBO_name;
	GLuint* COMMON_PRG;


	dotPattern();
	~dotPattern();

	void setup_gl();
	void randomize();
	void unique_process(int layer);
	void draw(int layer);
};

