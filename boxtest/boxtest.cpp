#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <fstream>

//Set Texture
GLuint texWall;
GLuint texDoor;
GLuint texWindow;

GLuint loadBMP(const char* filename) {
	FILE* f;
	fopen_s(&f, filename, "rb");
	if (!f) return 0;

	unsigned char header[54];
	fread(header, 1, 54, f);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];

	int imageSize = 3 * width * height;
	unsigned char* data = new unsigned char[imageSize];
	fread(data, 1, imageSize, f);
	fclose(f);

	// Create texture
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete[] data;
	return texID;
}

//Camera
float camX = 1.0f, camY = 0.0f, camZ = 5.0f;
float targetX = 0.0f, targetY = 0.0f, targetZ = 0.0f;
float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;

//Plane
void drawPlane(){
}

//Cube for House
void texturedCube(float size, GLuint texture, float repeat=1.0f) {
	float s = size / 2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	// Front
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
	glTexCoord2f(repeat, 0); glVertex3f(s, -s, s);
	glTexCoord2f(repeat, repeat); glVertex3f(s, s, s);
	glTexCoord2f(0, repeat); glVertex3f(-s, s, s);

	// Back
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
	glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
	glTexCoord2f(1, 1); glVertex3f(s, s, -s);
	glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

	// Left
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
	glTexCoord2f(1, 0); glVertex3f(-s, -s, s);
	glTexCoord2f(1, 1); glVertex3f(-s, s, s);
	glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

	// Right
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
	glTexCoord2f(1, 0); glVertex3f(s, -s, s);
	glTexCoord2f(1, 1); glVertex3f(s, s, s);
	glTexCoord2f(0, 1); glVertex3f(s, s, -s);

	// Top
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-s, s, -s);
	glTexCoord2f(1, 0); glVertex3f(s, s, -s);
	glTexCoord2f(1, 1); glVertex3f(s, s, s);
	glTexCoord2f(0, 1); glVertex3f(-s, s, s);

	// Bottom
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
	glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
	glTexCoord2f(1, 1); glVertex3f(s, -s, s);
	glTexCoord2f(0, 1); glVertex3f(-s, -s, s);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

//Roof
void texturedRoof(float width, float height, float depth, GLuint texture, float repeat=1.0f) {
	float w = width / 2;
	float d = depth / 2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, height, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-w, 0, -d);
	glTexCoord2f(repeat, 0.0f); glVertex3f(-w, 0, d);
	glTexCoord2f(repeat, repeat); glVertex3f(0, height, d);
	glTexCoord2f(0.0f, repeat); glVertex3f(0, height, -d);
	glEnd();

	// Right slope
	glBegin(GL_QUADS);
	glNormal3f(1.0f, height, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0, -d);
	glTexCoord2f(repeat, 0.0f); glVertex3f(w, 0, d);
	glTexCoord2f(repeat, repeat); glVertex3f(0, height, d);
	glTexCoord2f(0.0f, repeat); glVertex3f(0, height, -d);
	glEnd();

	// Front triangle
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, height, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-w, 0, d);
	glTexCoord2f(repeat, 0.0f); glVertex3f(w, 0, d);
	glTexCoord2f(repeat / 2, repeat); glVertex3f(0, height, d); // peak
	glEnd();

	// Back triangle
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, height, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-w, 0, -d);
	glTexCoord2f(repeat, 0.0f); glVertex3f(w, 0, -d);
	glTexCoord2f(repeat / 2, repeat); glVertex3f(0, height, -d); // peak
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

//3D Box
void house(){
	glMatrixMode(GL_MODELVIEW);
	
	glScalef(scaleX, scaleY, scaleZ);

	glPushMatrix();
		texturedCube(2, texWall); //House Base

		glTranslatef(0, 1, 0);
		glPushMatrix();                     // Roof
		texturedRoof(2.50, 1, 2, texDoor);  // <- please change this it sucks ass
		glPopMatrix();

		glPushMatrix();						// Door
		glTranslatef(0.525, -1.275, 1); 
		glScalef(1.2, 2.75, 0.1);
		texturedCube(.5, texDoor, 2.0f);
		glPopMatrix();

		glPushMatrix();					    // Window
		glTranslatef(-.415, -0.85, 1); 
		glScalef(1.25, 1.25, 0.01);
		texturedCube(.5, texWindow, 1.0f);  // This texture sucks ass
		glPopMatrix();

	glPopMatrix();

	glFlush();
}

//Camera Movements
void keyboardFunc(unsigned char key, int x, int y) {
	if (key == 27 || key == 'q' || key == 'Q') // ESC or q to quit
		std::exit(0);
	switch (key)
	{
	case 'w': targetY -= .25f; break;
	case 's': targetY += .25f; break;
	case 'a': targetX += .25f; break;
	case 'd': targetX -= .25f; break;
	case 'i': camY += .1f; break;
	case 'k': camY -= .1f; break;
	case 'j': camX -= .1f; break;
	case 'l': camX += .1f; break;
	case 'e': scaleZ = scaleY = scaleX += .1f; break;
	case 'r': scaleZ = scaleY = scaleX -= .1f; break;

	}
	glutPostRedisplay();
}

//Resize window
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

//Lighting
void lighting() {

	GLfloat lightpos[] = { 0.0, 0.0, 15.0 };
	GLfloat lightcolor[] = { 1.0, 1.0, 0.0 };
	GLfloat ambcolor[] = { 0.0, 0.0, 1.0 };

	glEnable(GL_LIGHTING);                               
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambcolor);     

	glEnable(GL_LIGHT0);                                 
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);           
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightcolor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcolor);

	
}

//Display
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, targetX, targetY, targetZ, 0.0, 1.0, 0.0);
	house();
}

//Initialize 
void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	texWall = loadBMP("wall.bmp");
	texDoor = loadBMP("door.bmp");
	texWindow = loadBMP("window.bmp");
	//Debug Texture
		/*if (!texWall) {
			printf("FAILED TO LOAD wall.bmp\n");
		}
		else {
			printf("Loaded wall.bmp successfully!\n");
		}*/
	lighting();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	
	glutCreateWindow("Test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardFunc);
	
	init();
	glutMainLoop();
	return 0;
}
