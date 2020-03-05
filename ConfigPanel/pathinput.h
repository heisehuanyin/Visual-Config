#ifndef PATHINPUT_H
#define PATHINPUT_H

#include <QWidget>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {

    class PathInput : public QWidget
    {
        Q_OBJECT
    public:
        explicit PathInput(WSFoundation::ConfigV1::PathInput *path_in, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::PathInput *path_in;
    };
}

#endif // PATHINPUT_H
