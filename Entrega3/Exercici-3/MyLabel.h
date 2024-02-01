#include <QLabel>

class MyLabel: public QLabel {
	Q_OBJECT

	public:
		MyLabel(QWidget *parent);

        //vector de colors
        QVector<QColor> cv = {
            QColor(255,0,0), 
            QColor(0,255,0),
            QColor(0,0,255),
            QColor(255,255,0),
            QColor(128,128,128),
        };
        /// MaxRondes serveix per establir el número de rondes de cada partida
        int MaxRondes = 5;
        int NumRonda = 0;
        int puntsp = 0;

        int ijoc = 0;
        int ijug = 0;
        bool start = false;
        bool inSeq = false;

        QVector<QColor> seqjoc;
        QVector<QColor> seqjug;

	public slots:
        void asigna_color();
	    void startSeq();
        void mostraSeq();
        void vermB();
        void verdB();
        void blauB();
        void grogB();
        void Procesa(int i);
        void InfoAtent();
        void InfoTorn();
        void InfoPunts();
        void InfoGuanyat();
        void InfoPerdut();
        void InfoPreparat();
        void ResetGame();
        void CleanPunts();
        void CleanInfo();
        void WarningInfo();
        void WarningInfo2();
        void MostraInfo();
    private:
        QTimer *timer;
    signals:
        void atent();
        void torn();
        void punts();
        void guanyat();
        void perdut();
        void reset();
        void cleanp();
        void cleaninfo();
        void warning();
        void warning2();
        void showinfo();
};