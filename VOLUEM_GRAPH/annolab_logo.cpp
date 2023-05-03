#include "annolab_logo.h"

annolab_logo::annolab_logo()
{
	printf("\n*** annolab logo init ***\n");
	svg_obj = new SVG_Loader_Class();
	//svg_obj->load_svg_from_path("logo.svg");
	svg_obj->load_svg_from_path("R5.svg");
	svg_obj->scale_point_data(0.1);
	svg_obj->flip_y();
}

annolab_logo::~annolab_logo()
{


}

void annolab_logo::setup_gl()
{
	// VAO, VBO
		// gen VAO, VBO
	glGenVertexArrays(1, &VAO_logo);
	glGenBuffers(1, &VBO_logo);

	// bind & enable
	glBindVertexArray(VAO_logo);
	glEnableVertexAttribArray(0);
}


void annolab_logo::unique_process(int layer)
{

}

void annolab_logo::randomize()
{}
void annolab_logo::draw(int layer)
{

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(*COMMON_PRG);


	glLineWidth(20.0);

	GLfloat* vPtr = svg_obj->get_pVert_pointer();
	int numVert = svg_obj->NUM_P;

	glBindVertexArray(VAO_logo);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_logo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numVert*4, vPtr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_LINES, 0, numVert);
}