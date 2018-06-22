//
// Created by sulin on 2017/11/21.
//

#ifndef UILIB_PROPERTY_H
#define UILIB_PROPERTY_H

#include <QObject>
#include <private/qmetaobjectbuilder_p.h>
#include <QJSValue>
#include <QVariant>
#include <QVariantList>
#include <QtConcurrent>
#include <QEvent>
#include <utility>

using Reader = std::function<void(void *ret)>;
using Writer = std::function<void(void *arg)>;
using Callback = std::function<void(QVariant &ret, QVariantList &arg)>;

// 通过异步事件处理JS回调
class Event : public QEvent {

public:
    QVariant *data;
    QJSValue *callback;

    Event(QVariant *data, QJSValue *callback) : QEvent(QEvent::User) {
        this->data = data;
        this->callback = callback;
    }

    ~Event() override {
        delete this->data;
        delete this->callback;
    }

};

/**
 * UI属性树中的一个节点, 可以在这个节点上挂载Property、Method、subNode等
 */
class PropertyNode : public QObject {
    enum Type {
        FIELD, METHOD
    };

    // UI中暴露的属性
    struct Field {
        QString name;
        int type;
        Reader reader;
        Writer writer;
    };

    // UI中暴露的函数
    struct Method {
        QString name;
        int argNum;
        Callback callback;
        PropertyNode *eventRecever;

        void asyncInvoke(QVariantList &args, QJSValue *callback) {
            auto ret = new QVariant();
            this->callback(*ret, args);
            QCoreApplication::postEvent(eventRecever, new Event(ret, callback));
        }
    };

private:
    PropertyNode *parentNode;
    QMetaObject *metadata;
    QString nodeName;
    QByteArray nodeClassName;
    QMap<QString, Field> fields;
    QMap<QString, Method> methods;
    QMap<QString, PropertyNode *> subNodes;

public:

    /**
     * 属性节点的构造函数
     * @param name 属性树中节点的名称
     * @param parent 父节点, 可以是null
     */
    PropertyNode(QString &name, PropertyNode *parent);

    /**
     * 获取元数据
     * @return
     */
    const QMetaObject *metaObject() const override;

    /**
     * 处理回调
     * @return
     */
    int qt_metacall(QMetaObject::Call, int, void **) override;
    
    void *qt_metacast(const char *) override;

    /**
     * 构建元数据, 执行后此PropertyNode将被冻结, 不能再进行属性修改
     */
    PropertyNode *buildMetaData();

    bool event(QEvent *event) override;

public:
    /**
     * 增加属性
     * @param name 属性名
     * @param type 属性类型
     * @param reader 读回调
     * @param writer 写回调
     * @return 是否新增成功
     */
    bool addField(QString &name, int type, Reader &reader, Writer &writer) {
        auto field = Field{name, type, reader, writer};
        return this->addProperty(name, Type::FIELD, &field);
    }

    /**
     * 增加函数
     * @param name 函数名
     * @param argNum 函数入参数量
     * @return 是否新增成功
     */
    bool addMethod(QString &name, int argNum, Callback &callback) {
        auto method = Method{name, argNum, callback, this};
        return this->addProperty(name, Type::METHOD, &method);
    }

    /**
     * 通知属性更新, 使用者需要再次读取此属性的最新值
     * @param name 属性名
     * @param val 属性值
     * @return 是否通知成功
     */
    bool notifyProperty(QString &name, QVariant &val);

    QByteArray &buildClassName();
    
private:

    // 新增属性
    bool addProperty(QString &name, Type propType, void *v);


};


#endif //UILIB_PROPERTY_H
