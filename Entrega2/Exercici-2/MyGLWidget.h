#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void mousePressEvent (QMouseEvent *e);
    virtual void mouseReleaseEvent( QMouseEvent *e);

  private:
  
    int printOglError(const char file[], int line, const char func[]);
    void iniEscena();
    void iniCamera1();
    void iniCamera2();
    void TerraTransform();
    void RoadTransform(glm::vec3 pos, float angle);
    void PipeTransform();
    void CarTransform (float radi, float angle);
    void carregaShaders();
    void viewTransform();
    void projectTransform();
    void mouseMoveEvent(QMouseEvent *e);
    void viewTransformCamCotxe2();
    void reset();

    //uniform colors cotxe
    GLuint cfcotxeLoc;
    float d;
    float alpha, psi, theta, phi;
    float rav;
    float desf = asin(7.5/7.5);

    bool camaraCotxe;
    glm::vec3 auxvrp = glm::vec3(7.5, 0.5, 0.0);
    glm::vec3 auxobs = glm::vec3(7.5, 0.5, 0.5);


    struct infoCamaraCotxe {
      glm::vec3 obs;
      glm::vec3 vrp;
      glm::vec3 up;
      float fov;
      float znear;
      float zfar;

    };
    infoCamaraCotxe CamCotxe;

};
