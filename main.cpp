/* Universidade Federal da Fronteira Sul - UFFS
 * Curso de Ciência da Computação
 * Disciplina de Computação Gráfica - 2016/02
 * Professor	José Carlos Bins Filho
 * Acadêmicos 	Edimar Roque Martello Júnior - 1111100038 - Noturno
 *				Edirlan José Censi - 1211100030 - Matutino
*/

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

float angx, angy;

GLfloat ctrlpoints[4][4][3] = {
	{ { -1.5, 1.0, -1.5 },{ -0.5, 1.0,-1.5 },{ 0.5, 1.0, -1.5 },{ 1.5, 1.0,-1.5 } },
	{ { -1.5, 1.0, -0.5 },{ -0.5, 2.0,-0.5 },{ 0.5, 2.0, -0.5 },{ 1.5, 1.0,-0.5 } },
	{ { -1.5, 1.0,  0.5 },{ -0.5, 2.0, 0.5 },{ 0.5, 2.0,  0.5 },{ 1.5, 1.0, 0.5 } },
	{ { -1.5, 1.0,  1.5 },{ -0.5, 1.0, 1.5 },{ 0.5, 1.0,  1.5 },{ 1.5, 1.0, 1.5 } }
};
GLfloat cor[3] = {1.0,0.5,0.0};


void nurbsMesh(GLfloat cp[4][4][3], GLint un, GLint vn, GLfloat cor[3], bool mesh = false) {
	int u, v;

	glColor3fv(cor);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 12, 4, 0, 1, 3, 4, &cp[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glBegin(GL_QUADS);
	for (u = 0; u < un; u++) {
		for (v = 0; v < vn; v++) {
			glEvalCoord2f((GLfloat)v / vn, (GLfloat)(u+1) / un);
			glEvalCoord2f((GLfloat)(v+1) / vn, (GLfloat)(u+1) / un);
			glEvalCoord2f((GLfloat)(v+1) / vn, (GLfloat)u / un);
			glEvalCoord2f((GLfloat)v / vn, (GLfloat)u / un);
		}
	}
	glEnd();
	if (mesh) {
		  glColor3f(1,1,1);
			for (u = 0; u < un; u++) {
				for (v = 0; v < vn; v++) {
					glBegin(GL_LINE_STRIP);
					glEvalCoord2f((GLfloat)v / vn, (GLfloat)u / un);
					glEvalCoord2f((GLfloat)(v+1) / vn, (GLfloat)u / un);
					glEvalCoord2f((GLfloat)(v+1) / vn, (GLfloat)(u+1) / un);
					glEvalCoord2f((GLfloat)v / vn, (GLfloat)(u+1) / un);
					glEvalCoord2f((GLfloat)v / vn, (GLfloat)u / un);
					glEnd();
				}
			}
	}
}
void display(void)
{
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Especifica posi��o do observador e do alvo
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	if (angx != 0 ) {
		glRotated(angx, 1, 0, 0);
	}
	if (angy != 0) {
		glRotated(angy, 0, 1, 0);
	}

	nurbsMesh(ctrlpoints,30,30,cor);
	glPopMatrix();
	glFlush();
}

void inicLuz(void)
{
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.5,0.5,0.5,1.0 };	   // "cor"
	GLfloat luzEspecular[4] = { 0.7,0.7,0.7, 1.0 };// "brilho"
	GLfloat posicaoLuz[4] = { 10.0, 10.0, 0, 1.0 };
//
	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 0.5,0.5,0.5,1.0 };
	GLint especMaterial = 2;

	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os par�metros da luz de n�mero 0
	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz);

	// Habilita a defini��o da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de ilumina��o
	glEnable(GL_LIGHTING);
	// Habilita a luz de n�mero 0
	glEnable(GL_LIGHT1);
	}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	inicLuz();
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button ==  GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) { angx +=15;
		}
	if (button ==  GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) { angy +=15;
			}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Especifica a proje��o perspectiva
	gluPerspective(45, (GLfloat)w/h, 3, 500);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Trabalho 2");
	init();
	glutMouseFunc(GerenciaMouse);
    glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
