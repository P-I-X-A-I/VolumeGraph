#pragma once
#include "drawBase.h"
#include "framework.h"
class koko :
	public drawBase
{

public:
	GLuint VAO_name;
	GLuint VBO_name;

	koko();
	~koko();

	void setup_gl();
	void randomize();
	void unique_process(int layer);
	void draw(int layer);


};

