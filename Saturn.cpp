#include <iostream>
#include <glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SOIL.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;

class Planet{

public:
	Planet();
	GLuint GetTexture(const char *);
	void BackGroundTexture();
	void LongLat(float);
	void Orbit(float, float);
	void Saturn(float, GLuint, GLUquadricObj*);
	void Moon(float, float,float,float, GLuint, GLUquadricObj*);
	void SunLight(float);
	void ring(GLuint);

};

void Planet::ring(GLuint texture)
{
	GLUquadricObj *disk;
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTranslatef(1.0, 1.0, 0.0);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0, 0.41f, 1.0f, 0.0f);
	//glColor3f(1, 0, 1);
	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricTexture(disk, GL_TRUE);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluDisk(disk, 3.5, 5.5, 75, 1);
	glPopMatrix();

}
Planet::Planet()
{
	return;
}


GLuint Planet::GetTexture(const char *file)
{
	//const char * filename = file;
	GLuint texture_id;
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGB);
	texture_id = SOIL_load_OGL_texture(
		file,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_COMPRESS_TO_DXT
		| SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
		);

	if (texture_id > 0)
	{
		delete image;
		return texture_id;
	}
	else
		return 0;
}
void Planet::SunLight(float lightAngle)
{
	float XPos = -9, ZPos = 9;
	XPos = (float)sin(lightAngle) * 11;
	ZPos = (float)cos(lightAngle) * 8;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float ambientLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	//float specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
	float position[] = { XPos, 0.0f, ZPos, 1.0f };
	//Assign created components to GL_LIGHT0 
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); 
	glLightfv(GL_LIGHT0, GL_POSITION, position);

}

//render Saturn
void Planet::Saturn(float saturnangle, GLuint texture, GLUquadricObj* Sphere)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glRotatef(50.0, 1.0f, 0.0f, 0.0f);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0, 0.0f, 1.0f, 0.0f);
	glRotatef(saturnangle, 0.0f, 0.0f, 1.0f);
	gluQuadricDrawStyle(Sphere, GLU_FILL);
	gluQuadricTexture(Sphere, GL_TRUE);
	gluQuadricNormals(Sphere, GLU_SMOOTH);
	gluSphere(Sphere, 3, 32, 32);
	glPopMatrix();
}
//render Moon
void Planet::Moon(float xAxis,float yAxis, float zAxis,float size, GLuint texture, GLUquadricObj* Sphere)
{
	glPushMatrix();
	glTranslatef(0, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTranslatef(xAxis, yAxis, zAxis);
	gluQuadricDrawStyle(Sphere, GLU_FILL);
	gluQuadricTexture(Sphere, GL_TRUE);
	gluQuadricNormals(Sphere, GLU_SMOOTH);
	gluSphere(Sphere, size, 32, 32);
	glPopMatrix();
}

//display the Orbit
void Planet::Orbit(float x, float z)
{
	glPushMatrix();
	glColor3f(1, 1, 1);
	glVertex3f(x, 0, z);
	glPopMatrix();
}

//display the Longitudes and Latitudes
void Planet::LongLat(float saturnangle)
{
	glPushMatrix();

	glColor3f(1, 1, 1);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glRotatef(saturnangle, 0.0f, 0.0f, -1.0f);
	glutWireSphere(3.03f, 36, 18);
	//glClearColor(1,1,1,1);
	glPopMatrix();

	return;
}
#define TWOPI 2*3.142
//texture[0]: Background texture, texture[1]:Earth texture and texture[2]:Moon texture
static GLuint texture[5];
//Quads for the Sphere		
GLUquadricObj* Sphere;
/*saturnangle is the Earth's angle of Rotation
moonAngle is the angle of Orbit at which the Moon has to revolve around earth
lightAngle is the angle of Orbit at which the Sun(Light Source) revolves
(here we assume the Earth is at centre)*/
float saturnangle = 0, moonAngle1 = 0, moonAngle2 = 2, moonAngle3 = 3, moonAngle4 = 4, moonAngle5=3.5, lightAngle = 0;
// angle of rotation for the camera direction
float CameraAngle = 0.0;
// actual vector representing the camera's direction
float xPlane = 0.0f,yPlane=0.0f, zPlane = -1.0f;
//position of the camera in the XZ plane
float x = 0.0f,y=1.0f, z = 5.0f;
//Create an object of the class Planet
Planet planet;

void SelectTexture(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			CameraAngle += 0.02f;
			yPlane = sin(CameraAngle);
			zPlane = -cos(CameraAngle);
			break;
		case 's':
			CameraAngle -= 0.02f;
			yPlane = sin(CameraAngle);
			zPlane = -cos(CameraAngle);
			break;
	}
}
//Changing the Camera View
void MoveCamera(int key, int xx, int yy)
{

	float Change = 0.1f;

	switch (key)
	{
	case GLUT_KEY_LEFT:
		CameraAngle -= 0.02f;
		xPlane = sin(CameraAngle);
		zPlane = -cos(CameraAngle);
		break;
	case GLUT_KEY_RIGHT:
		CameraAngle += 0.02f;
		xPlane = sin(CameraAngle);
		zPlane = -cos(CameraAngle);
		break;
	case GLUT_KEY_UP:
		x += xPlane * Change;
		y += yPlane * Change;
		z += zPlane * Change;
		break;
	case GLUT_KEY_DOWN:
		x -= xPlane * Change;
		z -= zPlane * Change;
		y -= yPlane * Change;
		break;
	}
}

void init()
{
	//glClearColor(1,1,1,1);
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Sphere = gluNewQuadric();
	//planet.BackGroundTexture();
	//Saturn's Texture
	texture[1] = planet.GetTexture("saturnmap.png");
	glutKeyboardFunc(SelectTexture);
	//Moon's Texture
	texture[2] = planet.GetTexture("Moon.png");
	texture[0] = planet.GetTexture("saturnringcolor.jpg");
}

//Resize the window whenever the window dimensions are explicitly changed
void Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);

}

void display()
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Set the Camera Position
	gluLookAt(x, y, z, x + xPlane, y + yPlane, z + zPlane, 0.0f, 1.0f, 0.0f);
	//Bring the view away from the centre 
	glTranslatef(0.0f, 0.0f, -16.0f);

	//Enable the Sunlight and intitialize its Position
	planet.SunLight(lightAngle);

	//Saturn
	planet.Saturn(saturnangle, texture[1], Sphere);
	planet.ring(texture[0]);
	//Longitudes and latitudes
	//planet.LongLat(saturnangle);

	//Calculate the orbit on which the Moon revolves around Earth
	float xAxis1 = (float)sin(moonAngle1) * 7;
	float zAxis1 = (float)cos(moonAngle1) * 5;

	//display the orbit
	planet.Orbit(xAxis1, zAxis1);

	//Moon
	planet.Moon(xAxis1,0, zAxis1,0.6f, texture[2], Sphere);

	float xAxis2 = (float)sin(moonAngle2) * 9;
	float zAxis2 = (float)cos(moonAngle2) * 7;
	planet.Moon(xAxis2,4, zAxis2,0.3f, texture[2], Sphere);

	float xAxis3 = (float)sin(moonAngle3) * 10;
	float zAxis3 = (float)cos(moonAngle3) * 7;
	planet.Moon(xAxis3,-4, zAxis3, 0.2f, texture[2], Sphere);

	float xAxis4 = (float)sin(moonAngle4) * 6;
	float zAxis4 = (float)cos(moonAngle4) * 5;
	planet.Moon(xAxis4, 3, zAxis4, 0.3f, texture[2], Sphere);

	float xAxis5 = (float)sin(moonAngle5) * 9;
	float zAxis5 = (float)cos(moonAngle5) * 2;
	planet.Moon(xAxis5, 1, zAxis5, 0.3596f, texture[2], Sphere);
	//Swap the Buffers and display
	glutSwapBuffers();

	//Disable the Lights and the textures
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glFlush();
}
void wait(int value)
{
	moonAngle1 += 0.005f;
	if (moonAngle1>TWOPI)
	{
		moonAngle1 = 0;
	}

	moonAngle2 += 0.009f;
	if (moonAngle2>TWOPI)
	{
		moonAngle2 = 0;
	}

	moonAngle3 += 0.003f;
	if (moonAngle3>TWOPI)
	{
		moonAngle3 = 0;
	}

	moonAngle4 += 0.007f;
	if (moonAngle4>TWOPI)
	{
		moonAngle4 = 0;
	}

	moonAngle5 += 0.01f;
	if (moonAngle5>TWOPI)
	{
		moonAngle5 = 0;
	}

	lightAngle += 0.002f;
	if (lightAngle>TWOPI)
	{
		lightAngle = 0;
	}

	saturnangle += 0.1f;
	if (saturnangle>360.0f)
	{
		saturnangle = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(25, wait, 0);
}


//Pause the Rotation
void Pause(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i<1000000000; i++);
		return;

	}
}

int main(int argc, char** argv)
{
	//Initializers
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Saturn and its satellites");
	init();
	//Set the CallBack functions
	glutDisplayFunc(display);
	glutReshapeFunc(Resize);
	glutTimerFunc(25, wait, 0);
	glutSpecialFunc(MoveCamera);
	glutMouseFunc(Pause);
	//Infinite Loop
	glutMainLoop();
	return 0;
}
