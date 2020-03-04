#ifndef OPTIONALGROUP_H
#define OPTIONALGROUP_H

#include <QGroupBox>
#include "../ConfigFoundation/configv1.h"


namespace CustomWidget {
    class OptionalGroup : public QGroupBox
    {
    public:
        OptionalGroup(WSFoundation::ConfigV1::OptionalGroup *group, QWidget *parent=nullptr);

    private:
        WSFoundation::ConfigV1::OptionalGroup *group;
    };
}

#endif // OPTIONALGROUP_H
