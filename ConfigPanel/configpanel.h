#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QtCore/qglobal.h>
#include <QScrollArea>
#include <QVBoxLayout>
#include "configv1.h"

#if defined(CONFIGPANEL_LIBRARY)
#  define CONFIGPANELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONFIGPANELSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace WSFoundation {

    class CONFIGPANELSHARED_EXPORT ConfigPanel: public QScrollArea
    {

    public:
        ConfigPanel(ConfigV1::Config *config_xml, QWidget *parent=nullptr);
        virtual ~ConfigPanel() = default;

    private:
        ConfigV1::Config *config_xml;
        QVBoxLayout *base_layout;
    };
}

#endif // CONFIGPANEL_H
