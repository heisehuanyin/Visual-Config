#ifndef FONTCONFIG_H
#define FONTCONFIG_H

#include <QWidget>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {

    class FontConfig : public QWidget
    {
        Q_OBJECT
    public:
        explicit FontConfig(WSFoundation::ConfigV1::FontConfig *config, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::FontConfig *config;

    };

}

#endif // FONTCONFIG_H
