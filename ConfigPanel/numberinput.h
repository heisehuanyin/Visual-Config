#ifndef NUMBERINPUT_H
#define NUMBERINPUT_H

#include "../ConfigFoundation/configv1.h"
#include <QSpinBox>
#include <QWidget>

namespace CustomWidget{
    class NumberInput : public QWidget
    {
        Q_OBJECT
    public:
        explicit NumberInput(WSFoundation::ConfigV1::NumberInput *num_input, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::NumberInput *num_input;
        bool    async_symbo;
    };
}

#endif // NUMBERINPUT_H
