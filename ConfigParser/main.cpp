#include <QCoreApplication>
#include <QtDebug>
#include <QFile>
#include "configv1.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    using namespace WSFoundation::ConfigV1;

    QFile f("/Users/wangshuai/Project/Cpp/build-ConfigLoad-Desktop_Qt_5_12_4_clang_64bit-Debug/ConfigGen/something.xml");
    if(!f.open(QIODevice::Text|QIODevice::ReadOnly)){
        exit(-1);
    }
    QTextStream tin(&f);

    QString error;
    auto xbase = Config::genNewInstance(tin.readAll(), &error);
    xbase->initChildren();

    for (int i=0; i<xbase->itemsCount(); ++i) {
        qDebug() << xbase->getItem(i)->configTitle();
    }

    return a.exec();
}
