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

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtDebug>

using namespace CustomWidget;

ItemsGroup::ItemsGroup(WSFoundation::ConfigV1::ItemsGroup *group, QWidget *parent)
    :QGroupBox (group->configTitle(),parent), group(group)
{
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    if(group->isTitleEditable()){
        auto w2 = new QWidget(this);
        auto lay2 = new QHBoxLayout(w2);
        lay2->addWidget(new QLabel("Title Enter:"));
        auto enter = new QLineEdit(group->configTitle(),this);
        connect(enter,  &QLineEdit::textChanged,    [=](const QString &c){
            group->resetTitle(c);
            this->setTitle(c);
        });
        lay2->addWidget(enter);
        layout->addWidget(w2);
    }

    for (auto index=0; index<group->itemsCount(); ++index) {
        auto item = group->getItem(index);
        QWidget *widget_p = genWidget(item);

        if(group->isItemEditable()){
            auto w_base = new QuickWidget(widget_p, group, item, this);
            layout->addWidget(w_base);
        }else {
            layout->addWidget(widget_p);
        }
    }

    if(group->isItemEditable()){
        auto btn = new QPushButton("+  添加新条目", this);
        layout->addWidget(btn);
        connect(btn,    QOverload<bool>::of(&QPushButton::clicked),     [=](bool){
            auto items = group->newTemplateElement();
            auto awis = genWidget(items);

            auto w_base = new QuickWidget(awis, group, items, this);
            layout->insertWidget(layout->count()-1,w_base);
        });
    }

}

QWidget *ItemsGroup::genWidget(WSFoundation::ConfigV1::ConfigItem *item)
{
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

    return widget;
}

QuickWidget::QuickWidget(QWidget *m_widget, WSFoundation::ConfigV1::ItemsGroup*group,
                         WSFoundation::ConfigV1::ConfigItem* item, QWidget *parent)
    :QWidget (parent), group(group),item(item)
{
    auto hbox = new QHBoxLayout(this);
    hbox->setMargin(0);
    hbox->setSpacing(0);

    m_widget->setParent(this);
    hbox->addWidget(m_widget);

    auto pbtn = new QPushButton("删除", this);
    hbox->addWidget(pbtn);
    connect(pbtn,    QOverload<bool>::of(&QPushButton::clicked), [=](bool){
        for (auto index=0; index<group->itemsCount(); ++index) {
            if(group->getItem(index)==item){
                group->removeItemAt(index);
                break;
            }
        }
        delete this;
    });
}
