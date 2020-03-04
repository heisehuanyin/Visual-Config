#include "widget.h"
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QtDebug>
#include <QFileDialog>

class MainWindow : public QMainWindow
{
public:
    MainWindow()
        :QMainWindow(),
          config_fname(""),
          widget_center(nullptr)
    {
        auto mbar = new QMenuBar();
        setMenuBar(mbar);

        auto fm = mbar->addMenu("File");
        fm->addAction("Open", this, &MainWindow::openConfig);
        fm->addAction("Save", this, &MainWindow::saveConfig);
    }

private:
    QString config_fname;
    Widget *widget_center;

    void saveConfig(){
        QFile f(config_fname);
        f.open(QIODevice::Text|QIODevice::WriteOnly);
        QTextStream out(&f);

        out << widget_center->getContent();
        out.flush();
        f.close();
    }

    void openConfig(){
        auto fname = QFileDialog::getOpenFileName(this);
        if(fname.length()){
            if(widget_center){
                delete widget_center;
                widget_center = nullptr;
            }

            config_fname = fname;
            this->setWindowTitle(config_fname);

            QFile f(fname);
            f.open(QIODevice::ReadOnly|QIODevice::Text);
            QTextStream intext(&f);

            widget_center = new Widget(intext.readAll(), this);
            this->setCentralWidget(widget_center);
            f.close();
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
