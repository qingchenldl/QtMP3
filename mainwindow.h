#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

private:
    void playVideo(QString);        //播放传入的文件

    void init();

    void getList();

    void goNext();

    void getIndex();

private slots:
    void on_forward_clicked();

    void on_control_clicked();

    void on_stop_clicked();

    void on_back_clicked();

    void on_up_clicked();

    void on_down_clicked();

    void playerMsg();

    void on_xh_clicked();

    void on_sx_clicked();

    void on_pause_play_clicked();

    void updateTime();

private:
    QProcess *process;
    QTimer *timer;
    QStringList media;

    bool issx;

    int allnum;
    int nownum;

};

const QString BLUE  = "QPushButton{background:blue}";
const QString GREEN = "QPushButton{background:green}";
const QString RED   = "QPushButton{background:red}";
const QString PINK  = "QPushButton{background:pink}";
const QString BROWN = "QPushButton{background:borwn}";
const QString YELLOW= "QPushButton{background:yellow}";
const QString WHITE = "QPushButton{background:white}";

#endif // MAINWINDOW_H
