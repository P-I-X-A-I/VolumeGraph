#include "koko.h"


koko::koko()
{

}

koko::~koko()
{
	
}



void koko::setup_gl()
{
	// gen VAO, VBO
	glGenVertexArrays(1, &VAO_name);
	glGenBuffers(1, &VBO_name);

	// bind enable vertex attrib array
	glBindVertexArray(VAO_name);
	glEnableVertexAttribArray(0);
}


void koko::randomize()
{
	// randomize
}


void koko::unique_process(int layer)
{
	// export illustrator script
}


void koko::draw(int layer)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use common shader
	glUseProgram(*COMMON_PRG);


	// updata buffer
	glBindVertexArray(VAO_name);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name);
	//glBufferData();
	//glVertexAttribPointer();


	// draw
	//glDrawArrays(GL_LINES, 0, 0);

}