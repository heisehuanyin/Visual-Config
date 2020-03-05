#include "textinput.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

using namespace CustomWidget;

TextInput::TextInput(WSFoundation::ConfigV1::TextInput *input, QWidget *parent)
    :QWidget (parent), input(input)
{
    auto layout = new QHBoxLayout(this);
    layout->setMargin(2);

    auto label = new QLabel(input->configTitle(), this);
    label->setFrameStyle(QFrame::Sunken|QFrame::Box);
    layout->addWidget(label);

    auto text_in = new QLineEdit(this);
    layout->addWidget(text_in);
    text_in->setPlaceholderText(input->preContent());
    text_in->setText(input->content());

    connect(text_in, QOverload<const QString&>::of(&QLineEdit::textChanged), [=](const QString &content){
        input->reset(content);
    });
}
