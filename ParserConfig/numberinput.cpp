#include "numberinput.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

using namespace CustomWidget;

NumberInput::NumberInput(WSFoundation::ConfigV1::NumberInput *num_input, QWidget *parent)
    :QWidget (parent), num_input(num_input),async_symbo(false)
{
    auto layout = new QHBoxLayout(this);
    layout->setMargin(2);

    auto label = new QLabel(num_input->configTitle(), this);
    label->setFrameStyle(QFrame::Sunken|QFrame::Box);
    layout->addWidget(label);

    if(num_input->type()==WSFoundation::ConfigV1::NumberInput::INTEGER){
        auto item = new QSpinBox(this);
        layout->addWidget(item);

        auto li = num_input->limits();
        item->setRange(li.first, li.second);
        item->setValue(num_input->intValue());

        auto slide = new QSlider(Qt::Horizontal, this);
        slide->setRange(li.first, li.second);
        layout->addWidget(slide);

        connect(item, QOverload<int>::of(&QSpinBox::valueChanged), [=](int v){
            if(async_symbo){
                async_symbo = false;
                return;
            }
            async_symbo = true;

            num_input->resetValue(v);
            slide->setValue(v);
        });
        connect(slide, QOverload<int>::of(&QSlider::valueChanged), [=](int v){
            if(async_symbo){
                async_symbo = false;
                return;
            }
            async_symbo = true;

            num_input->resetValue(v);
            item->setValue(v);
        });
    }
    else {
        auto item = new QDoubleSpinBox(this);
        layout->addWidget(item);

        auto limits = num_input->limits();
        item->setRange(limits.first, limits.second);
        item->setValue(num_input->intValue());

        auto slide = new QSlider(Qt::Horizontal, this);
        slide->setRange(limits.first, limits.second);
        layout->addWidget(slide);

        connect(item, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[=](double i){
            num_input->resetValue(i);
            slide->setValue(i);
        });

        connect(slide, QOverload<int>::of(&QSlider::valueChanged), [=](int v){
            num_input->resetValue(v);
            item->setValue(v);
        });

    }
}
