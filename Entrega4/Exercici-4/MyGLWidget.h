// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void iniEscena();
  private:
    int printOglError(const char file[], int line, const char func[]);

    void modelTransformTerra();
    void paintGL();
    void modelTransformTrack(float radi, float rotY);
    void iniCamera();
    void viewTransform();
    void modelTransformMineCart();
    void iniMaterialTerra();
    void iniFocus();
    void modelTransformFantasma();
    void modelTransformFocusM1();
    void modelTransformFocusM2();
    void move();
    void moveMF();

    GLuint colFocusLocC, posFocusLocC, colFocusLocM, posFocusLocM1, posFocusLocM2, transFocusLocM1, transFocusLocM2, boolFocusEscenaLoc;
    glm::vec4 posFocusC; 
    glm::vec3 colFocusC, colFocusM, posFocusM1, posFocusM2;
    
    bool Escena2 = false;

    float angleX;
    float angCarro = 0.0f;
    float sesc = 1.0f;
    
    QTimer *timer;
    bool timerStart = false;

};
