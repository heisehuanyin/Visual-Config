#include "selectlist.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

using namespace CustomWidget;
SelectList::SelectList(WSFoundation::ConfigV1::SelectList *select_list, QWidget *parent)
    :QWidget (parent), select_list(select_list)
{
    auto layout = new QHBoxLayout(this);
    layout->setMargin(2);

    auto label = new QLabel(select_list->configTitle(), this);
    label->setFrameStyle(QFrame::Sunken|QFrame::Box);
    layout->addWidget(label);

    auto select = new QComboBox(this);
    layout->addWidget(select);
    auto keys = select_list->availableKeys();
    for (auto key:keys) {
        select->addItem(select_list->itemName(key), key);
    }
    if(select_list->currentKey() == QString() && select->count()){
        select_list->selectItem(select->itemData(0).toString());
    }

    connect(select, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        auto key = select->itemData(index).toString();
        select_list->selectItem(key);
    });

    layout->setStretch(0, 0);
    layout->setStretch(1, 1);
}
