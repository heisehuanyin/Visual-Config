#ifndef WIDGET_H
#define WIDGET_H

#include "../ConfigFoundation/configv1.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class Widget : public QScrollArea
{
    Q_OBJECT

public:
    Widget(const QString content=QString(),QWidget *parent = nullptr);
    ~Widget();


    QString getContent() const ;

private:
    QWidget *base_panel;
    QVBoxLayout *base_layout;
    WSFoundation::ConfigV1::Config *config_xml;
};

#endif // WIDGET_H
