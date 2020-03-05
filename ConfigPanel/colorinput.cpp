#include "colorinput.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QColorDialog>

using namespace CustomWidget;

ColorInput::ColorInput(WSFoundation::ConfigV1::ColorInput *color_in, QWidget *parent)
    : QWidget(parent), color_in(color_in)
{
    auto layout = new QHBoxLayout(this);
    layout->setMargin(2);

    auto x = new QLabel(color_in->configTitle(), this);
    x->setFrameStyle(QFrame::Sunken|QFrame::Box);
    layout->addWidget(x);


    layout->addWidget(new QLabel("模式：", this));
    combobox = new QComboBox(this);
    layout->addWidget(combobox);

    auto t1 = new QLabel("R:", this);
    layout->addWidget(t1);
    i1 = new QSpinBox(this);
    i1->setRange(0, 255);
    layout->addWidget(i1);

    auto t2 = new QLabel("G:", this);
    layout->addWidget(t2);
    i2 = new QSpinBox(this);
    i2->setRange(0, 255);
    layout->addWidget(i2);

    auto t3 = new QLabel("B:", this);
    layout->addWidget(t3);
    i3 = new QSpinBox(this);
    i3->setRange(0, 255);
    layout->addWidget(i3);

    auto t4 = new QLabel("A:", this);
    layout->addWidget(t4);
    i4 = new QSpinBox(this);
    i4->setRange(0, 255);
    layout->addWidget(i4);

    auto t5 = new QLabel("disable:", this);
    layout->addWidget(t5);
    i5 = new QSpinBox(this);
    i5->setRange(0, 255);
    layout->addWidget(i5);

    connect(combobox,   QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int i){
        auto mode = static_cast<WSFoundation::ConfigV1::ColorInput::Mode>(combobox->itemData(i).toInt());
        color_in->resetMode(mode);

        switch (mode) {
            case WSFoundation::ConfigV1::ColorInput::RGB:{
                    t1->setText("R:");
                    t2->setText("G:");
                    t3->setText("B:");
                    t4->setText("A:");
                    t5->setText("disblae:");

                    auto ints = color_in->value3();
                    i1->setValue(std::get<0>(ints));
                    i2->setValue(std::get<1>(ints));
                    i3->setValue(std::get<2>(ints));
                    i4->setValue(std::get<3>(ints));
                    i5->setValue(255);
                }break;
            case WSFoundation::ConfigV1::ColorInput::HSL:{
                    t1->setText("H:");
                    t2->setText("S:");
                    t3->setText("L:");
                    t4->setText("A:");
                    t5->setText("disblae:");

                    auto ints = color_in->value3();
                    i1->setValue(std::get<0>(ints));
                    i2->setValue(std::get<1>(ints));
                    i3->setValue(std::get<2>(ints));
                    i4->setValue(std::get<3>(ints));
                    i5->setValue(255);
                }break;
            case WSFoundation::ConfigV1::ColorInput::HSV:{
                    t1->setText("H:");
                    t2->setText("S:");
                    t3->setText("V:");
                    t4->setText("A:");
                    t5->setText("disblae:");

                    auto ints = color_in->value3();
                    i1->setValue(std::get<0>(ints));
                    i2->setValue(std::get<1>(ints));
                    i3->setValue(std::get<2>(ints));
                    i4->setValue(std::get<3>(ints));
                    i5->setValue(255);
                }break;
            case WSFoundation::ConfigV1::ColorInput::CMYK:
                t1->setText("C:");
                t2->setText("M:");
                t3->setText("Y:");
                t4->setText("K:");
                t5->setText("A:");
                auto ints2 = color_in->value4();
                i1->setValue(std::get<0>(ints2));
                i2->setValue(std::get<1>(ints2));
                i3->setValue(std::get<2>(ints2));
                i4->setValue(std::get<3>(ints2));
                i5->setValue(std::get<4>(ints2));
                break;
        }

    });

    connect(i1, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorInput::value_change);
    connect(i2, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorInput::value_change);
    connect(i3, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorInput::value_change);
    connect(i4, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorInput::value_change);
    connect(i5, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorInput::value_change);

    auto btn = new QPushButton("选择颜色", this);
    layout->addWidget(btn);

    connect(btn,    &QPushButton::clicked,  [=](bool ste){
        auto color = QColorDialog::getColor(Qt::white, this, color_in->configTitle());
        if(!color.isValid())
            return ;

        auto e = static_cast<WSFoundation::ConfigV1::ColorInput::Mode>(combobox->currentData().toInt());
        int v1=0,v2=0,v3=0,v4=255,v5=255;
        switch (e) {
            case WSFoundation::ConfigV1::ColorInput::RGB:
                color.getRgb(&v1,&v2,&v3,&v4);
                break;
            case WSFoundation::ConfigV1::ColorInput::HSL:
                color.getHsl(&v1,&v2,&v3,&v4);
                break;
            case WSFoundation::ConfigV1::ColorInput::HSV:
                color.getHsv(&v1,&v2,&v3,&v4);
                break;
            case WSFoundation::ConfigV1::ColorInput::CMYK:
                color.getCmyk(&v1,&v2,&v3,&v4,&v5);
                break;
        }

        i1->setValue(v1);
        i2->setValue(v2);
        i3->setValue(v3);
        i4->setValue(v4);
        i5->setValue(v5);
    });



    combobox->addItem("RGB", WSFoundation::ConfigV1::ColorInput::RGB);
    combobox->addItem("HSL", WSFoundation::ConfigV1::ColorInput::HSL);
    combobox->addItem("HSV", WSFoundation::ConfigV1::ColorInput::HSV);
    combobox->addItem("CMYK", WSFoundation::ConfigV1::ColorInput::CMYK);
}




void ColorInput::value_change(int)
{
    auto v1 = i1->value();
    auto v2 = i2->value();
    auto v3 = i3->value();
    auto v4 = i4->value();
    auto v5 = i5->value();

    QColor c(255,255,255);
    auto e = static_cast<WSFoundation::ConfigV1::ColorInput::Mode>(combobox->currentData().toInt());

    switch (e) {
        case WSFoundation::ConfigV1::ColorInput::RGB:
            c.setRgb(v1,v2,v3,v4);
            color_in->resetValue3(v1,v2,v3,v4);
            break;
        case WSFoundation::ConfigV1::ColorInput::HSL:
            c.setHsl(v1,v2,v3,v4);
            color_in->resetValue3(v1,v2,v3,v4);
            break;
        case WSFoundation::ConfigV1::ColorInput::HSV:
            c.setHsv(v1,v2,v3,v4);
            color_in->resetValue3(v1,v2,v3,v4);
            break;
        case WSFoundation::ConfigV1::ColorInput::CMYK:
            c.setCmyk(v1,v2,v3,v4,v5);
            color_in->resetValue4(v1,v2,v3,v4,v5);
            break;
    }

}
