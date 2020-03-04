#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include "../ConfigFoundation/configv1.h"

int main(int argc, char *argv[])
{

    auto port = WSFoundation::ConfigV1::Config::genNewInstance();
    port->appendBoolIndicator("config0", true)->reset(false);
    port->appendPathInput("config1", WSFoundation::ConfigV1::PathInput::FILE)->resetPath("print-path");
    port->appendTextInput("config2", "nocontent")->reset("hello world.");
    port->appendColorInput("config3")->resetValue3(20,20,20);
    port->appendColorInput("config3-2")->resetMode(WSFoundation::ConfigV1::ColorInput::CMYK);
    port->appendFontConfig("config4", "no-font");
    port->appendSelectList("config5")->genSelectItem("key","间或");
    port->appendNumberInput("config6", WSFoundation::ConfigV1::NumberInput::DECIMAL)->resetValue(20);

    port->appendItemsGroup("group0")->appendTextInput("OK inner group.")->reset("voidstring");
    port->appendSetStack("ffffcd")->newItemsSet("key-1", "no title")->appendTextInput("objk");
    port->appendOptionalGroup("group2")->appendTextInput("some rights");

    auto list = port->appendItemsGroup("template-test");
    auto tnode = list->appendTextInput("template-element", "no time");
    list->resetTemplateAndRelaseTargetIns(tnode);
    list->newTemplateElement();
    list->newTemplateElement();
    list->newTemplateElement();

    QFile f("./something.xml");
    f.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&f);
    out << port->toString();
    out.flush();

}
