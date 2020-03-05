#ifndef SELECTLIST_H
#define SELECTLIST_H

#include <QWidget>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {
    class SelectList : public QWidget
    {
        Q_OBJECT
    public:
        explicit SelectList(WSFoundation::ConfigV1::SelectList *select_list, QWidget *parent = nullptr);

    private:
        WSFoundation::ConfigV1::SelectList *select_list;
    };
}

#endif // SELECTLIST_H
