//main
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <cmath>

void displayObject();
void init();
void genSpheres(GLfloat r, int deg, GLfloat startposX, GLfloat startposY, GLfloat startposZ, GLfloat arr[]);
void genColors(float r, float g, float b, float r_bias, float g_bias, float b_bias);
void genStars();
void drawSpheres();
void drawStars();
void drawText();
void drawAstronaut();
void draw_solid_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B);
void scrollControls(int type, int dir, int x, int y);
void mouseControls(int button, int state, int x, int y);
void keyControls(unsigned char key, int x, int y);
void animate(int value);

GLfloat PI = 3.1416;

GLfloat astroHead[2178];
GLfloat stars[36000];
GLfloat vertices[3630];
GLfloat normals[5808];
GLfloat colors[3630];
GLfloat opacity[600];
int vertices_count = 0;
int normals_count = 0;
int colors_count = 0;

GLfloat camX = 0;
GLfloat camY = 0;
GLfloat camZ = 2.5;
GLfloat camUp = 1.0;

GLfloat FPS = 60.0;
GLfloat YEAR_MS = 15000.0;
GLfloat DAY_MS = YEAR_MS/365.0;
GLfloat FRAME_MS = 1000/FPS;

GLfloat x_earthCore = 1.02;
GLfloat z_earthCore = 0;
GLfloat cos_merc = cos(PI * FRAME_MS/(0.5 * 88 * DAY_MS));
GLfloat sin_merc = sin(PI * FRAME_MS/(0.5 * 88 * DAY_MS));
GLfloat cos_venus = cos(PI * FRAME_MS/(0.5 * 225 * DAY_MS));
GLfloat sin_venus = sin(PI * FRAME_MS/(0.5 * 225 * DAY_MS));
GLfloat cos_earth = cos(PI * FRAME_MS/(0.5 * 365 * DAY_MS));
GLfloat sin_earth = sin(PI * FRAME_MS/(0.5 * 365 * DAY_MS));
GLfloat cos_moon = cos(PI * FRAME_MS/(0.5 * 27.3 * DAY_MS));
GLfloat sin_moon = sin(PI * FRAME_MS/(0.5 * 27.3 * DAY_MS));
GLfloat tempX_merc = 0;
GLfloat tempZ_merc = 0;
GLfloat tempX_venus = 0;
GLfloat tempZ_venus = 0;
GLfloat tempX_earthCore = 0;
GLfloat tempZ_earthCore = 0;
GLfloat tempX_earth = 0;
GLfloat tempZ_earth = 0;
GLfloat tempX_moon = 0;
GLfloat tempZ_moon = 0;
GLfloat tempX_normal = 0;
GLfloat tempZ_normal = 0;
GLfloat x_diff = 0;
GLfloat z_diff = 0;

GLfloat scale_val = 1;
GLfloat rotate_val = 0;
GLfloat translateX_val = 0;
GLfloat translateX_inc = 0.003;
GLfloat translateY_val = 0;
GLfloat translateY_inc = 0.0015;
GLfloat translateZ_val = 0;
GLfloat translateZ_inc = 0.003;

GLfloat astronautX = 0;
GLfloat astronautY = 0.3;
GLfloat astronautZ = 0;
GLfloat astronautD = 0;
GLfloat astro_scale = 0.05;	
	
GLfloat light_position[] = {0, 0, 0, 1};
GLfloat light_ambient[] = {1, 1, 3, 1};
GLfloat light_diffuse[] = {3, 3, 2, 1};
GLfloat light_specular[] = {1, 1, 0, 1};

GLfloat material_ambient[] = {0, 0, 0, 1};
GLfloat material_diffuse[] = {1, 1, 1, 1};
GLfloat material_specular[] = {1, 1, 1, 1};
GLfloat material_shininess = 100.0;

GLuint VBOid;
GLfloat *mappedVertices = NULL;
bool isStarted = false;
bool mouseRightHeld = false;
bool mouseLeftHeld = false;

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("");
	glutFullScreen();
	init();	
	glutDisplayFunc(displayObject);
	glutKeyboardFunc(keyControls);
	glutMouseFunc(mouseControls);
	glutMouseWheelFunc(scrollControls);
	glutTimerFunc(FRAME_MS, animate, 0);

	glutMainLoop();
}
 
void init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1920.0/1080.0, 1.0, 20.0);
	
	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	genSpheres(0.15f, 10, 0, 0, 0, vertices);
	genColors(255, 255, 255, 0.28, 0.3, 0);
	   
	genSpheres(0.02, 10, 0.36, 0, 0, vertices);
	genColors(109, 121, 140, 0, 0, 0);
	
	genSpheres(0.075, 10, 0.6525, 0, 0, vertices);
	genColors(140, 115, 80, 0, -0.2, 0);
	
	genSpheres(0.08, 10, 1.1, 0, 0, vertices);
	genColors(43, 100, 117, -0.7, 0.3, 0.2);
	
	genSpheres(0.02, 10, 1.3, 0, 0, vertices); vertices_count = 0;
	genColors(150, 150, 150, -0.2, -0.1, -0.1);
	
	genSpheres(0.33 * astro_scale, 10, -1.5 * astro_scale, -0.30 * astro_scale, 0.00 * astro_scale, astroHead);
	genSpheres(0.44 * astro_scale, 10, -1.5 * astro_scale,  0.00 * astro_scale, 0.00 * astro_scale, astroHead);
	genSpheres(0.28 * astro_scale, 10, -1.5 * astro_scale, -0.02 * astro_scale, 0.25 * astro_scale, astroHead);
	
	genStars();
	
	
	
	glewInit();
	glGenBuffers(1, &VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, (3630 + 2178 + 3630 + 5808)*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, (3630)*sizeof(GLfloat), 2178*sizeof(GLfloat), astroHead);
	glBufferSubData(GL_ARRAY_BUFFER, (3630 + 2178)*sizeof(GLfloat), 3630*sizeof(GLfloat), colors);
	glBufferSubData(GL_ARRAY_BUFFER, (3630 + 2178 + 3630)*sizeof(GLfloat), 5808*sizeof(GLfloat), normals);
}

void displayObject(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,    0.0, 0.0, 0.0,    0.0, camUp, 0.0);
	
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);
	
	
	drawSpheres();
	drawStars();
	drawText();
	
	glTranslatef(translateX_val, astronautY + translateY_val, translateZ_val);
	glRotatef(rotate_val, 0, 0, 1);
	glScalef(scale_val, scale_val, scale_val);
	drawAstronaut();

	glFlush();
}	

void genSpheres(GLfloat r, int deg, GLfloat startposX, GLfloat startposY, GLfloat startposZ, GLfloat arr[]){
	int sides = 10;
	
	for (int i = 0; i <= sides; i++) {
	    GLfloat vdeg = i * PI / sides;
	    GLfloat vsin = sin(vdeg);
	    GLfloat vcos = cos(vdeg);
	
	    for (int j = 0; j <= sides; j++) {
	      	GLfloat hdeg = j * 2 * PI / sides;
	      	GLfloat hsin = sin(hdeg);
	      	GLfloat hcos = cos(hdeg);
	
	      	GLfloat x = vsin * hcos;
	      	GLfloat y = vcos;
	      	GLfloat z = vsin * hsin;
	      	
	      	arr[vertices_count] = r * x + startposX;
	      	arr[vertices_count + 1] = r * y + startposY;
		    arr[vertices_count + 2] = r * z + startposZ;	
	        normals[normals_count] = x;
	        normals[normals_count + 1] = y;
	        normals[normals_count + 2] = z;
	
	      	x = sin(vdeg + (PI / sides)) * hcos;
	      	y = cos(vdeg + (PI / sides));
	      	z = sin(vdeg + (PI / sides)) * hsin;
	      
	      	arr[vertices_count + 3] = r * x + startposX;
	      	arr[vertices_count + 4] = r * y + startposY;
		    arr[vertices_count + 5] = r * z + startposZ;
	        normals[normals_count + 3] = x;
	        normals[normals_count + 4] = y;
	        normals[normals_count + 5] = z;
			
			normals_count += 6;
			vertices_count += 6;
		}
	}
}

void genColors(float r, float g, float b, float r_bias, float g_bias, float b_bias){
	r /= 255.0;
	g /= 255.0;
	b /= 255.0;
	int colors_counted = colors_count;
	GLfloat rand_r = r * ((rand()%1001/3000.0) + 0.7);
	GLfloat rand_g = g * ((rand()%1001/3000.0) + 0.7);
	GLfloat rand_b = b * ((rand()%1001/3000.0) + 0.7);
	
	
	for(int i = colors_count; i < colors_counted + 726; i+=9){
		rand_r = r * ((rand()%1001/3000.0) + 0.7 + r_bias);
		rand_g = g * ((rand()%1001/3000.0) + 0.7 + g_bias);
		rand_b = b * ((rand()%1001/3000.0) + 0.7 + b_bias);
		
		colors[i] = rand_r;
		colors[i+1] = rand_g;
		colors[i+2] = rand_b;
		colors[i+3] = rand_r;
		colors[i+4] = rand_g;
		colors[i+5] = rand_b;
		colors[i+6] = rand_r;
		colors[i+7] = rand_g;
		colors[i+8] = rand_b;
			
		colors_count += 9;
	}
}

void genStars(){
	for(int i = 0; i < 12000; i+=3){
		int randX = rand();
		int randY = rand();
		
		stars[i] = (randX%2 == 0)? -((randX%1001)/400.0):(randX%1001)/400.0;
		stars[i+1] = (randY%2 == 0)? -((randY%1001)/750.0):(randY%1001)/750.0;
		stars[i+2] = -1;
		
		stars[12000+i] = (randX%2 == 0)? -((randX%1001)/250.0):(randX%1001)/250.0;
		stars[12000+i+1] = 1.3;
		stars[12000+i+2] = (randY%2 == 0)? -((randY%1001)/1000.0):(randY%1001)/1000.0;
		
		
		stars[24000+i] = (randX%2 == 0)? -((randX%1001)/250.0):(randX%1001)/250.0;
		stars[24000+i+1] = -1.3;
		stars[24000+i+2] = (randY%2 == 0)? -((randY%1001)/1000.0):(randY%1001)/1000.0;
	}
}

void scrollControls(int type, int dir, int x, int y){
	if(dir == 1 && scale_val <= 2.3) scale_val += 0.02f;
	else if(dir == -1 && scale_val >= 1) scale_val -= 0.02f;
}

void keyControls(unsigned char key, int x, int y){
	GLfloat tempX = camX;
	GLfloat tempY = camY;
	GLfloat tempZ = camZ;
	GLfloat angle = 2*PI/180;
	GLfloat SIN = sin(angle);
	GLfloat COS = cos(angle);
	
	switch(key){
		case 'w':
			if(camZ > 0.5 || camY <= 0){
				angle = -2*PI/180;
				SIN = sin(angle);
				COS = cos(angle);
				camY = ((tempY * COS) - (tempZ*SIN));
				camZ = ((tempY * SIN) + (tempZ*COS));
			}
			
			isStarted = true;
		break;
		case 's':
			if(camZ > 0.5 || camY >= 0){
				camY = ((tempY * COS) - (tempZ * SIN));
				camZ = ((tempY * SIN) + (tempZ * COS));
			}
			
			isStarted = true;
		break;
		case 'a':
			YEAR_MS = YEAR_MS > 1000? YEAR_MS * 0.9: YEAR_MS;
			DAY_MS = YEAR_MS/365.0;
			cos_merc = cos(FRAME_MS/(0.5 * 88 * DAY_MS));
			sin_merc = sin(FRAME_MS/(0.5 * 88 * DAY_MS));
			cos_venus = cos(FRAME_MS/(0.5 * 225 * DAY_MS));
			sin_venus = sin(FRAME_MS/(0.5 * 225 * DAY_MS));
			cos_earth = cos(FRAME_MS/(0.5 * 365 * DAY_MS));
			sin_earth = sin(FRAME_MS/(0.5 * 365 * DAY_MS));
			cos_moon = cos(FRAME_MS/(0.5 * 27.3 * DAY_MS));
			sin_moon = sin(FRAME_MS/(0.5 * 27.3 * DAY_MS));
			
			isStarted = true;
		break;
		case 'd':
			YEAR_MS *= 1.1;
			DAY_MS = YEAR_MS/365.0;
			cos_merc = cos(FRAME_MS/(0.5 * 88 * DAY_MS));
			sin_merc = sin(FRAME_MS/(0.5 * 88 * DAY_MS));
			cos_venus = cos(FRAME_MS/(0.5 * 225 * DAY_MS));
			sin_venus = sin(FRAME_MS/(0.5 * 225 * DAY_MS));
			cos_earth = cos(FRAME_MS/(0.5 * 365 * DAY_MS));
			sin_earth = sin(FRAME_MS/(0.5 * 365 * DAY_MS));
			cos_moon = cos(FRAME_MS/(0.5 * 27.3 * DAY_MS));
			sin_moon = sin(FRAME_MS/(0.5 * 27.3 * DAY_MS));
		break;
		case 27:
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &VBOid);
			exit(0);
	}
	
	glutPostRedisplay();
}

void mouseControls(int button, int state, int x, int y){
	if(button == 0 && state == 0) mouseRightHeld = true;
	else if(button == 2 && state == 0) mouseLeftHeld = true;
	else{
		mouseRightHeld = false;
		mouseLeftHeld = false;
	}
}

void animate(int value){
	mappedVertices = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	tempX_earthCore = x_earthCore;
	tempZ_earthCore = z_earthCore;
	x_earthCore = (cos_earth * tempX_earthCore) + (sin_earth * tempZ_earthCore);
	z_earthCore = (cos_earth * tempZ_earthCore) - (sin_earth * tempX_earthCore);
	
	for(int i = 726; i < 1452; i+=3){
		tempX_merc = mappedVertices[i];
		tempZ_merc = mappedVertices[i+2]; 
		tempX_venus = mappedVertices[726+i];
		tempZ_venus = mappedVertices[726+i+2];
		tempX_earth = mappedVertices[1452+i];
		tempZ_earth = mappedVertices[1452+i+2];
		tempX_moon = mappedVertices[2178+i];
		tempZ_moon = mappedVertices[2178+i+2];
		
		x_diff = tempX_moon - x_earthCore;
		z_diff = tempZ_moon - z_earthCore;
		
		mappedVertices[i] = ((cos_merc * tempX_merc) + (sin_merc * tempZ_merc));
		mappedVertices[i+2] = (cos_merc * tempZ_merc) - (sin_merc * tempX_merc);
		mappedVertices[726+i] = ((cos_venus * tempX_venus) + (sin_venus * tempZ_venus));
		mappedVertices[726+i+2] = (cos_venus * tempZ_venus) - (sin_venus * tempX_venus);
		mappedVertices[1452+i] = ((cos_earth * tempX_earth) + (sin_earth * tempZ_earth));
		mappedVertices[1452+i+2] = (cos_earth * tempZ_earth) - (sin_earth * tempX_earth);
		mappedVertices[2178+i] = ((cos_moon * x_diff) + (sin_moon * z_diff)) + x_earthCore;
		mappedVertices[2178+i+2] = ((cos_moon * z_diff) - (sin_moon * x_diff)) + z_earthCore;
	}
	
	for(int i = 10164; i < 10890; i+=3){
		tempX_merc = mappedVertices[i];
		tempZ_merc = mappedVertices[i+2]; 
		tempX_venus = mappedVertices[726+i];
		tempZ_venus = mappedVertices[726+i+2];
		tempX_earth = mappedVertices[1452+i];
		tempZ_earth = mappedVertices[1452+i+2];
		tempX_moon = mappedVertices[2178+i];
		tempZ_moon = mappedVertices[2178+i+2];
		
		x_diff = tempX_moon - x_earthCore;
		z_diff = tempZ_moon - z_earthCore;
		
		mappedVertices[i] = ((cos_merc * tempX_merc) + (sin_merc * tempZ_merc));
		mappedVertices[i+2] = (cos_merc * tempZ_merc) - (sin_merc * tempX_merc);
		mappedVertices[726+i] = ((cos_venus * tempX_venus) + (sin_venus * tempZ_venus));
		mappedVertices[726+i+2] = (cos_venus * tempZ_venus) - (sin_venus * tempX_venus);
		mappedVertices[1452+i] = ((cos_earth * tempX_earth) + (sin_earth * tempZ_earth));
		mappedVertices[1452+i+2] = (cos_earth * tempZ_earth) - (sin_earth * tempX_earth);
		mappedVertices[2178+i] = ((cos_moon * x_diff) + (sin_moon * z_diff)) + x_earthCore;
		mappedVertices[2178+i+2] = ((cos_moon * z_diff) - (sin_moon * x_diff)) + z_earthCore;
	}
	
//This will orbit astro around the z axis of the sun
//	translateX_val = ((cos(astronautD * PI / 180.0)*astronautX) - (sin(astronautD * PI / 180.0)*astronautY));
//	translateY_val = ((sin(astronautD * PI / 180.0)*astronautX) + (cos(astronautD * PI / 180.0)*astronautY)) - astronautY;
//	astronautD = (int)(astronautD + 1)%360;

//This will move him around the scene like a dvd logo
	translateX_inc = (translateX_val > 1.5 || translateX_val < -1.5)? translateX_inc * -1: translateX_inc;
	translateX_val += translateX_inc;
	translateY_inc = (translateY_val > 0.5 || translateY_val < -0.5)? translateY_inc * -1: translateY_inc;
	translateY_val += translateY_inc;
	translateZ_inc = (translateZ_val > 1|| translateZ_val < -1)? translateZ_inc * -1: translateZ_inc;
	translateZ_val += translateZ_inc;
	
	if(mouseLeftHeld) rotate_val += 2;
	else if(mouseRightHeld) rotate_val -= 2;
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glutPostRedisplay();
	glutTimerFunc(FRAME_MS, animate, value+1);
}

void drawText(){
	glDisable(GL_LIGHTING);
	if(!isStarted){
		const unsigned char msg[] = "Hold a/d: controls revolution speed\n\nPress w/s: move camera up/down\n\nHold left/right click: rotate astro up/down\n\nScroll up/down: scale astro up/down";
		glColor3f(1, 1, 1);
		glRasterPos3f(-1.5, 0.7, 0);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, msg);
	}
	glEnable(GL_LIGHTING);
}

void drawSpheres() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)((3630 + 2178)*sizeof(GLfloat)));
	glNormalPointer(GL_FLOAT, 0, (GLvoid*)((3630 + 2178 + 3630)*sizeof(GLfloat)));
	
	glDisable(GL_LIGHTING);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 242);
	glEnable(GL_LIGHTING);

    GLfloat merc_diffuse[] = {0.43, 0.48, 0.54, 1.0}; 
    GLfloat merc_ambient[] = {0.043, 0.048, 0.054, 1.0}; 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, merc_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, merc_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 242, 242);
	
    GLfloat venus_diffuse[] = {0.6, 0.33, 0, 1.0}; 
    GLfloat venus_ambient[] = {0.06, 0.033, 0, 1.0}; 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, venus_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, venus_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 484, 242);
	
    GLfloat earth_diffuse[] = {0, 0.23, 0.36, 1.0}; 
    GLfloat earth_ambient[] = {0, 0.023, 0.036, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 726, 242);
	
    GLfloat moon_diffuse[] = {0.5, 0.5, 0.5, 1.0}; 
    GLfloat moon_ambient[] = {0.05, 0.05, 0.05, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, moon_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 968, 242);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawStars(){
	glDisable(GL_LIGHTING);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBegin(GL_POINTS);
	for(int i = 0; i < 36000; i+=3){
		glColor4f(1, 1, 1, (rand()%101/400.0)+0.1);
		glVertex3f(stars[i], stars[i+1], stars[i+2]);
	};
	glEnd();
	glEnable(GL_LIGHTING);
}

void draw_solid_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B){
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluQuadricDrawStyle(quadratic, GLU_FILL);
    gluQuadricNormals(quadratic, GLU_SMOOTH);

    glColor3ub(R, G, B);
    gluCylinder(quadratic, radius, radius, height, 32, 32);

    // Draw the circle on top of the cylinder
    glPushMatrix();
    glTranslatef(0.0, 0.0, height);
    gluDisk(quadratic, 0.0, radius, 32, 32);
    glPopMatrix();

    gluDeleteQuadric(quadratic);
}

void drawAstronaut(){
    GLfloat astronaut_specular[] = {1, 1, 0.5, 1};
    glMaterialfv(GL_FRONT, GL_SPECULAR, astronaut_specular);
    
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
    GLfloat orange_diffuse[] = {1, 0.5, 0.0, 1.0};
    GLfloat orange_ambient[] = {0.1, 0.05, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, orange_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, orange_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 1210, 242);
	
    GLfloat white_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat white_ambient[] = {0.10, 0.10, 0.10, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 1452, 242);
	
	glPushMatrix();
    GLfloat black_diffuse[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat black_ambient[] = {0.001, 0.001, 0.001, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glDrawArrays(GL_TRIANGLE_STRIP, 1694, 242);
	glPopMatrix();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

		
	    GLfloat white_diffuse1[] = {1.0, 1.0, 1.0, 1.0};
	    GLfloat white_ambient1[] = {0.10, 0.10, 0.10, 1.0};
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse1);
	    glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient1);
	    
    // body
	    glPushMatrix();
	    glTranslatef(-1.5 *astro_scale, -1.1 *astro_scale, 0);
	    glRotatef(-90, 1.0, 0.0, 0.0);
	    draw_solid_cylinder(0.33 *astro_scale, 0.66 *astro_scale, 255, 255, 255);
	    glPopMatrix();
    
    
    // left leg 
	    glPushMatrix();
	    glTranslatef(-1.72 *astro_scale, -1.35 *astro_scale, 0);
	    glRotatef(-90, 1.0, 0.0, 0.0);
	    draw_solid_cylinder(0.11 *astro_scale, 0.3 *astro_scale, 255, 255, 255);
	    glPopMatrix();
    
    // right leg
	    glPushMatrix();
	    glTranslatef(-1.29 *astro_scale, -1.35 *astro_scale, 0);
	    glRotatef(-90, 1.0, 0.0, 0.0);
	    draw_solid_cylinder(0.11 *astro_scale, 0.3 *astro_scale, 255, 255, 255);
	    glPopMatrix();
	    
	// right arm
	    glPushMatrix();
	    glTranslatef(-0.9 *astro_scale, -0.5 *astro_scale, 0);
	    glRotatef(-90, 0.0, 1.0, 0.0);
	    draw_solid_cylinder(0.11 *astro_scale, 0.4 *astro_scale, 255, 255, 255);
	    glPopMatrix();
	    
	// left arm
	    glPushMatrix();
	    glTranslatef(-1.8 *astro_scale, -0.5 *astro_scale, 0);
	    glRotatef(-90, 0.4, 1.0, 0.0);
	    draw_solid_cylinder(0.11 *astro_scale, 0.4 *astro_scale, 255, 255, 255);
	    glPopMatrix();
	
	
    GLfloat planets_specular[] = {0, 0, 0, 0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, planets_specular);
}
