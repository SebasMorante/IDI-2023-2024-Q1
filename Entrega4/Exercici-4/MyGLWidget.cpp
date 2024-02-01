// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QCoreApplication>
#include <QTimer>
#include <QObject> // Asegúrate de incluir el encabezado de QObject
#include <QDebug>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
    amb = glm::vec3(0.2, 0, 0.0);
    //Difusion
    diff = glm::vec3(0.4, 0, 0);
    //coeficiente de reflexion especular
    spec = glm::vec3(0.8, 0.8, 0.8);

    //Exponente reflexion especular -> a mas pequeño más definido el circulo de luz que incide
    shin = 100;
}

void MyGLWidget::initializeGL(){
  
  LL4GLWidget::initializeOpenGLFunctions();  

  if (not Escena2) glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  else glClearColor(0.0, 0.0, 0.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersTrack();
  creaBuffersFantasma();
  creaBuffersMineCart();
  creaBuffersTerra();

  posFocusLocC = glGetUniformLocation(program->programId(), "posFocusC");
  colFocusLocC = glGetUniformLocation(program->programId(), "colFocusC");

  posFocusLocM1 = glGetUniformLocation(program->programId(), "posFocusM1");
  posFocusLocM2 = glGetUniformLocation(program->programId(), "posFocusM2");
  colFocusLocM = glGetUniformLocation(program->programId(), "colFocusM");

  transFocusLocM1 = glGetUniformLocation(program->programId(), "TGFM1");
  transFocusLocM2 = glGetUniformLocation(program->programId(), "TGFM2");

  boolFocusEscenaLoc = glGetUniformLocation(program->programId(), "Escena2");

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MyGLWidget::move);
  timer->setInterval(50);

  iniEscena();
  iniCamera();

  iniFocus();
}


void MyGLWidget::iniFocus(){
  posFocusC = glm::vec4(0.0, 0.0, 0.0, 1.0);
  colFocusC = glm::vec3(0.8, 0.8, 0.8);
  glUniform4fv(posFocusLocC, 1, &posFocusC[0]);
  glUniform3fv(colFocusLocC, 1, &colFocusC[0]);

  //pos ini del carro -> 0,0,10
  posFocusM1 = glm::vec3(2.0, 1.75, 10.5);
  posFocusM2 = glm::vec3(2.0, 1.75, 9.5); 


  colFocusM = glm::vec3(0.6, 0.6, 0.0);
  glUniform3fv(posFocusLocM1, 1, &posFocusM1[0]);
  glUniform3fv(posFocusLocM2, 1, &posFocusM2[0]);
  glUniform3fv(colFocusLocM, 1, &colFocusM[0]);


  glm::mat4 TGFM1(1.0f);
  glUniformMatrix4fv(transFocusLocM1, 1, GL_FALSE, &TGFM1[0][0]);
  glm::mat4 TGFM2(1.0f);
  glUniformMatrix4fv(transFocusLocM2, 1, GL_FALSE, &TGFM2[0][0]);

  glUniform1i (boolFocusEscenaLoc, Escena2);
}

void MyGLWidget::resizeGL(int w, int h){
  LL4GLWidget::resizeGL(w, h);
}

void MyGLWidget::iniEscena(){
  centreEsc = glm::vec3(0,0.75,0);
  radiEsc = (sqrt(1800)/2);
}


void MyGLWidget::iniCamera(){

  angleY = 0.0;
  angleX = M_PI/8;
  ra = 1.0;

  LL4GLWidget::projectTransform();
  viewTransform();
}


void MyGLWidget::viewTransform(){

  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::modelTransformTerra () {

  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0.0));
  TG = glm::scale(TG, glm::vec3(3*sesc));
  TG = glm::translate(TG, glm::vec3(-5, 0, -5));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTrack(float radi, float rotY)
{

  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, float(glm::radians(rotY)), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::rotate(TG, glm::radians(90.0f), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaTrack*sesc, escalaTrack*sesc, escalaTrack*sesc));
  TG = glm::translate(TG, -centreBaseTrack);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMineCart ()
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, float(glm::radians(angCarro)), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(0,0,10));
  TG = glm::rotate(TG, float(glm::radians(90.0)), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaMineCart*sesc, escalaMineCart*sesc, escalaMineCart*sesc));
  TG = glm::translate(TG, -centreBaseMineCart);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformFantasma ()
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, float(glm::radians(angCarro)), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(0, 1, 10));
  TG = glm::rotate(TG, float(glm::radians(90.0)), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma*sesc, escalaFantasma*sesc, escalaFantasma*sesc));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFocusM1(){

  glm::mat4 TGFM1(1.0f);
  TGFM1 = glm::rotate(TGFM1, float(glm::radians(angCarro)), glm::vec3(0,1,0));
  //TGFM1 = glm::translate(TGFM1, glm::vec3(2.0, 1.75, 10.5));

  glUniformMatrix4fv(transFocusLocM1, 1, GL_FALSE, &TGFM1[0][0]);
}

void MyGLWidget::modelTransformFocusM2(){

  glm::mat4 TGFM2(1.0f);
  TGFM2 = glm::rotate(TGFM2, float(glm::radians(angCarro)), glm::vec3(0,1,0));
  //TGFM2 = glm::translate(TGFM2, glm::vec3(2.0, 1.75, 9.5));

  glUniformMatrix4fv(transFocusLocM2, 1, GL_FALSE, &TGFM2[0][0]);
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // TRACK
  glBindVertexArray (VAO_Track);

  float ang = 0.0f;
  for (int i = 0; i < 90; ++i){
    modelTransformTrack(10, ang);
    glDrawArrays(GL_TRIANGLES, 0, track.faces().size()*3);
    ang += (360/90);
  }
  
  // 
  if (Escena2){
    glBindVertexArray (VAO_Fantasma);
    modelTransformFantasma ();
    glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  }

  // MINE CART
  glBindVertexArray (VAO_MineCart);
  modelTransformMineCart ();
  glDrawArrays(GL_TRIANGLES, 0, mineCart.faces().size()*3);
  
  glBindVertexArray(0);

  modelTransformFocusM1();
  modelTransformFocusM2();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (yClick - e->y()) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      if (not timerStart) moveMF();
    break;
	}
  case Qt::Key_L: {
      if (Escena2){
        Escena2 = false;
        sesc = 1.0f;
        initializeGL();
      }
      else{
        Escena2 = true;
        sesc = 0.75f;
        initializeGL();
      }
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  case Qt::Key_S: {
    if (not timerStart){
      std::cout << "COMIENZA" << std::endl;
      timer->start();
      timerStart = true;
    }
    else {
      std::cout << "PARA TIMER" << std::endl;
      timer->stop();
      timerStart = false;
    }
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  //modelTransformFocusM1();
  //modelTransformFocusM2();
  update();
}

void MyGLWidget::moveMF(){
  angCarro += 5.0f;
  //modelTransformMineCart();
  //modelTransformFantasma();
}

void MyGLWidget::move(){
  moveMF();
  //modelTransformFocusM1();
  //modelTransformFocusM2();
  update();
}
