#include "boolindicator.h"
#include "../ConfigFoundation/configv1.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QtDebug>

using namespace CustomWidget;

BoolIndicator::BoolIndicator(WSFoundation::ConfigV1::BoolIndicator *data, QWidget *parent)
    :QWidget (parent), bool_indicator(data)
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(2);
    auto btn = new QCheckBox(bool_indicator->configTitle(), this);
    btn->setCheckable(true);
    btn->setChecked(bool_indicator->value());
    connect(btn, &QCheckBox::clicked,   this,   &BoolIndicator::accept);
    layout->addWidget(btn);
}

BoolIndicator::~BoolIndicator()
{

}

void BoolIndicator::accept(bool state)
{
    bool_indicator->reset(state);
}
