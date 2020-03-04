#include "configv1.h"
#include <QDomDocument>
#include <QtDebug>
#include <QColor>

using namespace WSFoundation::ConfigV1;

class ImplLoadCommon
{
public:
    virtual ~ImplLoadCommon() = default;
    /**
     * @brief 校验dom节点数据是否合格
     * @param elm
     * @return
     */
    virtual bool testPass(const QDomElement& elm) = 0;
    /**
     * @brief 载入合格dom节点的数据
     * @param elm
     */
    virtual void loadFrom(QDomElement &elm) = 0;

    virtual QDomElement innerDomNode() = 0;
};

class RBoolIndicator : public BoolIndicator,public ImplLoadCommon
{
public:
    RBoolIndicator(QDomElement &bind_at, const QString &name);
    virtual ~RBoolIndicator() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override;
    virtual TypeMask ItemType() const override;

    // BoolIndicator interface
public:
    virtual bool value() const override;
    virtual void reset(bool value) override;

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override;
    virtual void loadFrom(QDomElement &elm) override;
    virtual QDomElement innerDomNode() override;

private:
    QDomElement elm_store;
    QString name_store;
    bool value_store;
};

class RNumberInput : public NumberInput, public ImplLoadCommon
{
public:
    RNumberInput(QDomElement &bind_at, const QString &name, Type type)
        :target_elm(bind_at), name_store(name), type_store(type),
          up_limit(INT_MAX), down_limit(INT_MIN), value_store(0)
    {
        auto list = bind_at.childNodes();
        for (auto i=0; i<list.size(); ++i) {
            bind_at.removeChild(list.at(i));
        }

        bind_at.appendChild(bind_at.ownerDocument().createTextNode(name));

        bind_at.setTagName("number-input");
        bind_at.setAttribute("type", type==Type::INTEGER?"integer":"decimal");
        bind_at.setAttribute("limits", QString("%1:%2").arg(down_limit).arg(up_limit));
        bind_at.setAttribute("value", QString("%1").arg(value_store));
    }

    virtual ~RNumberInput() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return name_store;
    }
    virtual TypeMask ItemType() const override{
        return TypeMask::NUMBER_INPUT;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="number-input";
    }
    virtual void loadFrom(QDomElement &elm) override{
        target_elm = elm;
        name_store = elm.childNodes().at(0).toText().data();
        type_store = elm.attribute("type") == "integer"? Type::INTEGER : Type::DECIMAL;
        auto string_2 = elm.attribute("limits").split(":");
        down_limit = string_2.at(0).toDouble();
        up_limit = string_2.at(1).toDouble();
        value_store = elm.attribute("value").toDouble();
    }
    virtual QDomElement innerDomNode() override{
        return target_elm;
    }

    // NumberInput interface
public:
    virtual Type type() const override{
        return type_store;
    }
    virtual QPair<double, double> limits() const override{
        return qMakePair(down_limit, up_limit);
    }
    virtual void resetLimits(double down_limit, double up_limit) override{
        this->down_limit = down_limit;
        this->up_limit = up_limit;
        target_elm.setAttribute("limits", QString("%1:%2").arg(down_limit).arg(up_limit));
    }
    virtual double doubleValue() override{
        return value_store;
    }
    virtual int intValue() override{
        auto a = static_cast<int>(value_store);
        if(value_store-a < 0.5)
            return a;
        else
            return a+1;
    }
    virtual void resetValue(double value) override{
        value_store = value;
        target_elm.setAttribute("value", QString("%1").arg(value));
    }

private:
    QDomElement target_elm;
    QString name_store;
    Type type_store;
    double up_limit;
    double down_limit;
    double value_store;

};

class RTextInput: public TextInput, public ImplLoadCommon
{
public:
    RTextInput(QDomElement &bind_at, const QString &name, const QString &pre_content)
        :target_node(bind_at), name_store(name), text_store(""), pre_content_store(pre_content)
    {
        auto list = bind_at.childNodes();
        for (auto i=0; i<list.count(); ++i) {
            bind_at.removeChild(list.at(i));
        }

        target_node.appendChild(bind_at.ownerDocument().createTextNode(name));
        target_node.setTagName("text-input");
        target_node.setAttribute("pre-content", pre_content);
        target_node.setAttribute("content", "");
    }
    virtual ~RTextInput() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return name_store;
    }
    virtual TypeMask ItemType() const override{
        return TypeMask::TEXT_INPUT;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="text-input";
    }
    virtual void loadFrom(QDomElement &elm) override{
        target_node = elm;
        name_store = elm.childNodes().at(0).toText().data();
        text_store = elm.attribute("content");
        pre_content_store = elm.attribute("pre-content");
    }

    virtual QDomElement innerDomNode() override{
        return target_node;
    }

    // TextInput interface
public:
    virtual QString preContent() const override{
        return pre_content_store;
    }
    virtual QString content() const override{
        return text_store;
    }
    virtual void reset(const QString &content) override{
        text_store = content;
        target_node.setAttribute("content", content);
    }

private:
    QDomElement target_node;
    QString name_store;
    QString text_store;
    QString pre_content_store;
};

class RSelectList: public SelectList, public ImplLoadCommon
{
public:
    RSelectList(QDomElement &bind_at, const QString &title_name)
        :target(bind_at), title_store(title_name)
    {
        auto list = bind_at.childNodes();
        for (int i=0; i<list.size(); ++i) {
            bind_at.removeChild(list.at(i));
        }

        bind_at.setTagName("select-list");
        bind_at.setAttribute("selected-item", "");
        bind_at.setAttribute("title", title_name);
    }
    virtual ~RSelectList() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return title_store;
    }
    virtual TypeMask ItemType() const override{
        return TypeMask::SELECT_LIST;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="select-list";
    }
    virtual void loadFrom(QDomElement &elm) override{
        target = elm;
        title_store = elm.attribute("title");
    }

    virtual QDomElement innerDomNode() override{
        return target;
    }

    // SelectList interface
public:
    virtual bool genSelectItem(const QString &unique_key, const QString &item_name) override{
        auto list = target.childNodes();
        for (int i=0; i<list.size(); ++i) {
            if(list.at(i).toElement().attribute("key") == unique_key)
                return false;
        }

        auto item1 = target.ownerDocument().createElement("item");
        item1.setAttribute("key", unique_key);
        item1.setAttribute("name", item_name);
        target.appendChild(item1);

        return true;
    }
    virtual QList<QString> availableKeys() const override{
        QList<QString> list;
        auto nodes = target.childNodes();
        for (int ind=0; ind<nodes.size(); ++ind) {
            list << nodes.at(ind).toElement().attribute("key");
        }

        return list;
    }
    virtual QString itemName(const QString &unique_key) const override{
        auto nodes = target.childNodes();
        for (int ind=0; ind<nodes.size(); ++ind) {
            auto itelm = nodes.at(ind).toElement();
            if(itelm.attribute("key") == unique_key)
                return itelm.attribute("name");
        }

        return "";
    }
    virtual QString currentKey() const override{
        return target.attribute("selected-item");
    }
    virtual void selectItem(const QString &key_str) override{
        target.setAttribute("selected-item", key_str);
    }

private:
    QDomElement target;
    QString title_store;

};

class RFontConfig: public FontConfig, public ImplLoadCommon
{
public:
    RFontConfig(QDomElement &bind_at, const QString &title_name, const QString &default_font_name)
        :target(bind_at), title_store(title_name),font_name_store(default_font_name), size_store(11),
          bold_symbo(false), italic_symbo(false), strikeout_symbo(false), underline_symbo(false)
    {
        target.setTagName("font-config");
        target.appendChild(bind_at.ownerDocument().createTextNode(title_name));
        target.setAttribute("family", default_font_name);
        target.setAttribute("size", QString("%1").arg(size_store));
        target.setAttribute("italic", "false");
        target.setAttribute("strikeout", "false");
        target.setAttribute("underline", "false");
        target.setAttribute("bold", "false");
    }
    virtual ~RFontConfig() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return title_store;
    }
    virtual TypeMask ItemType() const override{
        return TypeMask::FONT_CONFIG;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName() == "font-config";
    }
    virtual void loadFrom(QDomElement &elm) override{
        target = elm;
        title_store = target.childNodes().at(0).toText().data();
        font_name_store = target.attribute("family");
        size_store = target.attribute("size").toDouble();
        italic_symbo = target.attribute("italic")=="true";
        strikeout_symbo=target.attribute("strikeout")=="true";
        underline_symbo=target.attribute("underline")=="true";
        bold_symbo=target.attribute("bold")=="true";
    }

    virtual QDomElement innerDomNode() override{
        return target;
    }

    // FontConfig interface
public:
    virtual QString fontFamily() const override{
        return font_name_store;
    }
    virtual void resetFont(const QString &font_family) override{
        font_name_store = font_family;
        target.setAttribute("family", font_family);
    }
    virtual double fontSize() const override{
        return size_store;
    }
    virtual void resetFontSize(double size) override{
        size_store = size;
        target.setAttribute("size", QString("%1").arg(size));
    }

    virtual void setBold(bool state) override{
        bold_symbo=state;
        target.setAttribute("bold", state?"true":"false");
    }
    virtual bool isBold() const override{
        return bold_symbo;
    }
    virtual void setItalic(bool state) override{
        italic_symbo=state;
        target.setAttribute("italic", state?"true":"false");
    }
    virtual bool isItalic() const override{
        return italic_symbo;
    }
    virtual void setStrikeOut(bool state) override{
        strikeout_symbo=state;
        target.setAttribute("strikeout",state?"true":"false");
    }
    virtual bool isStrikeOut() const override{
        return strikeout_symbo;
    }
    virtual void setUnderline(bool state) override{
        underline_symbo=state;
        target.setAttribute("underline",state?"true":"false");
    }
    virtual bool isUnderline() const override{
        return underline_symbo;
    }

private:
    QDomElement target;
    QString title_store;
    QString font_name_store;
    double size_store;
    bool bold_symbo;
    bool italic_symbo;
    bool strikeout_symbo;
    bool underline_symbo;

};

class RPathInput: public PathInput, public ImplLoadCommon
{
public:
    RPathInput(QDomElement &bind_at, Type type, const QString &title_name)
        :target(bind_at), item_type(type), title_store(title_name), path_store("./")
    {
        target.setTagName("path-input");
        target.appendChild(bind_at.ownerDocument().createTextNode(title_name));
        target.setAttribute("type", type==Type::FILE?"file":"dir");
        target.setAttribute("content", path_store);
    }
    virtual ~RPathInput() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return title_store;
    }
    virtual TypeMask ItemType() const override{
        return TypeMask::PATH_INPUT;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="path-input";
    }
    virtual void loadFrom(QDomElement &elm) override{
        target = elm;
        item_type = elm.attribute("type")=="file"?Type::FILE:Type::DIRECTORY;
        title_store = elm.childNodes().at(0).toText().data();
        path_store = elm.attribute("content");
    }
    virtual QDomElement innerDomNode() override{
        return target;
    }

    // PathInput interface
public:
    virtual Type type() const override{
        return item_type;
    }
    virtual QString path() const override{
        return path_store;
    }
    virtual void resetPath(const QString &path) override{
        path_store = path;
        target.setAttribute("content", path);
    }

private:
    QDomElement target;
    Type item_type;
    QString title_store;
    QString path_store;
};

class RColorInput: public ColorInput, public ImplLoadCommon
{
public:
    RColorInput(QDomElement &bind_at, const QString &title_name)
        :target(bind_at), title_store(title_name),value_mode_store(Mode::RGB),
          value_store_1(255),value_store_2(255),value_store_3(255),value_store_4(255),value_store_5(255)
    {
        target.setTagName("color-input");
        target.appendChild(bind_at.ownerDocument().createTextNode(title_name));
        target.setAttribute("mode", "rgb");
        target.setAttribute("value", QString("(%1,%2,%3,%4,%5)").arg(value_store_1)
                            .arg(value_store_2).arg(value_store_3).arg(value_store_4).arg(value_store_5));
    }
    virtual ~RColorInput() override = default;

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return title_store;
    }
    virtual TypeMask ItemType() const override{
        return TypeMask::COLOR_INPUT;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="color-input";
    }
    virtual void loadFrom(QDomElement &elm) override{
        target = elm;
        title_store = elm.childNodes().at(0).toText().data();
        auto vtemp = elm.attribute("mode");
        if(vtemp=="rgb")
            value_mode_store = Mode::RGB;
        if(vtemp=="hsl")
            value_mode_store = Mode::HSL;
        if(vtemp=="hsv")
            value_mode_store = Mode::HSV;
        if(vtemp=="cmyk")
            value_mode_store = Mode::CMYK;

        vtemp = elm.attribute("value");
        vtemp = vtemp.mid(1, vtemp.length()-2);
        auto values = vtemp.split(",");
        value_store_1 = values.at(0).toInt();
        value_store_2 = values.at(1).toInt();
        value_store_3 = values.at(2).toInt();
        value_store_4 = values.at(3).toInt();
        value_store_5 = values.at(4).toInt();
    }
    virtual QDomElement innerDomNode() override{
        return target;
    }

    // ColorInput interface
public:
    virtual Mode mode() const override{
        return value_mode_store;
    }
    virtual void resetMode(Mode color_mode) override{
        if(value_mode_store==color_mode)
            return;

        QColor c;
        switch (value_mode_store) {
            case Mode::RGB:
                c.setRgb(value_store_1,value_store_2,value_store_3,value_store_4);
                break;
            case Mode::HSL:
                c.setHsl(value_store_1,value_store_2,value_store_3,value_store_4);
                break;
            case Mode::HSV:
                c.setHsv(value_store_1,value_store_2,value_store_3,value_store_4);
                break;
            case Mode::CMYK:
                c.setCmyk(value_store_1,value_store_2,value_store_3,value_store_4,value_store_5);
                break;
        }

        value_mode_store = color_mode;
        switch (color_mode) {
            case Mode::RGB:
                target.setAttribute("mode", "rgb");
                c.getRgb(&value_store_1, &value_store_2, &value_store_3, &value_store_4);
                break;
            case Mode::HSL:
                target.setAttribute("mode", "hsl");
                c.getHsl(&value_store_1, &value_store_2, &value_store_3, &value_store_4);
                break;
            case Mode::HSV:
                target.setAttribute("mode", "hsv");
                c.getHsv(&value_store_1, &value_store_2, &value_store_3, &value_store_4);
                break;
            case Mode::CMYK:
                target.setAttribute("mode", "cmyk");
                c.getCmyk(&value_store_1, &value_store_2, &value_store_3, &value_store_4, &value_store_5);
                break;
        }

        target.setAttribute("value", QString("(%1,%2,%3,%4,%5)").arg(value_store_1).arg(value_store_2)
                            .arg(value_store_3).arg(value_store_4).arg(value_store_5));
    }
    virtual std::tuple<int, int, int, int> value3() const override{
        return std::make_tuple(value_store_1, value_store_2, value_store_3, value_store_4);
    }
    virtual void resetValue3(int r_h, int g_s, int b_l_v, int a=255) override{
        if(value_mode_store == CMYK)
            return;

        value_store_1 = r_h;
        value_store_2 = g_s;
        value_store_3 = b_l_v;
        value_store_4 = a;
        value_store_5 = 255;

        target.setAttribute("value", QString("(%1,%2,%3,%4,%5)").arg(value_store_1).arg(value_store_2)
                            .arg(value_store_3).arg(value_store_4).arg(value_store_5));
    }
    virtual std::tuple<int, int, int, int, int> value4() const override{
        return std::make_tuple(value_store_1, value_store_2, value_store_3, value_store_4, value_store_5);
    }
    virtual void resetValue4(int c, int m, int y, int k, int a=255) override{
        if(value_mode_store != CMYK)
            return;
        value_store_1 = c;
        value_store_2 = m;
        value_store_3 = y;
        value_store_4 = k;
        value_store_5 = a;

        target.setAttribute("value", QString("(%1,%2,%3,%4,%5)").arg(value_store_1).arg(value_store_2)
                            .arg(value_store_3).arg(value_store_4).arg(value_store_5));
    }

private:
    QDomElement target;
    QString title_store;
    Mode value_mode_store;
    int value_store_1;
    int value_store_2;
    int value_store_3;
    int value_store_4;
    int value_store_5;

};

class RConfig : public Config
{
public:
    /**
     * @brief 根据传入内容构建配置解析工具，默认传入空字符串创建一个默认配置解析工具
     * @param str_content
     */
    RConfig(const QString str_content = QString(), QString *error_out=nullptr);
    virtual ~RConfig() override;

    virtual QString toString() const override;
    virtual bool containsChildNode(ConfigItem *node) override;
    virtual void releaseChildInstance(ConfigItem *node) override;

    // ConfigItem interface
public:
    virtual QString configTitle() const override;
    virtual TypeMask ItemType() const override;

    // ConfigGroup interface
public:
    virtual void initChildren() override;
    virtual int itemsCount() const override;
    virtual ConfigItem *getItem(int index) const override;
    virtual BoolIndicator *appendBoolIndicator(const QString &name, bool default_value) override;
    virtual ColorInput *appendColorInput(const QString &name) override;
    virtual FontConfig *appendFontConfig(const QString &name, const QString &family_name) override;
    virtual ItemsGroup *appendItemsGroup(const QString &name, bool title_editable) override;
    virtual NumberInput *appendNumberInput(const QString &name, NumberInput::Type type) override;
    virtual PathInput *appendPathInput(const QString &name, PathInput::Type type) override;
    virtual SelectList *appendSelectList(const QString &name) override;
    virtual TextInput *appendTextInput(const QString &name, const QString &pre_content) override;
    virtual SetStack *appendSetStack(const QString &name) override;
    virtual OptionalGroup *appendOptionalGroup(const QString title_name, bool enable) override;

protected:
    /**
     * @brief 一个中间构造器，专门为继承子类设计，不要在外部调用它
     * @param doc document 节点
     * @param tag_elm 根节点
     */
    void clearStateAndResetBase(QDomDocument doc, QDomElement &tag_elm);
    /**
     * @brief 传入本dom节点直接子节点，实例化组件后添加入托管集合
     * @param root_base 直接dom子节点
     * @return dom子节点对应实例指针
     */
    ConfigItem* initItem(QDomElement root_base);

private:
    QList<ConfigItem*> node_list;
    QDomDocument doc;
    QDomElement target_root;
    QDomElement anchor_elm;
};


class RItemsGroup : public RConfig, virtual public ItemsGroup, public ImplLoadCommon
{
public:
    RItemsGroup(QDomElement &bind_at, const QString &title_name, bool title_editable=false)
        :target(bind_at), title_store(title_name), editable_store(title_editable)
    {
        clearStateAndResetBase(bind_at.ownerDocument(), bind_at);

        target.setTagName("items-group");
        target.setAttribute("title", title_name);
        target.setAttribute("title-editable", title_editable?"true":"false");
    }
    virtual ~RItemsGroup() override = default;

    // ConfigItem interface
public:
    virtual TypeMask ItemType() const override{
        return ITEMS_GROUP;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="items-group";
    }
    virtual void loadFrom(QDomElement &elm) override{
        clearStateAndResetBase(elm.ownerDocument(), elm);

        target = elm;
        title_store = elm.attribute("title");
        editable_store = elm.attribute("title-editable")=="true";
    }
    virtual QDomElement innerDomNode() override{
        return target;
    }

    // ItemsGroup interface
public:
    virtual bool isTitleEditable() const override{
        return editable_store;
    }
    virtual void resetTitle(const QString &title_name) override{
        if(editable_store){
            title_store = title_name;
            target.setAttribute("title", title_name);
        }
    }
    virtual bool resetTemplateAndRelaseTargetIns(ConfigItem *t) override{
        if(!containsChildNode(t))
            return false;

        if(target.elementsByTagName("template").size()==0){
            target.appendChild(target.ownerDocument().createElement("template"));
        }

        auto tmpnode = target.elementsByTagName("template").at(0).toElement();
        auto list = tmpnode.childNodes();
        for (int ind=0; ind<list.size(); ++ind) {
            tmpnode.removeChild(list.at(ind));
        }

        tmpnode.appendChild(dynamic_cast<ImplLoadCommon*>(t)->innerDomNode());
        releaseChildInstance(t);

        return true;
    }
    virtual bool isItemEditable() const override{
        if(target.elementsByTagName("template").size()==0)
            return false;

        return true;
    }
    virtual ConfigItem *newTemplateElement() override{
        if(!isItemEditable())
            return nullptr;

        auto target_dom = target.elementsByTagName("template").at(0);
        auto children = target_dom.childNodes();
        if(children.size()==0)
            return nullptr;

        auto new_target = children.at(0).cloneNode();
        target.appendChild(new_target);
        return initItem(new_target.toElement());
    }
    virtual void removeItemAt(int index) override{
        if(RConfig::itemsCount() <= index)
            return;

        auto it_ins = RConfig::getItem(index);
        auto it_2 = dynamic_cast<ImplLoadCommon*>(it_ins);
        target.removeChild(it_2->innerDomNode());
        releaseChildInstance(it_ins);
    }


private:
    QDomElement target;
    QString title_store;
    bool editable_store;
};

class ROptionalGroup : public RConfig, virtual public OptionalGroup, public ImplLoadCommon
{
public:
    ROptionalGroup(QDomElement &bind_at, const QString &title_name, bool enable=true)
        :target(bind_at),title_store(title_name),enable_store(enable)
    {
        clearStateAndResetBase(bind_at.ownerDocument(), bind_at);

        auto list = bind_at.childNodes();
        for (int i=0; i<list.count(); ++i) {
            bind_at.removeChild(list.at(i));
        }

        target.setTagName("optional-group");
        target.setAttribute("title", title_name);
        target.setAttribute("enable", enable?"true":"false");
    }
    virtual ~ROptionalGroup() override = default;

    // ConfigItem interface
public:
    virtual TypeMask ItemType() const override{
        return OPTIONAL_GROUP;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="optional-group";
    }
    virtual void loadFrom(QDomElement &elm) override{
        clearStateAndResetBase(elm.ownerDocument(), elm);

        target = elm;
        title_store = elm.attribute("title");
        enable_store = elm.attribute("enable")=="true";
    }
    virtual QDomElement innerDomNode() override{
        return target;
    }

    // OptionalGroup interface
public:
    virtual bool isEnable() override{
        return enable_store;
    }
    virtual void resetEnable(bool value) override{
        enable_store = value;
        target.setAttribute("enable", value?"true":"false");
    }


private:
    QDomElement target;
    QString title_store;
    bool enable_store;
};

class RItemsSet : public RConfig, public ItemsSet, public ImplLoadCommon
{
public:
    RItemsSet(QDomElement &bind_at, const QString &key, const QString &title)
        :target(bind_at), key_store(key), title_store(title)
    {
        clearStateAndResetBase(bind_at.ownerDocument(), bind_at);

        target.setTagName("items-set");
        target.setAttribute("key", key_store);
        target.setAttribute("title", title_store);
    }
    virtual ~RItemsSet() override = default;

    // ConfigItem interface
public:
    virtual TypeMask ItemType() const override{
        return ITEMS_SET;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="items-set";
    }
    virtual void loadFrom(QDomElement &elm) override{
        clearStateAndResetBase(elm.ownerDocument(), elm);

        target = elm;
        key_store = elm.attribute("key");
        title_store = elm.attribute("title");
    }
    virtual QDomElement innerDomNode() override{
        return target;
    }

    // ItemsSet interface
public:
    virtual QString bindKey() const override{
        return title_store;
    }

private:
    QDomElement target;
    QString key_store;
    QString title_store;
};

class RSetStack : public SetStack, public ImplLoadCommon
{
public:
    RSetStack(QDomElement &bind_at, const QString &title_name)
        :target(bind_at), title_store(title_name)
    {
        target.setTagName("set-stack");
        target.setAttribute("actived-set", "");
        target.setAttribute("title", title_name);
    }
    virtual ~RSetStack() override{
        for (auto it:children) {
            delete it;
        }
        children.clear();
    }

    // Group interface
public:
    virtual void initChildren() override{
        auto list = target.childNodes();
        auto elm2 = target.ownerDocument().createElement("elm-empty");

        for (auto index=0; index<list.size(); ++index) {
            auto node = list.at(index).toElement();

            if(node.tagName()=="items-set"){
                auto it = new RItemsSet(elm2, "noinit", "noinit");
                it->loadFrom(node);
                children.append(it);
                it->initChildren();
            }
        }
    }
    virtual int itemsCount() const override{
        return children.size();
    }
    virtual ConfigItem *getItem(int index) const override{
        return children.at(index);
    }

    // ConfigItem interface
public:
    virtual QString configTitle() const override{
        return title_store;
    }
    virtual TypeMask ItemType() const override{
        return SET_STACK;
    }

    // ImplLoadCommon interface
public:
    virtual bool testPass(const QDomElement &elm) override{
        return elm.tagName()=="set-stack";
    }
    virtual void loadFrom(QDomElement &elm) override{
        for (auto it:children) {
            delete it;
        }
        children.clear();

        target = elm;
        title_store = elm.attribute("title");
        actived_key_store = elm.attribute("actived-set");
    }
    virtual QDomElement innerDomNode() override{
        return target;
    }

    // SetStack interface
public:
    virtual ItemsSet *newItemsSet(const QString &key, const QString &title) override{
        for (auto it:children) {
            if(it->bindKey()==key)
                return nullptr;
        }

        auto elm = target.ownerDocument().createElement("elm");
        target.appendChild(elm);
        auto itins = new RItemsSet(elm, key, title);
        children.append(itins);

        return itins;
    }
    virtual QList<QString> allKeys() override{
        QList<QString> list;
        for (auto it:children) {
            list << it->bindKey();
        }
        return list;
    }
    virtual QString activedKey() override{
        return actived_key_store;
    }

    virtual void setActivedKey(const QString &key) override{
        actived_key_store = key;
        target.setAttribute("actived-set", key);
    }

    virtual ItemsSet *getItemsSet(const QString &key) override{
        for (auto it:children) {
            if(it->bindKey()==key)
                return it;
        }

        return nullptr;
    }

private:
    QDomElement target;
    QString title_store;
    QString actived_key_store;
    QList<ItemsSet*> children;
};


// implemention segement=============================================================================

Config *Config::genNewInstance(const QString &content, QString *error_out)
{
    return new RConfig(content, error_out);
}


RConfig::RConfig(const QString str_content, QString *error_out)
{
    if(str_content != ""){
        QString msg;int row,col;
        if(!doc.setContent(str_content,&msg, &row, &col)){
            if(error_out){
                (*error_out) = QString("error-msg<<type:%1|line:%2|column:%3>>").arg(msg).arg(row).arg(col);
            }
        }
        target_root = doc.documentElement();
    }
    else {
        doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\""));
        target_root = doc.createElement("config-list");
        doc.appendChild(target_root);
        target_root.setAttribute("pversion", "1.0");
    }
    clearStateAndResetBase(doc, target_root);
}

RConfig::~RConfig(){
    while (node_list.count()) {
        releaseChildInstance(node_list.at(0));
    }
}

QString RConfig::toString() const
{
    return doc.toString(4);
}

bool RConfig::containsChildNode(ConfigItem *node)
{
    return node_list.contains(node);
}

void RConfig::releaseChildInstance(ConfigItem *node)
{
    if(node_list.contains(node)){
        node_list.removeOne(node);
        delete node;
    }
}

QString RConfig::configTitle() const
{
    return target_root.attribute("title", "no-title");
}

ConfigItem::TypeMask RConfig::ItemType() const
{
    return ROOT_ELM;
}

void RConfig::initChildren()
{
    while (node_list.count()) {
        releaseChildInstance(node_list.at(0));
    }

    auto list = target_root.childNodes();
    for (int index=0; index<list.size(); ++index) {
        auto elm = list.at(index).toElement();
        initItem(elm);
    }
}

int RConfig::itemsCount() const
{
    return node_list.size();
}

ConfigItem *RConfig::getItem(int index) const
{
    return node_list.at(index);
}

BoolIndicator *RConfig::appendBoolIndicator(const QString &name, bool default_value)
{
    auto node = doc.createElement("elm");
    auto nins = new RBoolIndicator(node, name);
    nins->reset(default_value);

    if(anchor_elm.isNull()){
        target_root.appendChild(node);
    }else if(nins->testPass(anchor_elm)){
        nins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(node, anchor_elm);
    }

    node_list.append(nins);
    return nins;
}

ColorInput *RConfig::appendColorInput(const QString &name)
{
    auto dom = doc.createElement("elm");
    auto ins = new RColorInput(dom, name);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
    target_root.insertBefore(dom,anchor_elm);
}

    node_list.append(ins);
    return ins;
}

FontConfig *RConfig::appendFontConfig(const QString &name, const QString &family_name)
{
    auto dom = doc.createElement("elm");
    auto ins = new RFontConfig(dom, name, family_name);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

ItemsGroup *RConfig::appendItemsGroup(const QString &name, bool title_editable)
{
    auto dom = doc.createElement("elm");
    auto ins = new RItemsGroup(dom, name, title_editable);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

NumberInput *RConfig::appendNumberInput(const QString &name, NumberInput::Type type)
{
    auto node = doc.createElement("elm");
    auto nins = new RNumberInput(node, name, type);

    if(anchor_elm.isNull()){
        target_root.appendChild(node);
    }else if(nins->testPass(anchor_elm)){
        nins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(node, anchor_elm);
    }

    node_list.append(nins);
    return nins;
}

PathInput *RConfig::appendPathInput(const QString &name, PathInput::Type type)
{
    auto dom = doc.createElement("elm");
    auto ins = new RPathInput(dom, type, name);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

SelectList *RConfig::appendSelectList(const QString &name)
{
    auto dom = doc.createElement("elm");
    auto ins = new RSelectList(dom, name);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

TextInput *RConfig::appendTextInput(const QString &name, const QString &pre_content)
{
    auto dom = doc.createElement("elm");
    auto ins = new RTextInput(dom, name, pre_content);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

SetStack *RConfig::appendSetStack(const QString &name)
{
    auto dom = doc.createElement("elm");
    auto ins = new RSetStack(dom, name);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

OptionalGroup *RConfig::appendOptionalGroup(const QString title_name, bool enable)
{
    auto dom = doc.createElement("elm");
    auto ins = new ROptionalGroup(dom, title_name, enable);

    if(anchor_elm.isNull()){
        target_root.appendChild(dom);
    }else if(ins->testPass(anchor_elm)){
        ins->loadFrom(anchor_elm);
        anchor_elm = anchor_elm.nextSiblingElement();
    }else {
        target_root.insertBefore(dom, anchor_elm);
    }

    node_list.append(ins);
    return ins;
}

void RConfig::clearStateAndResetBase(QDomDocument doc, QDomElement &tag_elm)
{
    while (node_list.count()) {
        releaseChildInstance(node_list.at(0));
    }

    this->doc = doc;
    this->target_root = tag_elm;
    auto x = target_root.childNodes();
    if(x.size())
        this->anchor_elm = x.at(0).toElement();
}

ConfigItem *RConfig::initItem(QDomElement root_base)
{
    auto temp_elm = doc.createElement("invalid");

    RBoolIndicator a(temp_elm, "noname");
    if(a.testPass(root_base)){
        auto item = new RBoolIndicator(temp_elm, "noname");
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    RColorInput b(temp_elm, "noname");
    if(b.testPass(root_base)){
        auto item = new RColorInput(temp_elm, "noname");
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    RFontConfig c(temp_elm, "noname", "noname");
    if(c.testPass(root_base)){
        auto item = new RFontConfig(temp_elm, "noname", "nofont");
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    RItemsGroup d(temp_elm, "noname");
    if(d.testPass(root_base)){
        auto item = new RItemsGroup(temp_elm, "noname");
        item->loadFrom(root_base);
        item->initChildren();
        node_list.append(item);
        return item;
    }

    RNumberInput e(temp_elm, "noname", RNumberInput::INTEGER);
    if(e.testPass(root_base)){
        auto item = new RNumberInput(temp_elm, "noname", RNumberInput::INTEGER);
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    ROptionalGroup f(temp_elm, "noname");
    if(f.testPass(root_base)){
        auto item = new ROptionalGroup(temp_elm, "noname");
        item->loadFrom(root_base);
        item->initChildren();
        node_list.append(item);
        return item;
    }

    RPathInput g(temp_elm, PathInput::FILE, "noname");
    if(g.testPass(root_base)){
        auto item = new RPathInput(temp_elm, PathInput::FILE, "noname");
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    RSelectList h(temp_elm, "notitle");
    if(h.testPass(root_base)){
        auto item = new RSelectList(temp_elm, "noname");
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    RSetStack i(temp_elm, "noname");
    if(i.testPass(root_base)){
        auto item = new RSetStack(temp_elm, "noname");
        item->loadFrom(root_base);
        item->initChildren();
        node_list.append(item);
        return item;
    }

    RTextInput j(temp_elm, "noname", "nocontent");
    if(j.testPass(root_base)){
        auto item = new RTextInput(temp_elm, "noname", "nocontent");
        item->loadFrom(root_base);
        node_list.append(item);
        return item;
    }

    return nullptr;
}


RBoolIndicator::RBoolIndicator(QDomElement &bind_at, const QString &name)
    :elm_store(bind_at), name_store(name), value_store(false)
{
    bind_at.setTagName("bool-indicator");
    bind_at.setAttribute("value", "false");

    auto ns = bind_at.childNodes();
    for (int i=0; i<ns.count(); ++i) {
        bind_at.removeChild(ns.at(i));
    }

    bind_at.appendChild(bind_at.ownerDocument().createTextNode(name));
}

QString RBoolIndicator::configTitle() const{
    return name_store;
}

ConfigItem::TypeMask RBoolIndicator::ItemType() const{
    return TypeMask::BOOL_INDICATOR;
}

bool RBoolIndicator::value() const{
    return value_store;
}

void RBoolIndicator::reset(bool value){
    this->value_store = value;
    elm_store.setAttribute("value", value?"true":"false");
}

bool RBoolIndicator::testPass(const QDomElement &elm){
    return elm.tagName() == "bool-indicator";
}

void RBoolIndicator::loadFrom(QDomElement &elm){
    elm_store = elm;
    name_store = elm.childNodes().at(0).toText().data();
    value_store = elm.attribute("value", "false") == "true";
}

QDomElement RBoolIndicator::innerDomNode(){
    return elm_store;
}
