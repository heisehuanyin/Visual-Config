#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QWidget>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {
    class TextInput : public QWidget
    {
        Q_OBJECT
    public:
        explicit TextInput(WSFoundation::ConfigV1::TextInput *input, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::TextInput *input;
    };
}

#endif // TEXTINPUT_H
