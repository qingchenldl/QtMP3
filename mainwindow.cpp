#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    process=new QProcess(this);
    timer=new QTimer(this);

    allnum=nownum=0;
    issx=true;
    ui->sx->setStyleSheet(PINK);
    ui->xh->setStyleSheet(WHITE);

    init();

    //ui->medialist->setViewMode(QListView::IconMode);
    //sui->medialist->setFlow(QListView::LeftToRight);
    //ui->medialist->setFlow(QListView::LeftToRight);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    connect(this->process,SIGNAL(readyReadStandardOutput()),this,SLOT(playerMsg()));
    getIndex();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playVideo(QString file_name)
{
    process->kill();
    process->waitForFinished();
    timer->stop();
    QStringList args;
    args << "-slave";        //使用slave模式
    args << "-quiet";        //不要输出冗余信息
    //args << "-wid" << QString::number(ui->widget->winId());        //将输出定位到ui下的widget窗口内
    //args << "-zoom";        //自适应窗口
    //args << "-vo";
    //args << "x11";        //使用x11模式播放(只有这种模式下才支持23行的指定输出窗口到ui->widget)
    args << file_name;        //播放file_name文件
    process->start("mplayer", args);        //启动该进程，并传入参数args
    timer->start(200);
    process->waitForStarted();
    //process->write("pause\n");
    //process->waitForFinished();
    process->write("get_time_length\n");
    qDebug()<<args<<endl;
}

void MainWindow::init()
{
    ui->alltime->setText("00:00");
    ui->curtime->setText("00:00");
    ui->horizontalSlider->setValue(0);
    media << ".rmvb"<< ".wav"\
          <<".mp4"  << ".mpg" \
         << ".mpeg"<< ".mp3";

    QStringList args;
    args<<"/dev/sda";
    args<<"/mnt/usb/";
    process->start("mount",args);

    qDebug()<<args<<endl;

    process->waitForFinished();

    args.clear();
    //args<<"/lib";
    args<<"/mnt/usb/";
    process->start("ls",args);
    qDebug()<<args<<endl;

    process->waitForFinished();
    getList();

}

void MainWindow::playerMsg()
{
    while(process->canReadLine())
    {
        QString str=process->readLine();
        if(str.contains("ANS_LENGTH="))
        {
            QString cnt=str.mid(11);
            double tt=cnt.toDouble();
            int m,s,t=tt;
            ui->horizontalSlider->setMaximum(t);
            m=t/60;
            s=t%60;
            QString ms=QString::number(m);
            QString ss=QString::number(s);
            if(ms.length()==1)
            {
                ms="0"+ms;
            }
            if(ss.length()==1)
            {
                ss="0"+ms;
            }
            QString tmp=ms+":"+ss;
            qDebug()<<t<<endl;
            ui->alltime->setText(tmp);
        }
        else if(str.contains("ANS_TIME_POSITION="))
        {
            QString cnt=str.mid(18);
            double tt=cnt.toDouble();
            int m,s,t=tt;
            //ui->horizontalSlider->setMaximum(t);
            m=t/60;
            s=t%60;
            QString ms=QString::number(m);
            QString ss=QString::number(s);
            if(ms.length()==1)
            {
                ms="0"+ms;
            }
            if(ss.length()==1)
            {
                ss="0"+ss;
            }
            QString tmp=ms+":"+ss;
            //qDebug()<<t<<" "<<tt<<" "<<tmp<<" "<<ms<<" "<<ss<<" "<<m<<" "<<s<<endl;
            ui->curtime->setText(tmp);
            ui->horizontalSlider->setValue((t));
            //ui->horizontalSlider->set;
        }
        if(ui->curtime->text()==ui->alltime->text()&&ui->alltime->text()!="00:00")
        {
            ui->alltime->setText("00:00");
            ui->curtime->setText("00:00");
            ui->horizontalSlider->setValue(0);
            timer->stop();
            goNext();
        }
        //qDebug()<<str<<endl;
    }
}

void MainWindow::getList()
{
    QStringList strs;
    while(process->canReadLine())
    {
        QString str=process->readLine();
        str.chop(1);
        for(int i=0; i<media.length(); i++)
        {
            if(str.toLower().contains(media.at(i)))
            {
                //qDebug()<<str<<" "<<media.at(i);
                strs<<str;
                break;
            }
        }
    }
    allnum=strs.size();
    //qDebug()<<"allnum is "<<allnum<<endl;
    ui->medialist->addItems(strs);
    ui->medialist->setCurrentRow(0);
}

void MainWindow::on_control_clicked()
{
    process->kill();
    process->waitForFinished();
    QString str=ui->medialist->currentItem()->text();
    nownum=ui->medialist->currentIndex().row();
    qDebug()<<"******************************************************************** "<<nownum<<endl;
    ui->alltime->setText("00:00");
    ui->curtime->setText("00:00");
    ui->horizontalSlider->setValue(0);
    playVideo("/mnt/usb/"+str);
    //playVideo(str);

    //process->write("get_time_pos\n");
}

void MainWindow::on_stop_clicked()
{
    process->kill();
    process->waitForFinished();
    ui->alltime->setText("00:00");
    ui->curtime->setText("00:00");
    ui->horizontalSlider->setValue(0);
    //ui->widget->hide();
    //ui->widget->show();
}

void MainWindow::updateTime()
{
    process->write("get_time_pos\n");
}

void MainWindow::on_forward_clicked()
{
    process->write("pausing_keep seek +5 0\n");
}


void MainWindow::on_back_clicked()
{
    process->write("pausing_keep seek -5 0\n");
}

void MainWindow::on_up_clicked()
{
    process->write("volume 10\n");
}

void MainWindow::on_down_clicked()
{
    process->write("volume -10\n");
}


void MainWindow::on_xh_clicked()
{
    issx=false;
    ui->xh->setStyleSheet(PINK);
    ui->sx->setStyleSheet(WHITE);
}

void MainWindow::on_sx_clicked()
{
    issx=true;
    ui->sx->setStyleSheet(PINK);
    ui->xh->setStyleSheet(WHITE);
}

void MainWindow::getIndex()
{
    QString str=ui->medialist->item(nownum)->text();
    ui->medialist->setCurrentRow(nownum);
    playVideo("/mnt/usb/"+str);
}

void MainWindow::goNext()
{
    nownum++;
    qDebug()<<allnum<<" "<<nownum<<endl;
    if(allnum==nownum&&nownum!=0)
    {
        if(!issx)
        {
            nownum=0;
            getIndex();
        }
        else
        {
            nownum=0;
        }
    }
    else
    {
        qDebug()<<"nownum is "<<nownum<<endl;
        getIndex();
    }
}

void MainWindow::on_pause_play_clicked()
{
    process->write("pause\n");
    qDebug()<<"pause   ===="<<endl;
    if(ui->pause_play->text()=="play")
    {
        timer->start(200);
        ui->pause_play->setText("pause");
    }
    else
    {
        timer->stop();
        ui->pause_play->setText("play");
    }
}
