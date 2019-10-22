#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

#include <GL/freeglut.h>

using namespace std;

int resolution = 500;
GLubyte* PixelBuffer = new GLubyte[resolution * resolution * 3];
double degree = -45.0f;

enum projection_type {CABINET, CAVALIER};
projection_type current_proj = CABINET;

void menuHandler(int item)
{
  switch (item)
  {
  case 0:
    /* CABINET */
    current_proj = CABINET;
    break;
  case 1:
    /* CAVALIER */
    current_proj = CAVALIER;
  default:
    break;
  }
  glutPostRedisplay();
}

void createMenu(void)
{
  glutCreateMenu(menuHandler);
  glutAddMenuEntry("Cabinet", 0);
  glutAddMenuEntry("Cavalier", 1);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void render(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // orthographic projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10.0, 10.0, -10.0, 10.0, 0.0, 20.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
  double projecMatrix[16];
  double half_circle = M_PI / 180.0f;
  double angle = half_circle * degree;

  glGetDoublev(GL_MODELVIEW_MATRIX, projecMatrix);
  projecMatrix[8] = -cos(angle);
  projecMatrix[9] = sin(angle);
  if(current_proj == CABINET)
  {
    projecMatrix[8] /= 2;
    projecMatrix[9] /= 2;
  }
  glMultMatrixd(projecMatrix);

  glTranslatef(-4.0, -8.0, 0.0);

  /* cube wireframes */
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0.0, 0.0, -9.0);
  glRotatef(0.0, 0.0, 0.0, 1.0);
  glutWireCube(3.1);
  glPopMatrix();
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0.0, 4.0, -9.0);
  glRotatef(0.0, 0.0, 0.0, 1.0);
  glutWireCube(3.1);
  glPopMatrix();
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0.0, 8.0, -9.0);
  glRotatef(0.0, 0.0, 0.0, 1.0);
  glutWireCube(3.1);
  glPopMatrix();

  /* cubes */
  glPushMatrix();
  glColor3f(0.34, 0.18, 0.49);
  glTranslatef(0.0, 0.0, -9.0);
  glutSolidCube(3.0);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0, 4.0, -9.0);
  glutSolidCube(3.0);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0, 8.0, -9.0);
  glutSolidCube(3.0);
  glPopMatrix();

  /* cylinder */
  glPushMatrix();
  glColor3f(0.27, 0.13, 0.07);
  glTranslatef(-3.0, -2.0, -5);
  glRotatef(90, -1.0, 0.0, 0.0);
  glutSolidCylinder(0.5, 2, 50, 50);
  glPopMatrix();

  /* cone */
  glPushMatrix();
  glColor3f(0.34, 0.49, 0.19);
  glTranslatef(-3.0, 0.5, -5);
  glRotatef(90, -1.0, 0.0, 0.0);
  glutSolidCone(2, 8, 50, 50);
  glPopMatrix();

  glutSwapBuffers(); 
}

int main(int argc, char *argv[])
{
  // inicia GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(resolution, resolution);
  glutCreateWindow("Trabalho CG");

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(render);
  createMenu();
  glutMainLoop();
  return 0;
}
