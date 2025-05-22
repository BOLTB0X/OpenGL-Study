#include <GL/freeglut.h>

void draw(void);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(800, 200);
	glutCreateWindow("Draw Teapot");

	glutDisplayFunc(draw);

	glutMainLoop();
	return 0;
}

void draw(void)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0f, 0.5f, 0.3f, 1.0f);

	glutWireTeapot(0.5);

	glFlush();
}