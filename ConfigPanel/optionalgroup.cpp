#include "boolindicator.h"
#include "colorinput.h"
#include "fontconfig.h"
#include "itemsgroup.h"
#include "numberinput.h"
#include "optionalgroup.h"
#include "pathinput.h"
#include "selectlist.h"
#include "setstack.h"
#include "textinput.h"

#include <QHBoxLayout>

using namespace CustomWidget;

OptionalGroup::OptionalGroup(WSFoundation::ConfigV1::OptionalGroup *groupin, QWidget *parent)
    :QGroupBox (groupin->configTitle(), parent), group(groupin)
{
    setCheckable(true);
    connect(this,   QOverload<bool>::of(&QGroupBox::clicked), [=](bool v){
        group->resetEnable(v);
    });

    auto layout = new QVBoxLayout(this);
    QWidget *widget = nullptr;

    for (auto index=0; index<group->itemsCount(); ++index) {
        auto item = group->getItem(index);
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

    setChecked(group->isEnable());
}
