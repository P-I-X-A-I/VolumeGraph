#pragma once
#include "OpenGL_Manager_Class.h"


class drawBase
{
public:
	OpenGL_Manager_Class* gl_obj;
	int state_num = 0;

	GLuint* COMMON_PRG;

	drawBase();
	~drawBase();

	void set_status_value( int num );
	void setup_gl();
	void randomize();
	void unique_process(int layer);
	void draw(int layer);
};

