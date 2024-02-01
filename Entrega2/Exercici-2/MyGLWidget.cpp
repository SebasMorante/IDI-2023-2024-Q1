#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <cmath>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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

MyGLWidget::~MyGLWidget(){
}
const glm::vec3 veccfcotxes[4] = {
  glm::vec3(0.0, 0.0, 1.0),
  glm::vec3(0.0, 1.0, 0.0),
  glm::vec3(1.0, 0.0, 0.0),
  glm::vec3(1.0, 1.0, 1.0),
};

const float vecposcotxes[3] = {6, 7.5, 9};
float angCotxes[3] = {0.0, 0.0, 0.0};


void MyGLWidget::initializeGL(){
  LL2GLWidget::initializeGL();
  camaraCotxe = false;
  iniEscena();
  if (camaraCotxe) iniCamera2();
  else iniCamera1();

}


void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      angCotxes[0] += 2.0f;
      angCotxes[1] += 3.0f;
      angCotxes[2] += 4.0f;

      viewTransformCamCotxe2();

      break;
    }
    case Qt::Key_C: { 
      if (camaraCotxe){
        camaraCotxe = false;
        iniCamera1();
        
        
        if (rav > 1){
          glm::mat4 Proj(1.0f);
          Proj = glm::perspective (fov, rav, znear, zfar);
          glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
        }
      }
      else{
        camaraCotxe = true;
        iniCamera2();
        //añdido
        projectTransform();
      }
      break;
        }
    case Qt::Key_R: {
      reset(); 
      break;
        }  
    case Qt::Key_T: { 
      break;
        }          
    default: event->ignore(); break;
  }
  update();
}
void MyGLWidget::iniEscena(){
    radiEscena = sqrt(800)/2;
    d = 2*radiEscena;
}

void MyGLWidget::iniCamera1(){

  obs = glm::vec3(0, 2, 5);
  vrp = glm::vec3(0, 1, 0);
  up = glm::vec3(0, 1, 0);
  alpha = asin(float(radiEscena/d));
  fov = 2*alpha;
  znear = radiEscena;
  zfar  = 3*radiEscena;
  ra = 1.0f;

  psi = 0;
  theta = float(M_PI)/4.0f;
  phi = 0;

  projectTransform();
  viewTransform();
}

void MyGLWidget::iniCamera2(){
  //CamCotxe.obs = glm::vec3(7.5, 0.5, 0.5);
  //CamCotxe.vrp = glm::vec3(7.5, 0.5, 0.0);
  CamCotxe.obs = auxobs;
  CamCotxe.vrp = auxvrp;
  CamCotxe.up = glm::vec3(0, 1, 0);
  CamCotxe.fov = glm::radians(60.0f);
  CamCotxe.znear =  0.25;
  CamCotxe.zfar  = 40;

  obs = CamCotxe.obs;
  vrp = CamCotxe.vrp;
  up = CamCotxe.up;
  fov = CamCotxe.fov;
  znear = CamCotxe.znear;
  zfar = CamCotxe.zfar;
  ra = float(ample)/float(alt);

  LL2GLWidget::viewTransform();
}

void MyGLWidget::viewTransformCamCotxe2(){
  
  /////////////elim
  //auxobs = auxvrp;
  ////////////elimm

  std::cout << (angCotxes[1]-desf) << std::endl;
  auxobs.x = 7.5*cos(glm::radians(angCotxes[1]-3.85));
  auxobs.y = 0.5;
  auxobs.z = -(7.5*sin(glm::radians(angCotxes[1]-3.85)));

  auxvrp.x = 7.5*cos(glm::radians(angCotxes[1]));
  auxvrp.y = 0.5;
  auxvrp.z = -(7.5*sin(glm::radians(angCotxes[1])));

  /*
  auxobs.x = auxvrp.x + 0.5*cos(glm::radians(angCotxes[1]));
  auxobs.y = 0.5;
  auxobs.z = auxvrp.z + 0.5*sin(glm::radians(angCotxes[1]));
  */




  if (camaraCotxe){
    obs = auxobs;
    vrp = auxvrp;
    glm::mat4 View(1.0f);
    View = glm::lookAt (obs, vrp, up);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }

  std::cout << "vrp: " << auxvrp.x << " " << auxvrp.z << std::endl;
  std::cout << "obs: " << auxobs.x << " " << auxobs.z << std::endl;
  std::cout << "x: " <<  auxobs.x-auxvrp.x << ", y: " << auxvrp.z-auxobs.z << std::endl;
  std::cout << "h: " << glm::sqrt((auxobs.x-auxvrp.x)*(auxobs.x-auxvrp.x)+(auxvrp.z-auxobs.z)*(auxvrp.z-auxobs.z)) << std::endl;
}

void MyGLWidget::mousePressEvent (QMouseEvent *e){
  makeCurrent();
  LL2GLWidget::mousePressEvent(e);
  update();
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *e){
  makeCurrent();
  LL2GLWidget::mouseReleaseEvent(e);
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  if (not camaraCotxe){
    makeCurrent();

    psi += (e->x() - xClick)*factorAngleY;
    theta += (e->y() - yClick)*factorAngleX;

    viewTransform();

    xClick = e->x();
    yClick = e->y();

  update();
  }
}

void MyGLWidget::reset(){
  angCotxes[0] = 0;
  angCotxes[1] = 0;
  angCotxes[2] = 0;

  psi = 0;
  theta = float(M_PI)/4.0f;
  phi = 0;
  viewTransform();
  auxvrp = glm::vec3(7.5, 0.5, 0.0);
  auxobs = glm::vec3(7.5, 0.5, 0.5);

  if (camaraCotxe){
    camaraCotxe = false;
    iniCamera1();


    if (rav > 1){
      glm::mat4 Proj(1.0f);
      Proj = glm::perspective (fov, rav, znear, zfar);
      glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
      }
    }
}

void MyGLWidget::resizeGL(int w, int h){
  LL2GLWidget::resizeGL(w, h);
  rav = float(w)/float(h);
  projectTransform();
}

void MyGLWidget::projectTransform(){
  glm::mat4 Proj(1.0f);

  if (not camaraCotxe){
    if (rav < 1) fov = 2*glm::atan(glm::tan(alpha)/rav);
    else fov = 2*alpha;
  }

  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}



void MyGLWidget::viewTransform(){
  glm::mat4 View(1.0f);
  View = glm::translate(View, glm::vec3(0.0, 0.0, -d));
  View = glm::rotate(View, -phi, glm::vec3(0.0, 0.0, 1.0));
  View = glm::rotate(View, theta, glm::vec3(1.0, 0.0, 0.0));
  View = glm::rotate(View, -psi, glm::vec3(0.0, 1.0, 0.0));
  View = glm::translate(View, -vrp);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}



void MyGLWidget::TerraTransform(){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0.0));
  TG = glm::scale(TG, glm::vec3(2.5));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::RoadTransform (glm::vec3 pos, float angle){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[ROAD]*5));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]*3));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::CarTransform (float radi, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::rotate(TG, glm::radians(0.0f), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL(){
    // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Road
  glBindVertexArray (VAO_models[ROAD]);

  int n = 5;
  float ang = 0.0f;
  for (int i = 0; i < 4; ++i){
    int x = (i == 0 or i == 3) ? n : -n;
    int z = (i < 2) ? -n : n;

    RoadTransform(glm::vec3(x, 0.01, z), ang);
    glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
    ang += 90;
  }

  // Car
  glBindVertexArray (VAO_models[CAR]);
  for (int i = 0; i < 3; ++i){
    glm::vec3 aux = veccfcotxes[i];
    glUniform3fv(cfcotxeLoc, 1, &aux[0]);
    CarTransform(vecposcotxes[i], angCotxes[i]);
    glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
  }
  glm::vec3 aux = veccfcotxes[3];
  glUniform3fv(cfcotxeLoc, 1, &aux[0]);


  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  PipeTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders(){
  LL2GLWidget::carregaShaders();
  cfcotxeLoc = glGetUniformLocation(program->programId(), "cfcotxe");
}

