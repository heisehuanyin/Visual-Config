#ifndef CONFIGV1_H
#define CONFIGV1_H

#include <QtCore/qglobal.h>
#include <QList>
#include <QPair>
#include <tuple>

#if defined(CONFIGFOUNDATION_LIBRARY)
#  define CONFIGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONFIGSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace WSFoundation {
    namespace ConfigV1 {
        // 基本配置条目 ======================================================
        class ConfigItem
        {
        public:
            enum TypeMask{
                BOOL_INDICATOR,
                NUMBER_INPUT,
                TEXT_INPUT,
                SELECT_LIST,
                FONT_CONFIG,
                PATH_INPUT,
                COLOR_INPUT,
                ITEMS_GROUP,
                OPTIONAL_GROUP,
                ITEMS_SET,
                SET_STACK,
                ROOT_ELM
            };

            virtual ~ConfigItem() = default;
            /**
             * @brief 返回配置项名称，不是dom节点名称
             * @return
             */
            virtual QString configTitle() const = 0;
            virtual TypeMask ItemType() const = 0;
        };

        class BoolIndicator : public ConfigItem
        {
        public:
            virtual ~BoolIndicator() override = default;
            virtual bool value() const = 0;
            virtual void reset(bool value) = 0;
        };

        class NumberInput: public ConfigItem
        {
        public:
            enum Type{
                INTEGER,
                DECIMAL
            };

            virtual ~NumberInput() override = default;
            virtual Type type() const = 0;
            virtual QPair<double,double> limits() const = 0;
            virtual void resetLimits(double down_limit =INT_MIN, double up_limit = INT_MAX) = 0;
            virtual double doubleValue() = 0;
            virtual int intValue() = 0;
            virtual void resetValue(double value) = 0;
        };

        class TextInput: public ConfigItem
        {
        public:
            virtual ~TextInput() override = default;
            virtual QString preContent() const = 0;
            virtual QString content() const = 0;
            virtual void reset(const QString &content) = 0;
        };

        class SelectList: public ConfigItem
        {
        public:
            virtual ~SelectList() override = default;

            virtual bool genSelectItem(const QString &unique_key, const QString &item_name) = 0;
            virtual QList<QString> availableKeys() const = 0;
            virtual QString itemName(const QString &unique_key) const = 0;
            virtual QString currentKey() const = 0;
            virtual void selectItem(const QString &key_str) = 0;
        };

        class FontConfig: public ConfigItem
        {
        public:
            virtual ~FontConfig() override = default;
            virtual QString fontFamily() const = 0;
            virtual void resetFont(const QString &font_family) = 0;
            virtual double fontSize() const = 0;
            virtual void resetFontSize(double size) = 0;
            virtual void setBold(bool state) = 0;
            virtual bool isBold() const = 0;
            virtual void setItalic(bool state) = 0;
            virtual bool isItalic() const = 0;
            virtual void setStrikeOut(bool state) = 0;
            virtual bool isStrikeOut() const = 0;
            virtual void setUnderline(bool state)=0;
            virtual bool isUnderline() const = 0;
        };

        class PathInput: public ConfigItem
        {
        public:
            enum Type{
                DIRECTORY,
                FILE,
            };

            virtual ~PathInput() override = default;
            virtual Type type() const = 0;
            virtual QString path() const = 0;
            virtual void resetPath(const QString &path) = 0;
        };

        class ColorInput: public ConfigItem
        {
        public:
            enum Mode{
                RGB,
                HSL,
                HSV,
                CMYK
            };

            virtual ~ColorInput() override = default;
            virtual Mode mode() const = 0;
            virtual void resetMode(Mode color_mode) = 0;

            virtual std::tuple<int,int,int,int> value3() const = 0;
            virtual void resetValue3(int r_h, int g_s, int b_lv, int a=255) = 0;

            virtual std::tuple<int,int,int,int,int> value4() const = 0;
            virtual void resetValue4(int c, int m, int y, int k, int a=255) = 0;
        };


        // 基本配置条目容器 ====================================================
        class ItemsGroup;
        class OptionalGroup;
        class SetStack;

        class Group
        {
        public:
            virtual ~Group() = default;

            virtual void initChildren() = 0;
            virtual int itemsCount() const = 0;
            virtual ConfigItem* getItem(int index) const = 0;
        };

        class ConfigGroup : virtual public ConfigItem, public Group
        {
        public:
            virtual ~ConfigGroup() = default;

            /**
             * @brief 直接添加一个配置节点
             * @return
             */
            virtual BoolIndicator* appendBoolIndicator(const QString &title_name, bool default_value=true) = 0;
            virtual TextInput* appendTextInput(const QString &title_name, const QString &pre_content=QString()) = 0;
            virtual NumberInput* appendNumberInput(const QString &title_name, NumberInput::Type type=NumberInput::DECIMAL) = 0;
            virtual FontConfig* appendFontConfig(const QString &title_name, const QString &family_name) = 0;
            virtual ColorInput* appendColorInput(const QString &title_name) = 0;
            virtual PathInput* appendPathInput(const QString &title_name, PathInput::Type type=PathInput::FILE) = 0;
            virtual SelectList* appendSelectList(const QString &title_name) = 0;
            virtual ItemsGroup* appendItemsGroup(const QString &title_name, bool title_editable=false) = 0;
            virtual OptionalGroup* appendOptionalGroup(const QString title_name, bool enable=true) = 0;
            virtual SetStack* appendSetStack(const QString &title_name) = 0;
        };

        /**
         * @brief The ItemsGroup class
         * 允许设置模板，由上游配置端自由添加配置条目。
         * 设置模板的条目，默认允许上游增删条目，反之不允许
         */
        class ItemsGroup : virtual public ConfigGroup
        {
        public:
            virtual ~ItemsGroup() override = default;
            virtual bool isTitleEditable() const = 0;
            virtual void resetTitle(const QString &title) = 0;

            virtual bool resetTemplateAndRelaseTargetIns(ConfigItem *t) = 0;
            virtual bool isItemEditable() const = 0;
            virtual ConfigItem* newTemplateElement() = 0;
            virtual void removeItemAt(int index) = 0;
        };

        /**
         * @brief The OptionalGroup class
         * 上游决定其内部条目是否可用
         */
        class OptionalGroup : virtual public ConfigGroup
        {
        public:
            virtual ~OptionalGroup() override = default;
            virtual bool isEnable() = 0;
            virtual void resetEnable(bool value) = 0;
        };

        /**
         * @brief The ItemsSet class
         * 固定配置的基本面板，用于切换整套配置条目
         */
        class ItemsSet : virtual public ConfigGroup
        {
        public:
            virtual ~ItemsSet() override = default;
            virtual QString bindKey() const = 0;
        };

        /**
         * @brief The StackedSet class
         * 可切换的成套配置条目承载，选择其中一套配置
         */
        class SetStack : public ConfigItem, public Group
        {
        public:
            virtual ~SetStack() = default;
            virtual ItemsSet* newItemsSet(const QString &key, const QString &title) = 0;

            virtual QList<QString> allKeys() = 0;
            virtual QString activedKey() = 0;
            virtual void setActivedKey(const QString &key) = 0;
            virtual ItemsSet* getItemsSet(const QString &key) = 0;
        };


        /**
         * @brief The ConfigFoundation class
         * 两种途径进行代码和配置之间进行交互：
         * 1. code -> xml
         *    config(content-string-or-empty);  生成或解析已存在配置文档
         *    auto item = config->genXXXXX();   生成实例若配置文档已解析，同时载入状态
         *    * item是带类型实例可以直接交互
         * 2. xml -> code
         *    config(content-string);           载入已存在配置文档
         *    config->parserToItems();          根据xml生成配置条目
         *    int count = config->itemCount();  获取group中配置项目数量
         *    auto item = config->itemAt(index);获取指定配置条目
         *    if(item->typeMask() == "xxxx")
         *      auto itemXX = static_case<XX>(item);
         *    * 经过转换itemXX是带类型实例，可以直接交互
         */
        class CONFIGSHARED_EXPORT Config : virtual public ConfigGroup
        {
        public:
            virtual ~Config() override = default;

            static Config *genNewInstance(const QString &content=QString(), QString *error_out=nullptr);

            virtual QString toString() const = 0;
            virtual bool containsChildNode(ConfigItem *node) = 0;
            virtual void releaseChildInstance(ConfigItem *node) = 0;
        };
    }
}

#endif // CONFIGV1_H
