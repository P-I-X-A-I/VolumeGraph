#include "swingBy.h"

swingBy::swingBy()// constructor
{


	for (int i = 0; i < NUM_PLANET; i++)
	{
		PNT_XY[i][0] = (rand() % 2000 - 1000)*0.001;
		PNT_XY[i][1] = (rand() % 2000 - 1000)*0.001;
		PNT_WEIGHT[i] = 0.01;
	}


	double tempVel[2];
	tempVel[0] = (rand() % 200 - 100)*0.0001;
	tempVel[1] = (rand() % 200 - 100)*0.0001;


	double DEG = rand() % 360;
	double CENTER[2];
	CENTER[0] = cos(DEG*0.0174532925);
	CENTER[1] = sin(DEG*0.0174532925);


	for (int i = 0; i < SWING_PT; i++)
	{
		double rad = (rand() % 360)*0.0174532925;
		double R = (rand() % 100)*0.008;
		PT_POS[i][0] = CENTER[0] + cos(rad)*R;
		PT_POS[i][1] = CENTER[1] + sin(rad)*R;
		PT_POS[i][2] = 0.0;
		PT_POS[i][3] = 1.0;

		PT_STOCK[i][0] = PT_POS[i][0];
		PT_STOCK[i][1] = PT_POS[i][1];

		for (int t = 0; t < SWING_TAIL; t++)
		{
			PT_TAIL[i][t][0] = PT_POS[i][0];
			PT_TAIL[i][t][1] = PT_POS[i][1];
			PT_TAIL[i][t][2] = 0.0;
			PT_TAIL[i][t][3] = 1.0;
		}


		//PT_VELOCITY[i][0] = (rand() % 200 - 100)*0.00001;
		//PT_VELOCITY[i][1] = (rand() % 200 - 100)*0.00001;

		PT_VELOCITY[i][0] = tempVel[0];
		PT_VELOCITY[i][1] = tempVel[1];

		PT_WEIGHT[i] = 0.001;
	}
}


swingBy::~swingBy()
{
	
}


void swingBy::setup_gl()
{
	// gen VAO, VBO
	glGenVertexArrays(1, &VAO_name);
	glGenBuffers(1, &VBO_name);

	// bind, enable vertex attrib array
	glBindVertexArray(VAO_name);
	glEnableVertexAttribArray(0);
}



void swingBy::randomize()
{
	printf("swingBy randomize\n");

	for (int i = 0; i < NUM_PLANET; i++)
	{
		PNT_XY[i][0] = (rand() % 2000 - 1000)*0.001;
		PNT_XY[i][1] = (rand() % 2000 - 1000)*0.001;
		PNT_WEIGHT[i] = 0.01;
	}

	double tempVel[2];
	tempVel[0] = (rand() % 200 - 100)*0.0001;
	tempVel[1] = (rand() % 200 - 100)*0.0001;


	double DEG = rand() % 360;
	double CENTER[2];
	CENTER[0] = cos(DEG*0.0174532925);
	CENTER[1] = sin(DEG*0.0174532925);


	for (int i = 0; i < SWING_PT; i++)
	{
		double rad = (rand() % 360)*0.0174532925;
		double R = (rand() % 100)*0.005;
		PT_POS[i][0] = CENTER[0] + cos(rad)*R;
		PT_POS[i][1] = CENTER[1] + sin(rad)*R;
		PT_POS[i][2] = 0.0;
		PT_POS[i][3] = 1.0;

		PT_STOCK[i][0] = PT_POS[i][0];
		PT_STOCK[i][1] = PT_POS[i][1];

		for (int t = 0; t < SWING_TAIL; t++)
		{
			PT_TAIL[i][t][0] = PT_POS[i][0];
			PT_TAIL[i][t][1] = PT_POS[i][1];
			PT_TAIL[i][t][2] = 0.0;
			PT_TAIL[i][t][3] = 1.0;
		}

		PT_VELOCITY[i][0] = tempVel[0];
		PT_VELOCITY[i][1] = tempVel[1];

		PT_WEIGHT[i] = 0.001;
	}
}



void swingBy::unique_process(int layer)
{
	printf("swingBy uniqueprocess\n");
}






void swingBy::animation_loop()
{
	for (int i = 0; i < SWING_PT; i++)
	{
		double F[2] = {0.0, 0.0};
		
		// PLANET gravity
		for (int P = 0; P < NUM_PLANET; P++)
		{
			double tempF[2];
			double dist[2];
			double len;
			dist[0] = PNT_XY[P][0] - PT_POS[i][0];
			dist[1] = PNT_XY[P][1] - PT_POS[i][1];

			len = sqrt(dist[0] * dist[0] +
						dist[1] * dist[1]);

			// normalized F
			if (len > 0.05)
			{
				dist[0] = dist[0] / len;
				dist[1] = dist[1] / len;

				tempF[0] = (dist[0] * PNT_WEIGHT[P] * PT_WEIGHT[i]) / (len * len);
				tempF[1] = (dist[1] * PNT_WEIGHT[P] * PT_WEIGHT[i]) / (len * len);
			}
			else
			{
				tempF[0] = 0.0;
				tempF[1] = 0.0;
			}

			// planet f


			// sum F
			F[0] += (tempF[0]);
			F[1] += (tempF[1]);

		} // for P ( Planet gravity )


		// each point gravity
		for (int n = 0; n < SWING_PT; n++)
		{
			if (n != i)
			{
				double unitVec[2];
				double D;
				double eF[2];

				unitVec[0] = PT_STOCK[n][0] - PT_POS[i][0];
				unitVec[1] = PT_STOCK[n][1] - PT_POS[i][1];

				D = sqrt(unitVec[0] * unitVec[0] + unitVec[1] * unitVec[1]);

				if (D > 0.01)
				{
					unitVec[0] = unitVec[0] / D;
					unitVec[1] = unitVec[1] / D;

					eF[0] = (unitVec[0] * PT_WEIGHT[n] * PT_WEIGHT[i]) / (D*D);
					eF[1] = (unitVec[1] * PT_WEIGHT[n] * PT_WEIGHT[i]) / (D*D);
				}
				else
				{
					eF[0] = 0.0;
					eF[1] = 0.0;
				}


				F[0] += (eF[0]*0.1);
				F[1] += (eF[1]*0.1);
			}
		} // for n (each point gravity )

		




		// calc velocity
		double brake = 1.0;
		PT_VELOCITY[i][0] = PT_VELOCITY[i][0]*brake + F[0];
		PT_VELOCITY[i][1] = PT_VELOCITY[i][1]*brake + F[1];

		// lim velocity
		double LIM_V = 0.02;
		if (PT_VELOCITY[i][0] > LIM_V)
		{
			PT_VELOCITY[i][0] = LIM_V;
		}
		else if (PT_VELOCITY[i][0] < -LIM_V)
		{
			PT_VELOCITY[i][0] = -LIM_V;
		}

		if (PT_VELOCITY[i][1] > LIM_V)
		{
			PT_VELOCITY[i][1] = LIM_V;
		}
		else if (PT_VELOCITY[i][1] < -LIM_V)
		{
			PT_VELOCITY[i][1] = -LIM_V;
		}



		// calc pos
		PT_POS[i][0] += PT_VELOCITY[i][0];
		PT_POS[i][1] += PT_VELOCITY[i][1];

		// loop x position
		double RANGE = 2.0;
		if (PT_POS[i][0] > RANGE)
		{
			PT_POS[i][0] = -RANGE;
		}
		else if (PT_POS[i][0] < -RANGE)
		{
			PT_POS[i][0] = RANGE;
		}
		// loop y position
		if (PT_POS[i][1] > RANGE)
		{
			PT_POS[i][1] = -RANGE;
		}
		else if (PT_POS[i][1] < -RANGE)
		{
			PT_POS[i][1] = RANGE;
		}

	}// for i



	// shift tail
	for (int i = 0; i < SWING_PT; i++)
	{
		for (int t = (SWING_TAIL - 1); t > 0; t--)
		{
			PT_TAIL[i][t][0] = PT_TAIL[i][t - 1][0];
			PT_TAIL[i][t][1] = PT_TAIL[i][t - 1][1];
		}

		PT_TAIL[i][0][0] = PT_POS[i][0];
		PT_TAIL[i][0][1] = PT_POS[i][1];

		PT_STOCK[i][0] = PT_POS[i][0];
		PT_STOCK[i][1] = PT_POS[i][1];
	}

}





void swingBy::draw(int layer)
{
	this->animation_loop();




	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use common shader
	glUseProgram(*COMMON_PRG);



	// bind vao
	glBindVertexArray(VAO_name);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * SWING_PT * SWING_TAIL * 4, &PT_TAIL[0][0][0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_DOUBLE, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0, SWING_PT*SWING_TAIL);

}








