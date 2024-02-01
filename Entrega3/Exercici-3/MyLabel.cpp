#include "MyLabel.h"
#include <QTimer>
#include <QCoreApplication>
#include <QObject>
#include <QTime>
#include <QRandomGenerator>
#include <iostream>
MyLabel::MyLabel(QWidget *parent=0):QLabel(parent) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyLabel::mostraSeq);
    timer->setInterval(1000);
}

void MyLabel::asigna_color(){
    int randN = QRandomGenerator::global()->bounded(4);
    seqjoc.push_back(cv[randN]);
    seqjoc.push_back(cv[4]);
}

void MyLabel::startSeq(){
    if (not start){
        ResetGame();
        emit cleanp();
        emit cleaninfo();
        asigna_color();
        start = true;
        emit atent();
        mostraSeq();
    }
    else emit warning();
}

void MyLabel::CleanInfo(){
    setText("");
}

void MyLabel::WarningInfo(){
    QPalette palette2;
    palette2.setColor(QPalette::WindowText, qRgb(200,0,0));
    setPalette(palette2);
    setText("Ja estas en una partida!");
}

void MyLabel::WarningInfo2(){
    QPalette palette2;
    palette2.setColor(QPalette::WindowText, qRgb(200,0,0));
    setPalette(palette2);
    setText("Has d'esperar que acabi la seqüència!");
}

void MyLabel::MostraInfo(){
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::black);
    setPalette(palette);
    setText("Fes clic a ""Play"" per començar!");
}

void MyLabel::mostraSeq(){
    if (ijoc == 0){
        timer->start();
        inSeq = true;
    }
    QPalette palette;
    palette.setColor(QPalette::Window, seqjoc[ijoc]);
    setAutoFillBackground(true);
    setPalette(palette);
    ijoc += 1;
    if (ijoc == int(seqjoc.size())){
        timer->stop();
        ijoc = 0;
        inSeq = false;
        emit cleaninfo();
        emit torn();
    }
}

void MyLabel::Procesa(int i){
    if (start){
        if (not inSeq){
            emit cleaninfo();
            if (cv[i] != seqjoc[ijug]){
            emit perdut();
            emit showinfo();
            start = false;
            }
            else {
                emit punts();
                ijug += 2;
                if (ijug == int(seqjoc.size())){
                    ++NumRonda;
                    if (NumRonda == MaxRondes){
                        emit guanyat();
                        emit showinfo();
                        start = false;

                    }
                    else {
                        emit atent();
                        ijug = 0;
                        asigna_color();
                        mostraSeq();
                    }
                }
            }
        }
        else emit warning2();
    }

}

void MyLabel::vermB(){
    Procesa(0);
}

void MyLabel::verdB(){
    Procesa(1);
}

void MyLabel::blauB(){
    Procesa(2);
}

void MyLabel::grogB(){
    Procesa(3);
}

void MyLabel::InfoAtent(){
    setText("Atent !");
}

void MyLabel::InfoTorn(){
    setText("El teu torn!");
}

void MyLabel::InfoPreparat(){
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::black);
    setPalette(palette);
    setText("Preparat per començar!");
}

void MyLabel::InfoPunts(){
    puntsp += 1;
    QString s = QString::number(puntsp);
    setText(s);
}

void MyLabel::CleanPunts(){
    puntsp = 0;
    setText("0");
}

void MyLabel::InfoPerdut(){
    QPalette palette2;
    palette2.setColor(QPalette::WindowText, qRgb(200,0,0));
    setPalette(palette2);
    setText("Has perdut!");
    start = false;

}

void MyLabel::InfoGuanyat(){
    QPalette palette;
    palette.setColor(QPalette::WindowText, qRgb(0,200,0));
    setPalette(palette);
    setText("Has guanyat!");
}

void MyLabel::ResetGame(){
    start = false;
    ijoc = 0;
    ijug = 0;
    seqjoc.clear();
    seqjug.clear();
    NumRonda = 0;
    emit showinfo();
    emit reset();
}
