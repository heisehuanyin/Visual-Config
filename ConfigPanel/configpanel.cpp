#include "configpanel.h"
#include "boolindicator.h"
#include "numberinput.h"
#include "pathinput.h"
#include "selectlist.h"
#include "textinput.h"
#include "colorinput.h"
#include "fontconfig.h"
#include "setstack.h"
#include "optionalgroup.h"
#include "itemsgroup.h"

using namespace WSFoundation;
using namespace WSFoundation::ConfigV1;


ConfigPanel::ConfigPanel(Config *entity, QWidget *parent)
    :QScrollArea (parent), config_xml(entity)
{
    auto base_panel = new QWidget(this);
    setWidget(base_panel);
    setWidgetResizable(true);
    base_layout = new QVBoxLayout(base_panel);
    base_layout->setSpacing(2);
    base_layout->setMargin(10);

    config_xml->initChildren();


    for (auto index=0; index<config_xml->itemsCount(); ++index) {
        auto item = config_xml->getItem(index);
        QWidget *widget = nullptr;

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

        base_layout->addWidget(widget);
    }
}
