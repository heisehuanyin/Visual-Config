#include "setstack.h"

#include "boolindicator.h"
#include "colorinput.h"
#include "fontconfig.h"
#include "numberinput.h"
#include "pathinput.h"
#include "textinput.h"
#include "selectlist.h"
#include "itemsgroup.h"
#include "optionalgroup.h"

#include <QComboBox>
#include <QLabel>
#include <QStackedLayout>
#include <QFrame>

using namespace CustomWidget;

SetStack::SetStack(WSFoundation::ConfigV1::SetStack *stack, QWidget *parent)
    : QFrame(parent), stack(stack)
{
    setFrameStyle(QFrame::Sunken|QFrame::Box);
    auto base_layout = new QGridLayout(this);
    base_layout->setMargin(2);
    base_layout->setSpacing(0);
    base_layout->addWidget(new QLabel(stack->configTitle(), this));
    auto select_t = new QComboBox(this);
    auto list = stack->allKeys();
    for (auto key:list) {
        select_t->addItem(stack->getItemsSet(key)->configTitle(), key);
    }
    base_layout->addWidget(select_t, 0, 1, 1, 3);

    auto bottom = new QFrame(this);
    bottom->setFrameStyle(QFrame::Sunken|QFrame::Panel);
    base_layout->addWidget(bottom, 1, 0, 4, 4);

    auto layout = new QStackedLayout(bottom);
    for (int i=0;i<stack->itemsCount();++i) {
        auto item = dynamic_cast<WSFoundation::ConfigV1::ItemsSet*>(stack->getItem(i));
        layout->addWidget(new ItemsSet(item));
    }

    connect(select_t, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int v){
        stack->setActivedKey(select_t->itemData(v).toString());
        layout->setCurrentIndex(v);
    });
    auto key = stack->activedKey();
    select_t->setCurrentIndex(1);
    select_t->setCurrentIndex(0);
    for (auto i=0; i<select_t->count(); ++i) {
        if(select_t->itemData(i).toString() == key){
            select_t->setCurrentIndex(i);
            break;
        }
    }
}

ItemsSet::ItemsSet(WSFoundation::ConfigV1::ItemsSet *set, QWidget *parent)
    :QGroupBox (set->configTitle(), parent), set(set)
{
    auto layout = new QVBoxLayout(this);
    QWidget *widget = nullptr;

    for (auto index=0; index<set->itemsCount(); ++index) {
        auto item = set->getItem(index);
        switch (item->ItemType()) {
            case WSFoundation::ConfigV1::ConfigItem::BOOL_INDICATOR:
                widget = new CustomWidget::BoolIndicator(
                             static_cast<WSFoundation::ConfigV1::BoolIndicator*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::NUMBER_INPUT:
                widget = new CustomWidget::NumberInput(
                             static_cast<WSFoundation::ConfigV1::NumberInput*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::TEXT_INPUT:
                widget = new CustomWidget::TextInput(
                             static_cast<WSFoundation::ConfigV1::TextInput*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::FONT_CONFIG:
                widget = new CustomWidget::FontConfig(
                             static_cast<WSFoundation::ConfigV1::FontConfig*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::PATH_INPUT:
                widget = new CustomWidget::PathInput(
                             static_cast<WSFoundation::ConfigV1::PathInput*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::COLOR_INPUT:
                widget = new CustomWidget::ColorInput(
                             static_cast<WSFoundation::ConfigV1::ColorInput*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::SELECT_LIST:
                widget = new CustomWidget::SelectList(
                             static_cast<WSFoundation::ConfigV1::SelectList*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::ITEMS_GROUP:
                widget = new CustomWidget::ItemsGroup(
                             dynamic_cast<WSFoundation::ConfigV1::ItemsGroup*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::OPTIONAL_GROUP:
                widget = new CustomWidget::OptionalGroup(
                             dynamic_cast<WSFoundation::ConfigV1::OptionalGroup*>(item), this);
                break;
            case WSFoundation::ConfigV1::ConfigItem::SET_STACK:
                widget = new CustomWidget::SetStack(
                             static_cast<WSFoundation::ConfigV1::SetStack*>(item), this);
                break;
            default:
                break;
        }

        layout->addWidget(widget);
    }
}
