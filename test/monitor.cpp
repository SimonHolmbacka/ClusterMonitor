/***************************************************************************
 *   Copyright (C) 2006-2008 by Tomasz Ziobrowski                          *
 *   http://www.3electrons.com                                             *
 *   e-mail: t.ziobrowski@3electrons.com                                   *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

    #include "monitor.h"
    #include "init.h"
    #include <QFont>
    #include <QPushButton>
    #include <QSlider>
    #include <QString>
    #include <QTimer>
    #include <QFile>
    #include <QTextStream>
    #include <pthread.h>
    #include <sys/socket.h>
    #include <netinet/in.h>


    #include "manometer.h"
    #include "utils.h"
    #include "udp/udpserver.h"

    #include "widgettester.h"

    #include <iostream>
    #include <fstream>
    #include <list>
    #include <vector>

    #define GSIZE 140
    #define GHEIGHT 180

    int freqs[4];
    int cores[16];
    int loads[4];
    double g1members[GSIZE];
    double g2members[GSIZE];
    double g3members[GSIZE];
    double g4members[GSIZE];
    AbstractMeter * meter[4];
    QGraphicsScene * scene1;
    QGraphicsScene * scene2;
    QGraphicsScene * scene3;
    QGraphicsScene * scene4;
    double p1 = 0;
    double p2 = 0;
    double p3 = 0;
    double p4 = 0;

using namespace std;

void* udpserver(void* v)
{
  int sockfd,n;
  struct sockaddr_in servaddr,cliaddr;
  socklen_t len;
  char mesg[1000];
  char header;
  char temp[8];

  sockfd=socket(AF_INET,SOCK_DGRAM,0);

  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(32000);
  bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

  for (;;)
  {
     len = sizeof(cliaddr);
     n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
     sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
     mesg[n] = 0;
     //cout << mesg << endl;

    int i = 0;
    int b = 0;
    int j = 0;
    int ftemp;
    int ltemp;
    char cid;
    char cid2[2];
    int id = 0;
    while(mesg[i] != '\0'){
      header = mesg[i];
      i++;
      switch (header){
        case 'F':                 //Select Frequency
          cid = mesg[i];          //Get board id
          i++;
          id = cid-'0';           //Set id as integer
          for(int clean=0;clean<8;clean++) temp[clean] = '\0';
          while((mesg[i] != ',')&&(mesg[i] != '\0')){   //Read the characters until we hit ','
            temp[b] = mesg[i];
            b++;
            i++;
          }
          i++;
          ftemp = atoi(temp);
          //cout << "freq is " << ftemp << endl;
          for(j=0;j<4;j++)                              //Update the variables
            freqs[id-1] = ftemp;
          b = 0;
          break;


        case 'C':
          cid2[0] = '\0';
          cid2[1] = '\0';
          cid2[0] = mesg[i];
          cid2[1] = mesg[i+1];
          id = atoi(cid2);           //Set id as integer

          if(id >= 10)
            i += 2;
          else
            i += 1;

          if(mesg[i]=='a')  cores[id] = 1;
          else              cores[id] = 0;
          break;


        case 'L':
          cid = mesg[i];           //Get board id
          i++;
          id = cid-'0';
          for(int clean=0;clean<8;clean++) temp[clean] = '\0';
          while((mesg[i] != ',')&&(mesg[i] != '\0')){
            temp[b] = mesg[i];
            b++;
            i++;
          }
          i++;
          ltemp = atoi(temp);
          //cout << "load is " << ltemp << endl;
          for(j=0;j<4;j++)
            loads[id-1] = ltemp;
          b = 0;
          break;
      }
     }
  }
}


// ----------------------------------------------------------------------------------------



    char* readFileString(char* loc)
    {
      ifstream	file;
      int		size;
      char*	inBuf;

      file.open(loc,ios_base::binary);
      if (file.is_open())
      {
        file.seekg(0,ios::end);		//get file size
        size = file.tellg();
        file.seekg(0,ios::beg);

        inBuf = new char[size];

        file.read(inBuf,size);
        file.close();
      }
      else
        cout << "shite" << endl;
      return inBuf;
    }


    TestWidget::TestWidget(QMainWindow *parent)
        : QMainWindow(parent)
    {
        initialize ();
    }

    void TestWidget::initialize()
    {
      setupUi(this);
      setWindowFlags(Qt::Window);
      setGeometry(50,50,400, 320);

      QWidget * widget = stackedWidget->widget(0);
      QWidget * widget2 = stackedWidget->widget(0);
      QWidget * widget3 = stackedWidget->widget(0);
      QWidget * widget4 = stackedWidget->widget(0);
      // Layout of stack 1 widget - manometer

      QFont* barfont = new QFont();
      barfont->setPixelSize(25);

      widget = stackedWidget->widget(0);
      bar = new ManoMeter(widget);
      bar->setValueFont(font()); // bar->digitFont());
      bar->setFont(*barfont);
      bar->resize(200,200);
      bar->setMaximum(1600);
      bar->setCritical(1400);
      bar->setValue(1200);
      bar->setMinimum(200);
      bar->setNominal(200);
      bar->setSuffix(" MHz");
      meter[0] = bar;
      QPoint p;
      p.setX(15);
      p.setY(30);
      bar->move(p);

      widget2 = stackedWidget->widget(0);
      bar2 = new ManoMeter(widget2);
      bar2->setValueFont(font()); // bar->digitFont());
      bar2->setFont(*barfont);
      bar2->resize(200,200);
      bar2->setMaximum(1600);
      bar2->setCritical(1400);
      bar2->setValue(1200);
      bar2->setMinimum(200);
      bar2->setNominal(200);
      bar2->setSuffix(" MHz");
      p.setX(263);
      p.setY(30);
      bar2->move(p);
      meter[1] = bar2;

      widget3 = stackedWidget->widget(0);
      bar3 = new ManoMeter(widget3);
      bar3->setValueFont(font()); // bar->digitFont());
      bar3->setFont(*barfont);
      bar3->resize(200,200);
      bar3->setMaximum(1600);
      bar3->setCritical(1400);
      bar3->setValue(1200);
      bar3->setMinimum(200);
      bar3->setNominal(200);
      bar3->setSuffix(" MHz");
      p.setX(260+253);
      p.setY(30);
      bar3->move(p);
      meter[2] = bar3;

      widget4 = stackedWidget->widget(0);
      bar4 = new ManoMeter(widget4);
      bar4->setValueFont(font()); // bar->digitFont());
      bar4->setFont(*barfont);
      bar4->resize(200,200);
      bar4->setMaximum(1600);
      bar4->setCritical(1400);
      bar4->setValue(1200);
      bar4->setMinimum(200);
      bar4->setNominal(200);
      bar4->setSuffix(" MHz");
      p.setX(265+257+250);
      p.setY(30);
      bar4->move(p);
      meter[3] = bar4;

      this->core0->setStyleSheet("background-color: green");
      this->core1->setStyleSheet("background-color: green");
      this->core2->setStyleSheet("background-color: green");
      this->core3->setStyleSheet("background-color: green");

      this->core4->setStyleSheet("background-color: green");
      this->core5->setStyleSheet("background-color: green");
      this->core6->setStyleSheet("background-color: green");
      this->core7->setStyleSheet("background-color: green");
      this->core8->setStyleSheet("background-color: green");
      this->core9->setStyleSheet("background-color: green");
      this->core10->setStyleSheet("background-color: green");
      this->core11->setStyleSheet("background-color: green");
      this->core12->setStyleSheet("background-color: green");
      this->core13->setStyleSheet("background-color: green");
      this->core14->setStyleSheet("background-color: green");
      this->core15->setStyleSheet("background-color: green");
      this->progressBar->setStyleSheet(QString("QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 white, stop: 1 lightblue);}")+QString("QProgressBar::horizontal {border: 1px solid gray; border-radius: 3px; background: white; padding: 0px; text-align: left; margin-right: 4ex;}"));
      this->progressBar_2->setStyleSheet(QString("QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 white, stop: 1 lightblue);}")+QString("QProgressBar::horizontal {border: 1px solid gray; border-radius: 3px; background: white; padding: 0px; text-align: left; margin-right: 4ex;}"));
      this->progressBar_4->setStyleSheet(QString("QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 white, stop: 1 lightblue);}")+QString("QProgressBar::horizontal {border: 1px solid gray; border-radius: 3px; background: white; padding: 0px; text-align: left; margin-right: 4ex;}"));
      this->progressBar_5->setStyleSheet(QString("QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 white, stop: 1 lightblue);}")+QString("QProgressBar::horizontal {border: 1px solid gray; border-radius: 3px; background: white; padding: 0px; text-align: left; margin-right: 4ex;}"));
      //total load:
      this->progressBar_3->setStyleSheet(QString("QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 white, stop: 1 #ff4444);}")+QString("QProgressBar::horizontal {border: 1px solid gray; border-radius: 3px; background: white; padding: 0px; text-align: left; margin-right: 4ex;}"));
      this->progressBar_3->setRange(0, 400);
      this->progressBar_3->setMaximum(400);

      this->lcdNumber_5->setSegmentStyle(QLCDNumber::Flat);
      this->lcdNumber_6->setSegmentStyle(QLCDNumber::Flat);
      this->lcdNumber_7->setSegmentStyle(QLCDNumber::Flat);
      this->lcdNumber_8->setSegmentStyle(QLCDNumber::Flat);
      this->lcdNumber_10->setSegmentStyle(QLCDNumber::Flat);


      this->label_12->setStyleSheet("font: 16pt;");
      this->label_29->setStyleSheet("font: 16pt;");
      this->label_30->setStyleSheet("font: 16pt;");
      this->label_31->setStyleSheet("font: 16pt;");
      this->label_32->setStyleSheet("font: 9pt;");
      this->label_35->setStyleSheet("font: 9pt;");
      this->label_36->setStyleSheet("font: 9pt;");
      this->label_37->setStyleSheet("font: 9pt;");

      for(int clean=0;clean<GSIZE;clean++)
      {
        g1members[clean]=0;
        g2members[clean]=0;
        g3members[clean]=0;
        g4members[clean]=0;
      }

      scene1 = new QGraphicsScene(0, 0, GSIZE, GHEIGHT);
      this->graphicsView->setScene(scene1);
      scene2 = new QGraphicsScene(0, 0, GSIZE, GHEIGHT);
      this->graphicsView_2->setScene(scene2);
      scene3 = new QGraphicsScene(0, 0, GSIZE, GHEIGHT);
      this->graphicsView_3->setScene(scene3);
      scene4 = new QGraphicsScene(0, 0, GSIZE, GHEIGHT);
      this->graphicsView_4->setScene(scene4);

      pthread_t t1;

      pthread_create(&t1, NULL, &udpserver, NULL);

      QTimer *timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(TCallBack()));
      timer->start(300);
    }

    void TestWidget::closeEvent ( QCloseEvent * /* event */)
    {
      qApp->quit();
    }

    void TestWidget::make_freqs()
    {
      set_freqs(freqs,meter);
    }

    void TestWidget::make_cores()
    {

    cores[0] ? this->core0->setStyleSheet("background-color: #7dff5f") : this->core0->setStyleSheet("background-color: #ff4444");
    cores[1] ? this->core1->setStyleSheet("background-color: #7dff5f") : this->core1->setStyleSheet("background-color: #ff4444");
    cores[2] ? this->core2->setStyleSheet("background-color: #7dff5f") : this->core2->setStyleSheet("background-color: #ff4444");
    cores[3] ? this->core3->setStyleSheet("background-color: #7dff5f") : this->core3->setStyleSheet("background-color: #ff4444");
    cores[4] ? this->core4->setStyleSheet("background-color: #7dff5f") : this->core4->setStyleSheet("background-color: #ff4444");
    cores[5] ? this->core5->setStyleSheet("background-color: #7dff5f") : this->core5->setStyleSheet("background-color: #ff4444");
    cores[6] ? this->core6->setStyleSheet("background-color: #7dff5f") : this->core6->setStyleSheet("background-color: #ff4444");
    cores[7] ? this->core7->setStyleSheet("background-color: #7dff5f") : this->core7->setStyleSheet("background-color: #ff4444");
    cores[8] ? this->core8->setStyleSheet("background-color: #7dff5f") : this->core8->setStyleSheet("background-color: #ff4444");
    cores[9] ? this->core9->setStyleSheet("background-color: #7dff5f") : this->core9->setStyleSheet("background-color: #ff4444");
    cores[10] ? this->core10->setStyleSheet("background-color: #7dff5f") : this->core10->setStyleSheet("background-color: #ff4444");
    cores[11] ? this->core11->setStyleSheet("background-color: #7dff5f") : this->core11->setStyleSheet("background-color: #ff4444");
    cores[12] ? this->core12->setStyleSheet("background-color: #7dff5f") : this->core12->setStyleSheet("background-color: #ff4444");
    cores[13] ? this->core13->setStyleSheet("background-color: #7dff5f") : this->core13->setStyleSheet("background-color: #ff4444");
    cores[14] ? this->core14->setStyleSheet("background-color: #7dff5f") : this->core14->setStyleSheet("background-color: #ff4444");
    cores[15] ? this->core15->setStyleSheet("background-color: #7dff5f") : this->core15->setStyleSheet("background-color: #ff4444");
    }

    void TestWidget::make_load()
    {
      this->progressBar->setValue(loads[0]);
      this->progressBar_2->setValue(loads[1]);
      this->progressBar_4->setValue(loads[2]);
      this->progressBar_5->setValue(loads[3]);

      this->progressBar_3->setValue(loads[0]+loads[1]+loads[2]+loads[3]);

    }

    void TestWidget::make_graph()
    {
      char * buf = 0;
      double zoom = 17.0;

      buf = readFileString("measurements/power1.txt");
      p1 = (double)atof(buf);
      buf = readFileString("measurements/power2.txt");
      p2 = (double)atof(buf);
      buf = readFileString("measurements/power3.txt");
      p3 = (double)atof(buf);
      buf = readFileString("measurements/power4.txt");
      p4 = (double)atof(buf);


      for(int i=0; i<GSIZE; i++){
        if(i<GSIZE-1)
          g1members[i] = g1members[i+1];  //shift all members to the left
          g2members[i] = g2members[i+1];  //shift all members to the left
          g3members[i] = g3members[i+1];  //shift all members to the left
          g4members[i] = g4members[i+1];  //shift all members to the left
      }
      if(p1>0.0)
        g1members[GSIZE-1] = p1;            //add new value to the rightmost position
      else
        g1members[GSIZE-1] = g1members[GSIZE-2];
      if(p2>0.0)
        g2members[GSIZE-1] = p2;            //add new value to the rightmost position
      else
        g2members[GSIZE-1] = g2members[GSIZE-2];
      if(p3>0.0)
        g3members[GSIZE-1] = p3;            //add new value to the rightmost position
      else
        g3members[GSIZE-1] = g3members[GSIZE-2];
      if(p4>0.0)
        g4members[GSIZE-1] = p4;            //add new value to the rightmost position
      else
        g4members[GSIZE-1] = g4members[GSIZE-2];

      scene1->clear();                    //Clear graph
      scene2->clear();                    //Clear graph
      scene3->clear();                    //Clear graph
      scene4->clear();                    //Clear graph
      QPen pen(Qt::black, 2);
      for(int i=0; i<GSIZE-1; i++){
        scene1->addLine(i+1,GHEIGHT-g1members[i]*zoom,i+2,GHEIGHT-g1members[i+1]*zoom,pen);
        scene2->addLine(i+1,GHEIGHT-g2members[i]*zoom,i+2,GHEIGHT-g2members[i+1]*zoom,pen);
        scene3->addLine(i+1,GHEIGHT-g3members[i]*zoom,i+2,GHEIGHT-g3members[i+1]*zoom,pen);
        scene4->addLine(i+1,GHEIGHT-g4members[i]*zoom,i+2,GHEIGHT-g4members[i+1]*zoom,pen);
      }
      this->lcdNumber_5->display(p1);
      this->lcdNumber_6->display(p2);
      this->lcdNumber_7->display(p3);
      this->lcdNumber_8->display(p4);
      this->lcdNumber_10->display(p1+p2+p3+p4);
    }

    void TestWidget::TCallBack()
    {
      this->make_freqs();
      this->make_cores();
      this->make_load();
      this->make_graph();
    }
