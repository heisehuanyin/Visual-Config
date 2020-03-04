#ifndef ITEMSGROUP_H
#define ITEMSGROUP_H

#include <QGroupBox>
#include "../ConfigFoundation/configv1.h"

namespace CustomWidget {
    class QuickWidget : public QWidget
    {
    public:
        QuickWidget(QWidget *m_widget, WSFoundation::ConfigV1::ItemsGroup *group,
                    WSFoundation::ConfigV1::ConfigItem *item, QWidget *parent=nullptr);
        virtual ~QuickWidget() override = default;

    private:
        WSFoundation::ConfigV1::ItemsGroup *group;
        WSFoundation::ConfigV1::ConfigItem *item;
    };

    class ItemsGroup : public QGroupBox
    {
        Q_OBJECT
    public:
        ItemsGroup(WSFoundation::ConfigV1::ItemsGroup *group, QWidget *parent=nullptr);

    private:
        WSFoundation::ConfigV1::ItemsGroup *group;
        QWidget *genWidget(WSFoundation::ConfigV1::ConfigItem *item);
    };
}


#endif // ITEMSGROUP_H
