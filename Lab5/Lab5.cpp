/* CS6555 Computer Animation, Fall 2014
* Lab 5£ºFinal Project - Particle System
* Edited by Fei Yan (Email:hcifaye@gwu.edu)
* Reference: Lab 0's code ; Lab 1's code;
*			 Lab 2's code; Lab 3's code;
*			 NeHe's OpenGL Tutorial Lesson 19 - Particle Tutorial;
*            Nehe's OpenGL Tutorial Lesson  5 - Texture
* Basic Steps:
*		Step 1: define the particle structure
*		Step 2: initialize particles with random value
*		Step 3: update particle state
*		Step 4: if one particle died, re-birth it. 
*/

#include "stdafx.h"
#include <GL/glut.h>
#include <olectl.h>   // OLE control for LoadTexture
#include "Texture.h"

#define MAX_PARTICLE_NUM 1000

// storage space for particle texture and background
GLuint texture[2]; 
GLuint listname = 1;

//====================================
// define the structure for particles
//====================================

typedef struct
{
	// particle position
	GLfloat x;
	GLfloat y;
	GLfloat z;
	// particle volectiy
	GLfloat vx;
	GLfloat vy;
	GLfloat vz;
	// acceration
	GLfloat ax;
	GLfloat ay;
	GLfloat az;
	// life value
	GLfloat life;
	// size
	GLfloat size;
}particle;
particle particles[MAX_PARTICLE_NUM];

//=======================================
// initialize particles kinematic status
//=======================================
void iniParticleKinematic()
{
	for (int i = 0; i < MAX_PARTICLE_NUM; i++)
	{
		particles[i].life = rand()%300;
		particles[i].size = rand() % 3 + 1;
		particles[i].x = float(rand() % 250 - 90.0);
		particles[i].y = float(rand() % 30 + 150);
		particles[i].z = float(rand() % 250 - 90.0);
		particles[i].vy = float(rand() % 10 - 4) / 40;
		particles[i].vx = float(rand() % 10 - 4) / 80;
		particles[i].vz = float(rand() % 10 - 4) / 40;
		particles[i].ax = -1.0 / 10000;
		particles[i].ay = -3.0 / 50000;
		particles[i].az = -1.0 / 10000;
	}

}

//====================================
// iniParticle: initialize particles 
//====================================
void iniParticle()
{
	// clear the background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// color and transparency for squares that holding particles
	glColor4f(1.0, 1.0, 1.0, 0.8f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	iniParticleKinematic();

	// load images into texture[2]
	LoadTexture("Christmas Tree.jpg", texture[0]);
	LoadTexture("Particle.bmp", texture[1]);

	// create list to store texture for particles
	glNewList(listname, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Create Squres for particles movement 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-1, -1, 0.0);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 0.0);
	glTexCoord2f(1, 1); glVertex3f(1, 1, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEndList();
}

//=======================================
// Display: Display all the particles
//=======================================
void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display the background
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-130.0, -120.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(130.0, -120.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(130.0, 120.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(-130.0, 120.0, -1.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// display particles
	for (int i = 0; i<MAX_PARTICLE_NUM; i++)
	{
		// when particles are alive
		if (particles[i].life>0)
		{
			glPushMatrix();
			glTranslatef(particles[i].x, particles[i].y, 0.0);
			glScalef(particles[i].size, particles[i].size, 1.0);
			//readin particles'texture
			glCallList(listname);
			glPopMatrix();

			//update particles' states
			particles[i].life -= float(rand() % 100) / 1000.0f + 0.0003f;
			particles[i].vx += particles[i].ax;
			particles[i].vy += particles[i].ay;
			particles[i].vz += particles[i].az;
			particles[i].x += particles[i].vx;
			particles[i].y += particles[i].vy;
			particles[i].z += particles[i].vz;
		}
		// when particles are dead, renew particles
		if (particles[i].life <= 0)
		{
			particles[i].life = rand() % 200;
			particles[i].size = rand() % 3 + 1;
			particles[i].x = rand() % 250 - 90.0;
			particles[i].y = rand() % 30 + 150;
			particles[i].z = rand() % 250 - 90.0;
			particles[i].vx = float(rand() % 10 - 4) / 80;
			particles[i].vy = float(rand() % 10 - 4) / 40;
			particles[i].vz = float(rand() % 10 - 4) / 80;
			particles[i].ax = -1.0 / 10000;
			particles[i].ay = -3.0 / 50000;
			particles[i].az = -1.0 / 10000;
		}
	}
	glFlush();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 210.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab 5 - Particle System - Merry Christmas - Fei Yan");
	iniParticle();
	glutReshapeFunc(reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutMainLoop();
	return 0;
}
