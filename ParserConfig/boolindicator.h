#ifndef BOOLINDICATOR_H
#define BOOLINDICATOR_H

#include <QWidget>

#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {

    class BoolIndicator : public QWidget
    {
        Q_OBJECT
    public:
        BoolIndicator(WSFoundation::ConfigV1::BoolIndicator *data, QWidget *parent=nullptr);
        virtual ~BoolIndicator() override;

    private:
        WSFoundation::ConfigV1::BoolIndicator *bool_indicator;

    private slots:
        void accept(bool state);
    };
}

#endif // BOOLINDICATOR_H
