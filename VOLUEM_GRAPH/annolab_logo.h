#pragma once
#include "drawBase.h"
#include "framework.h"
#include "SVG_Loader_Class.h"


class annolab_logo : public drawBase
{
public:

	SVG_Loader_Class* svg_obj;

	OpenGL_Manager_Class* gl_obj;
	int state_num = 0;

	// normal shader of main controller
	GLuint* COMMON_PRG;

	GLuint VAO_logo;
	GLuint VBO_logo;


	annolab_logo();
	~annolab_logo();

	void setup_gl();
	void randomize();
	void unique_process(int layer);
	void draw(int layer);
};

