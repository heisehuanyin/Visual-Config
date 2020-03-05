#ifndef COLORINPUT_H
#define COLORINPUT_H

#include <QComboBox>
#include <QSpinBox>
#include <QWidget>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {

    class ColorInput : public QWidget
    {
        Q_OBJECT
    public:
        explicit ColorInput(WSFoundation::ConfigV1::ColorInput *color_in, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::ColorInput *color_in;
        QSpinBox    *i1,*i2,*i3,*i4,*i5;
        QComboBox   *combobox;

    private slots:
        void value_change(int);
    };
}

#endif // COLORINPUT_H
