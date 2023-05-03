#pragma once
#include "framework.h"

// class
#include "OpenGL_Manager_Class.h"
#include "Window_Manager_Class.h"
#include "GUI_Manager_Class.h"
#include "Bitmap_Save_Class.h"
#include "SVG_Loader_Class.h"

// draw class
#include "drawBase.h"
#include "draw_scratch.h"
#include "annolab_logo.h"
#include "dotPattern.h"
#include "repulsive_force.h"
#include "swingBy.h"

#define RENDER_WIDTH 2048
#define RENDER_HEIGHT 2048

class mainController
{
public:
	OpenGL_Manager_Class* gl_manager_obj;
	Window_Manager_Class* window_manager_obj;
	GUI_Manager_Class* gui_manager_obj;
	Bitmap_Save_Class* bitmap_save_obj;

	HWND base_window_obj;
	HWND gl_view_obj;

	// GUI HWND
	HWND bmpWnd;
	HWND textWnd;
	HWND comboWnd;

	UINT timerID = 0;

	int current_layer = 0;
	// FBO, shader, sampler VAO
	GLuint Sampler_name;



	GLuint FBO_name;
	GLuint FBOTEX_name[2]; // color, depth


	GLuint NORMAL_VS;
	GLuint NORMAL_FS;
	GLuint NORMAL_PRG;

	GLuint FINAL_VS;
	GLuint FINAL_FS;
	GLuint FINAL_PRG;
	GLint UNF_texImage;

	GLuint VAO_final;
	GLuint VBO_final[2]; // vert, texcoord


	GLuint READ_FBO_buf;

	///////////////////////
	// draw class 
	//repulsive_force* draw_object;
	swingBy* draw_object;

	draw_scratch* draw_scratch_obj;
	annolab_logo* annolab_logo_obj;
	dotPattern* dot_pattern_obj;
	repulsive_force* repulsive_obj;
	swingBy* swingBy_obj;

	mainController();
	~mainController();


	void drawTimer();

	void init_openGL();
	void setupSampler();
	void setupShader();
	void setupVAO();
	void setupFBO();

	void startTimer();
	void stopTimer();

	void unique_process();
	void save_FBO_to_bmp();
	void send_randomize_message();

	// GUI method
	void gui_button(UINT id);
	void gui_slider(UINT id, int pos);
	void gui_combobox(UINT id);
};

