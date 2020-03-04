#include "fontconfig.h"

#include <QFontComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QtDebug>

using namespace CustomWidget;

FontConfig::FontConfig(WSFoundation::ConfigV1::FontConfig *config, QWidget *parent)
    : QWidget(parent), config(config)
{
    auto layout = new QHBoxLayout(this);
    layout->setMargin(2);

    auto x = new QLabel(config->configTitle(), this);
    x->setFrameStyle(QFrame::Sunken|QFrame::Box);
    layout->addWidget(x);

    auto f1 = new QLabel("字体:", this);
    layout->addWidget(f1);
    auto fontfamily = new QFontComboBox(this);
    layout->addWidget(fontfamily);

    connect(fontfamily, QOverload<const QFont&>::of(&QFontComboBox::currentFontChanged), [=](const QFont& f){
        config->resetFont(f.family());
    });


    auto f2 = new QLabel("大小:", this);
    layout->addWidget(f2);
    auto size = new QDoubleSpinBox(this);
    layout->addWidget(size);

    connect(size,   QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double v){
        config->resetFontSize(v);
    });


    auto btn_bold = new QPushButton("粗体", this);
    btn_bold->setCheckable(true);
    layout->addWidget(btn_bold);
    connect(btn_bold, QOverload<bool>::of(&QPushButton::clicked),   [=](bool s){
        config->setBold(s);
    });

    auto btn_italic = new QPushButton("斜体", this);
    btn_italic->setCheckable(true);
    layout->addWidget(btn_italic);
    connect(btn_italic, QOverload<bool>::of(&QPushButton::clicked), [=](bool s){
        config->setItalic(s);
    });

    auto btn_strikeout = new QPushButton("删除线", this);
    btn_strikeout->setCheckable(true);
    layout->addWidget(btn_strikeout);
    connect(btn_strikeout,  QOverload<bool>::of(&QPushButton::clicked), [=](bool s){
        config->setStrikeOut(s);
    });

    auto btn_underline = new QPushButton("下划线", this);
    btn_underline->setCheckable(true);
    layout->addWidget(btn_underline);
    connect(btn_underline,  QOverload<bool>::of(&QPushButton::clicked), [=](bool s){
        config->setUnderline(s);
    });

    bool flag = 0;
    for(int i=0; i<fontfamily->count(); ++i){
        if(fontfamily->itemText(i)==config->fontFamily()){
            fontfamily->setCurrentIndex(i);
            flag=1;
            break;
        }
    }
    if(!flag){
        fontfamily->setCurrentIndex(1);
        fontfamily->setCurrentIndex(0);
    }

    size->setValue(config->fontSize());
    btn_bold->setChecked(config->isBold());
    btn_italic->setChecked(config->isItalic());
    btn_strikeout->setChecked(config->isStrikeOut());
    btn_underline->setChecked(config->isUnderline());
}
