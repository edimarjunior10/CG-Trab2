/*
  UFFS - Universidade Federal da Fronteira Sul
  Curso de Ciência da Computação
  Disciplina de Computação Gráfica
  7ª Fase - Noturno
  Professor: José Carlos Bins Filho
  Acadêmicos: Edimar R. Martello Júnior e Andrey B. Ramos
*/
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>

#define numPontoSuperficie    4
#define ordemSuperficie        4   // cúbico, grau 3 + 1 
#define numNoSuperficie    (numPontoSuperficie + ordemSuperficie)
#define numCordenadasSuperficie    3

#define numPontosCurva 4
#define ordemCurva        4 
#define numNosCurva    (numPontosCurva + ordemCurva)
#define numCordenadasCurva    3


typedef GLfloat ponto;
typedef GLfloat Ponto[3];


// Sequencias de pontos para aplicação das curvas
ponto pontosSuperficie[numNoSuperficie] = {0., 0., 0., 0., 1., 1., 1., 1.};
ponto pontosOnda[numNosCurva] =  {0., 0., 0., 0., 1., 1., 1., 1.};


// Pontos de controle da bandeira para fazer tremular
Ponto pontosControle[numPontoSuperficie][numPontosCurva] = {
    { {0., 3., 0.}, {1., 3., 0.}, {2., 3., 0}, {3., 3., 0.}},
    { {0., 2., 0.}, {1., 2., 0.}, {2., 2., 0}, {3., 2., 0.}},
    { {0., 1., 0.}, {1., 1., 0.}, {2., 1., 0}, {3., 1., 0.}},
    { {0., 0., 0.}, {1., 0., 0.}, {2., 0., 0}, {3., 0., 0.}}
};


static GLUnurbsObj *bandeira;

//Função que desenha o mastro
void criaMastro()
{
	glPushMatrix();
	//Desenha o mastro:
    glTranslatef(-20,20,-80);
    glColor3f(0, 0, 0);
    glScaled(5,18,0);

    float raio=0.5;
    float maxY = 0.7f;
    float minY = -4.0f;

    float vetorPosicoes[] = { -raio, minY , -raio,  //0
                         -raio, minY , +raio,  //1
                         +raio, minY , +raio,  //2
                         +raio, minY , -raio,  //3
                         -raio, maxY , -raio,  //4
                         -raio, maxY , +raio,  //5
                         +raio, maxY , +raio,  //6
                         +raio, maxY , -raio   //7
                        };

    glBegin(GL_TRIANGLES);
    glNormal3f(0.0,1.0,0.0);

      //Embaixo
      glVertex3fv(&vetorPosicoes[0*3]);
      glVertex3fv(&vetorPosicoes[1*3]);
      glVertex3fv(&vetorPosicoes[2*3]);
      
      glVertex3fv(&vetorPosicoes[0*3]);
      glVertex3fv(&vetorPosicoes[2*3]);
      glVertex3fv(&vetorPosicoes[3*3]);

      //Topo
      glVertex3fv(&vetorPosicoes[4*3]);
      glVertex3fv(&vetorPosicoes[5*3]);
      glVertex3fv(&vetorPosicoes[6*3]);
      
      glVertex3fv(&vetorPosicoes[4*3]);
      glVertex3fv(&vetorPosicoes[6*3]);
      glVertex3fv(&vetorPosicoes[7*3]);


      glNormal3f(1.0,0.0,0.0);
      //Esquerda
      glVertex3fv(&vetorPosicoes[0*3]);
      glVertex3fv(&vetorPosicoes[1*3]);
      glVertex3fv(&vetorPosicoes[5*3]);
      
      glVertex3fv(&vetorPosicoes[0*3]);
      glVertex3fv(&vetorPosicoes[5*3]);
      glVertex3fv(&vetorPosicoes[4*3]);

      //Direita
      glVertex3fv(&vetorPosicoes[3*3]);
      glVertex3fv(&vetorPosicoes[2*3]);
      glVertex3fv(&vetorPosicoes[6*3]);
      
      glVertex3fv(&vetorPosicoes[3*3]);
      glVertex3fv(&vetorPosicoes[6*3]);
      glVertex3fv(&vetorPosicoes[7*3]);

      glNormal3f(0.0,0.0,1.0);
      //Frente
      glVertex3fv(&vetorPosicoes[1*3]);
      glVertex3fv(&vetorPosicoes[2*3]);
      glVertex3fv(&vetorPosicoes[5*3]);
      
      glVertex3fv(&vetorPosicoes[2*3]);
      glVertex3fv(&vetorPosicoes[5*3]);
      glVertex3fv(&vetorPosicoes[6*3]);

      //Costas
      glVertex3fv(&vetorPosicoes[0*3]);
      glVertex3fv(&vetorPosicoes[4*3]);
      glVertex3fv(&vetorPosicoes[3*3]);
      
      glVertex3fv(&vetorPosicoes[3*3]);
      glVertex3fv(&vetorPosicoes[4*3]);
      glVertex3fv(&vetorPosicoes[7*3]);

    glColor3f(0, 0, 0);
    glEnd();
    glPopMatrix();
}

//Função que desenha a bandeira
void desenhaBandeira() 
{	
	//Define as máscaras de cores utilizadas
	GLfloat vermelho[] = { 0.7, 0.0, 0.1, 1.0 };   
	GLfloat azul[] = { 0.0, 0.1, 0.7, 1.0 };
	GLfloat amarelo[] = { 0.7, 0.8, 0.1, 1.0 };   
  GLfloat cinza[] = { 0.4, 0.4, 0.4, 1.0 }; 

  static GLfloat angulo = 0.0;
  int i,j;


  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  criaMastro();

	//Efetua a curva da bandeira
    for (i=1; i<4; i++)
        for (j=0; j<4; j++)
            pontosControle[i][j][2] = sin((GLfloat)i+angulo);  
    angulo += 0.1;         

    //Desenha a primeira listra da bandeira, de cor amarela
    glPushMatrix();
      	
     	glTranslatef(18.5,25,-80);
        glScalef(12,2.3,3.5);
        glRotatef(90,0.,0.,1.);
        
	    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, amarelo);
	    gluBeginSurface(bandeira);
	            gluNurbsSurface(bandeira,numNoSuperficie, pontosSuperficie, numNosCurva, pontosOnda,
	                          3 * numPontosCurva, 3,
	                          &pontosControle[0][0][0], ordemCurva, ordemSuperficie, GL_MAP2_VERTEX_3);           
	    gluEndSurface(bandeira);  
		    
    glPopMatrix();
	        
 	//Desenha a primeira listra da bandeira, de cor azul
 	glPushMatrix();
      	
     	glTranslatef(18.5,17.5,-80);
        glScalef(12,2.5,3.5);
        glRotatef(90,0.,0.,1.);
        
     
	    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, azul);
	    gluBeginSurface(bandeira);
	            gluNurbsSurface(bandeira,numNoSuperficie, pontosSuperficie, numNosCurva, pontosOnda,
	                          3 * numPontosCurva, 3,
	                          &pontosControle[0][0][0], ordemCurva, ordemSuperficie, GL_MAP2_VERTEX_3);           
	    gluEndSurface(bandeira);  
		    
    glPopMatrix();
 
    //Desenha a primeira listra da bandeira, de cor vermelha
 	glPushMatrix();
      	
     	glTranslatef(18.5,10,-80);
        glScalef(12,2.5,3.5);
        glRotatef(90,0.,0.,1.);
        
     
	    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vermelho);
	    gluBeginSurface(bandeira);
	            gluNurbsSurface(bandeira,numNoSuperficie, pontosSuperficie, numNosCurva, pontosOnda,
	                          3 * numPontosCurva, 3,
	                          &pontosControle[0][0][0], ordemCurva, ordemSuperficie, GL_MAP2_VERTEX_3);           
	    gluEndSurface(bandeira);  
		    
    glPopMatrix();
 	//Define a máscara de cor do resto de cinza
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cinza);
}

//Função que desenha na tela
void display(void)
{
 	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	desenhaBandeira();
	criaMastro();	

  //Desenha a esfera
	glPushMatrix();
		glColor3f(  0, 0, 0 );
		glTranslated(-20, 35, -80);
		glRotated(80, 1.0, 1, 1);
		glutSolidSphere(3,20,20);
	glPopMatrix();
  glFlush();
}


//Função de redefinição de tamanho da janela
void reshape(int w, int h)
{
    const float mid = (float) w / (float) h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-mid, mid, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

//Função que controla o desenho na tela
static void controle(void)
{
    display();
    glutSwapBuffers();
}

//Função que inicializa o ambiente
void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    //Define o objeto NURBS, que é do tipo da bandeira
    bandeira = gluNewNurbsRenderer();
    gluNurbsProperty(bandeira, GLU_SAMPLING_TOLERANCE, 8);
    gluNurbsProperty(bandeira, GLU_DISPLAY_MODE, GLU_FILL);

}

int main(int argc, char *argv[])
{ 
  glutInit(&argc, argv);
  glutInitWindowSize(800,600);
  glutInitWindowPosition(10,10);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Trabalho 2 Bandeira da Colombia");    
	glutDisplayFunc(controle);   
  glutReshapeFunc(reshape);
  glutIdleFunc(controle);
	init();
	glutMainLoop();
}
