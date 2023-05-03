#include "mainController.h"

mainController* this_obj;


VOID CALLBACK drawWithMessageTimer(HWND hWnd, UINT msg, UINT idEvent, DWORD dwTime)
{
	this_obj->drawTimer();
}


mainController::mainController()
{
	// constructor
	srand(time(NULL));

	this_obj = this;
	gl_manager_obj = new OpenGL_Manager_Class();
	window_manager_obj = new Window_Manager_Class();
	window_manager_obj->setMainController(this);
	gui_manager_obj = new GUI_Manager_Class();

	bitmap_save_obj = new Bitmap_Save_Class();

	// init opengl
	this->init_openGL();



	// draw object ///////////////////////////////////////////////
	draw_scratch_obj = new draw_scratch();
	draw_scratch_obj->gl_obj = gl_manager_obj;
	draw_scratch_obj->setup_gl();
	////////////////////////////////////////////////////////////
	annolab_logo_obj = new annolab_logo();
	annolab_logo_obj->gl_obj = gl_manager_obj; 
	annolab_logo_obj->COMMON_PRG = &NORMAL_PRG; // common normal shader
	annolab_logo_obj->setup_gl();
	/////////////////////////////////////////////////////////////
	dot_pattern_obj = new dotPattern();
	dot_pattern_obj->gl_obj = gl_manager_obj;
	dot_pattern_obj->COMMON_PRG = &NORMAL_PRG;
	dot_pattern_obj->setup_gl();
	/////////////////////////////
	repulsive_obj = new repulsive_force();
	repulsive_obj->gl_obj = gl_manager_obj;
	repulsive_obj->COMMON_PRG = &NORMAL_PRG;
	repulsive_obj->setup_gl();
	//////////////////////////////
	swingBy_obj = new swingBy();
	swingBy_obj->gl_obj = gl_manager_obj;
	swingBy_obj->COMMON_PRG = &NORMAL_PRG;
	swingBy_obj->setup_gl();

	// target object
	//draw_object = dot_pattern_obj;
	draw_object = swingBy_obj;
	


	// FBO, shader, texture....
	this->setupSampler();
	this->setupShader();
	this->setupVAO();
	this->setupFBO();

	// timer
	//this->startTimer();


}


mainController::~mainController()
{
	// destructor
}



void mainController::init_openGL()
{
	// create dummy window
	HWND dummyWnd = window_manager_obj->create_borderless_window(L"dummy");

	// create base window
	base_window_obj = window_manager_obj->create_panel_window(L"baseWin", L"base window");
	window_manager_obj->set_window_position(base_window_obj, 100, 100);
	window_manager_obj->set_window_size(base_window_obj, 800, 700);

	
	// add gl view
	gl_view_obj = gui_manager_obj->create_textfield(base_window_obj,
		NULL,
		0,
		0,
		650,
		650,
		1111);
	
	// init opengl
	gl_manager_obj->init_OpenGL_functions(dummyWnd, gl_view_obj);

	// close dummy window
	window_manager_obj->close_window(dummyWnd);

	// set opengl status
	gl_manager_obj->set_OpenGL_status();

	///////////////////////////
	// add GUI
	// timer ON/OFF
	gui_manager_obj->create_button(base_window_obj, L"timer", 660, 20, 60, 25, 201);
	
	// 1 frame rendering
	gui_manager_obj->create_button(base_window_obj, L"1 frame", 660, 60, 60, 25, 202);
	
	// slider
	//gui_manager_obj->create_slider(base_window_obj, 7, 660, 100, 120, 20, 203);

	// combo box
	comboWnd = gui_manager_obj->create_combobox(base_window_obj, 660, 120, 120, 160, 300);
	gui_manager_obj->add_combobox_item(comboWnd, L"3");
	gui_manager_obj->add_combobox_item(comboWnd, L"4");
	gui_manager_obj->add_combobox_item(comboWnd, L"5");
	gui_manager_obj->add_combobox_item(comboWnd, L"6");

	// randomize button
	gui_manager_obj->create_button(base_window_obj, L"randomize", 660, 200, 80, 25, 204);

	// koyu
	gui_manager_obj->create_button(base_window_obj, L"export script", 660, 250, 80, 25, 205);

	// save bmp button
	gui_manager_obj->create_button(base_window_obj, L"save bmp", 660, 400, 60, 25, 203);
}




void mainController::setupSampler()
{
	// gernerate sampler
	glGenSamplers(1, &Sampler_name);

	// setup sampler
	glSamplerParameteri(Sampler_name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(Sampler_name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(Sampler_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(Sampler_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set sampler to all texture object
	GLint maxTex;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex);

	for (int i = 0; i < maxTex; i++)
	{
		glBindSampler(i, Sampler_name);
	}
}



void mainController::setupShader()
{
	printf("\n*** SETUP SHADER ***\n");

	gl_manager_obj->loadShaderSource_And_CompileShader("NORMAL_VS.txt", 0, &NORMAL_VS);
	gl_manager_obj->loadShaderSource_And_CompileShader("NORMAL_FS.txt", 2, &NORMAL_FS);
	gl_manager_obj->createProgram_And_AttachShader(&NORMAL_PRG, &NORMAL_VS, NULL, &NORMAL_FS);


	gl_manager_obj->loadShaderSource_And_CompileShader("final_VS.txt", 0, &FINAL_VS);
	gl_manager_obj->loadShaderSource_And_CompileShader("final_FS.txt", 2, &FINAL_FS);
	gl_manager_obj->createProgram_And_AttachShader(&FINAL_PRG, &FINAL_VS, NULL, &FINAL_FS);
	gl_manager_obj->getUniformLocation(&FINAL_PRG, &UNF_texImage, "texImage");
}



void mainController::setupVAO()
{
	// gen vao
	glGenVertexArrays(1, &VAO_final);
	// gen buffer
	glGenBuffers(2, VBO_final);

	// bind vertex array
	glBindVertexArray(VAO_final);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// alloc buffer ( for final board )
	glBindBuffer(GL_ARRAY_BUFFER, VBO_final[0]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_STORAGE_BIT);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_final[1]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, NULL, GL_DYNAMIC_STORAGE_BIT);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}



void mainController::setupFBO()
{
	// generate FBO
	glGenFramebuffers(1, &FBO_name);

	// generate texture for FBO
	glGenTextures(2, FBOTEX_name);

	// bind texture unit & texture object/////////////////////////////////
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTEX_name[0]);
	// alloc memory
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, RENDER_WIDTH, RENDER_HEIGHT);// color


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, FBOTEX_name[1]);
	// alloc memory
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, RENDER_WIDTH, RENDER_HEIGHT);

	glActiveTexture(0);


	// attach texture to FBO ///////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_name);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		FBOTEX_name[0],
		0);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		FBOTEX_name[1],
		0);


	// setup draw buffer ////////////////////////////////
	GLenum bufs[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, bufs);

	// setup read buffer
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	// check FBO status /////////////////////////////////////
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	printf("\nFBO status %x (8cd5 is OK)\n", status);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	///////////// read buffer setup //////////////
	// gen buffer
	glGenBuffers(1, &READ_FBO_buf);
	// attach buffer to FBO copy attachment point
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, READ_FBO_buf);

	// alloc memory
	int memSize = RENDER_WIDTH * RENDER_HEIGHT * 4;
	glNamedBufferStorage(READ_FBO_buf, memSize, NULL, GL_MAP_READ_BIT);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

}




void mainController::startTimer()
{
	if (timerID == 0)
	{
		timerID = SetTimer(NULL, 100, 15, (TIMERPROC)drawWithMessageTimer); // 30FPS
	}
}

void mainController::stopTimer()
{
	if(timerID != 0)
	{
		KillTimer(NULL, timerID);
		timerID = 0;
	}
}


////////////////////////////////////////////////////////////////////

void mainController::gui_button(UINT id)
{
	switch (id)
	{
	case 201:// timer button
		if (timerID == 0)
		{
			this->startTimer();
		}
		else
		{
			this->stopTimer();
		}
		break;

	case 202:// 1 frame button
		this->drawTimer();
		break;

	case 203: // save bmp
		this->save_FBO_to_bmp();
		break;

	case 204:
		this->send_randomize_message();
		this->drawTimer();
		break;

	case 205:
		this->unique_process();
		break;

	drfault:
		break;
	}
}

void mainController::gui_slider(UINT id, int pos)
{
	printf("slider ID:%d, pos:%d\n", id, pos);

}


void mainController::gui_combobox(UINT id)
{
	// get combobox item index
	int idx = SendMessage(comboWnd, CB_GETCURSEL, 0, 0);
	
	current_layer = idx;
	this->drawTimer();
}

///////////////////////////////////////////////////////////////////


void mainController::drawTimer()
{
	// texture draw
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_name);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGHT);



	// current draw object ***********************
	draw_object->draw(current_layer);
	// current draw object ***********************




	////////////////////////////////////////////////////////////////
	// screen draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 650, 650);


	// change shader
	glUseProgram(FINAL_PRG);
	glUniform1i(UNF_texImage, 0);// FBO color texture0


	GLfloat b_vert[4][4];
	GLfloat b_texc[4][2];

	for (int i = 0; i < 4; i++)
	{
		b_vert[i][2] = 0.0;
		b_vert[i][3] = 1.0;
	}

	float sqSize = 1.0;
	b_vert[0][0] = -sqSize;		b_vert[0][1] = sqSize;
	b_vert[1][0] = -sqSize;		b_vert[1][1] = -sqSize;
	b_vert[2][0] = sqSize;		b_vert[2][1] = sqSize;
	b_vert[3][0] = sqSize;		b_vert[3][1] = -sqSize;

	b_texc[0][0] = 0.0;	b_texc[0][1] = 1.0;
	b_texc[1][0] = 0.0;	b_texc[1][1] = 0.0;
	b_texc[2][0] = 1.0;	b_texc[2][1] = 1.0;
	b_texc[3][0] = 1.0;	b_texc[3][1] = 0.0;


	// update buffer
	glBindVertexArray(VAO_final);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_final[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * 4, b_vert);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_final[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * 2, b_texc);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glFlush();
	SwapBuffers(gl_manager_obj->OpenGL_HDC);


}





void mainController::send_randomize_message()
{
	// current draw object ***********************
	draw_object->randomize();
	// current draw object ***********************

}



void mainController::unique_process()
{
	// current draw object ***********************
	draw_object->unique_process(current_layer);
	// current draw object ***********************
}












void mainController::save_FBO_to_bmp()
{
	if (timerID != 0) // if timer is ON
	{
		printf("timer is ON,,, return\n");
		return;
	}

	////////////////////////////////////////////////////////

	// bind FBO to read point
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_name);

	// bind READ buffer to download point
	glBindBuffer(GL_PIXEL_PACK_BUFFER, READ_FBO_buf);

	// read pixel
	glReadPixels(0, 0, RENDER_WIDTH, RENDER_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// get pointer of readbuffer
	unsigned char* bufPtr = (unsigned char*)glMapNamedBuffer(READ_FBO_buf, GL_READ_ONLY);

	///////////////////////////////////////////////////////


	// get desktop path
	TCHAR desktop_path_w[MAX_PATH];
	char desktop_path[MAX_PATH];
	char filename_0[] = "\\temp0.bmp";
	char filename_1[] = "\\temp1.bmp";
	char filename_2[] = "\\temp2.bmp";
	char filename_3[] = "\\temp3.bmp";
	char filename_4[] = "\\temp4.bmp";
	char filename_5[] = "\\temp5.bmp";

	char* filename_ptr[6];
	filename_ptr[0] = filename_0;
	filename_ptr[1] = filename_1;
	filename_ptr[2] = filename_2;
	filename_ptr[3] = filename_3;
	filename_ptr[4] = filename_4;
	filename_ptr[5] = filename_5;

	SHGetSpecialFolderPath(NULL, desktop_path_w, CSIDL_DESKTOP, 0);

	WideCharToMultiByte(CP_ACP, 0, desktop_path_w, -1, desktop_path, MAX_PATH, NULL, NULL);

	// concat filepath
	strcat_s(desktop_path, sizeof(desktop_path), filename_ptr[current_layer]);


	// save to file
	bitmap_save_obj->save_RGBA_bmp_to_file(
		desktop_path,
		bufPtr,
		RENDER_WIDTH, RENDER_HEIGHT);

	// umpap
	glUnmapNamedBuffer(READ_FBO_buf);
}
