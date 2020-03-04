#ifndef SETSTACK_H
#define SETSTACK_H

#include <QGroupBox>
#include <QWidget>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {

    class ItemsSet : public QGroupBox
    {
        Q_OBJECT
    public:
        explicit ItemsSet(WSFoundation::ConfigV1::ItemsSet *set, QWidget *parent = nullptr);


    private:
        WSFoundation::ConfigV1::ItemsSet *set;
    };

    class SetStack : public QFrame
    {
        Q_OBJECT
    public:
        explicit SetStack(WSFoundation::ConfigV1::SetStack *stack, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::SetStack *stack;
    };
}

#endif // SETSTACK_H
