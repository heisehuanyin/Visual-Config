#include "configpanel.h"
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
          config_xml(nullptr),
          widget_center(nullptr)
    {
        auto mbar = new QMenuBar();
        setMenuBar(mbar);

        auto fm = mbar->addMenu("File");
        fm->addAction("Open", this, &MainWindow::openConfig);
        fm->addAction("Save", this, &MainWindow::saveConfig);
    }
    virtual ~MainWindow(){
        delete widget_center;
        delete config_xml;
    }

private:
    QString config_fname;
    WSFoundation::ConfigV1::Config *config_xml;
    WSFoundation::ConfigPanel *widget_center;

    void saveConfig(){
        QFile f(config_fname);
        f.open(QIODevice::Text|QIODevice::WriteOnly);
        QTextStream out(&f);

        out << config_xml->toString();
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
            QString error;
            config_xml = WSFoundation::ConfigV1::Config::genNewInstance(intext.readAll(),&error);
            if(error.length())
                qDebug() << error;

            widget_center = new WSFoundation::ConfigPanel(config_xml, this);
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
