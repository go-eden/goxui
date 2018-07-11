//
// Created by sulin on 2017/11/21.
//

#include <QQmlEngine>
#include "ui_property.h"

PropertyNode::PropertyNode(QString &name, PropertyNode *parent) : QObject(parent) {
    this->nodeName = name;
    this->metadata = nullptr;
    this->parentNode = parent;
    this->fields = QMap<QString, Field>();
    this->methods = QMap<QString, Method>();
    this->subNodes = QMap<QString, PropertyNode *>();
}

const QMetaObject *PropertyNode::metaObject() const {
    return this->metadata;
}

// 处理属性回调的关键函数
int PropertyNode::qt_metacall(QMetaObject::Call call, int v, void **args) {
    if (call == QMetaObject::ReadProperty) {
        void *ret = args[0];
        QMetaProperty prop = metaObject()->property(v);
        if (subNodes.contains(prop.name())) {
            *reinterpret_cast< PropertyNode **>(ret) = subNodes[prop.name()];
        } else if (fields.contains(prop.name())) {
            fields[prop.name()].reader(ret);
        } else {
            qWarning() << "invalid ReadProperty operate: " << v;
        }
    } else if (call == QMetaObject::WriteProperty) {
        void *arg = args[0];
        QMetaProperty prop = metaObject()->property(v);
        if (fields.contains(prop.name())) {
            fields[prop.name()].writer(arg);
        } else {
            qWarning() << "invalid WriteProperty operate: " << v;
        }
    } else if (call == QMetaObject::InvokeMetaMethod) {
        QMetaMethod prop = metaObject()->method(v);
        if (methods.contains(prop.name())) {
            auto method = methods[prop.name()];
            QVariantList argList;
            for (int i = 1; i <= method.argNum; i++) {
                argList.append((*static_cast<QJSValue *>(args[i])).toVariant());
            }
            if (prop.parameterCount() == method.argNum) {
                QVariant ret;
                method.callback(ret, argList);
                *reinterpret_cast< QVariant *>(args[0]) = ret;
            } else {
                auto callback = static_cast<QJSValue *>(args[method.argNum + 1]);
                if (callback != nullptr && callback->isCallable()) {
                    QtConcurrent::run(method, &Method::asyncInvoke, argList, new QJSValue(*callback));
                } else {
                    qWarning() << "invalid InvokeMetaMethod arguments.";
                }
            }
        } else {
            qWarning() << "invalid InvokeMetaMethod operate: " << v;
        }
    } else {
        qWarning() << "invalid metacall " << int(call) << "," << v;
    }
    return 0;
}

// 这个函数用于类型转换, 其实没有用
void *PropertyNode::qt_metacast(const char *str){
    return QObject::qt_metacast(str);
}

// 通知属性更新, val其实没有用
bool PropertyNode::notifyProperty(QString &name, QVariant &val) {
    QString rest;
    if (name.contains(".")) {
        rest = name.midRef(name.indexOf(".") + 1).toString();
        name = name.midRef(0, name.indexOf(".")).toString();
    }
    if (rest.size() > 0) {
        if (!subNodes.contains(name)) {
            qWarning() << "key invalid: " << name;
            return false;
        }
        return subNodes[name]->notifyProperty(rest, val);
    }
    if (!fields.contains(name)) {
        qWarning() << "key invalid: " << name;
        return false;
    }
    auto meta = metaObject();
    int propIndex = meta->indexOfProperty(qPrintable(name));
    if (propIndex == -1) {
        qWarning() << "notify-property-change-FAIL, property not exist: ", name;
        return false;
    }
    QMetaProperty prop = meta->property(propIndex);
    if (prop.notifySignalIndex() == -1) {
        qWarning() << "notify-property-change-FAIL, property hasn't notify: ", name;
        return false;
    }
    void *args[] = {nullptr, &val}; // out, in
    QMetaObject::activate(this, meta, prop.notifySignalIndex() - meta->methodOffset(), args);
    return true;
}

// 新增属性
bool PropertyNode::addProperty(QString &name, Type type, void *v) {
    if (metadata != nullptr) {
        qWarning() << "Property freeze";
        return false;
    }
    QString rest;
    if (name.contains(".")) {
        rest = name.midRef(name.indexOf(".") + 1).toString();
        name = name.midRef(0, name.indexOf(".")).toString();
    }
    if (fields.contains(name) || methods.contains(name)) {
        qWarning() << "key existed allready: " << name;
        return false;
    }
    if (rest.size() == 0) {
        if (type == Type::FIELD) {
            auto prop = static_cast<Field *>(v);
            prop->name = name;
            fields.insert(name, *prop);
        } else if (type == Type::METHOD) {
            auto method = static_cast<Method *>(v);
            method->name = name;
            methods.insert(name, *method);
        }
        return true;
    }
    if (!subNodes.contains(name)) {
        subNodes.insert(name, new PropertyNode(name, this));
    }
    return subNodes[name]->addProperty(rest, type, v);
}

// 构建节点元数据, 不需要处理子节点
PropertyNode *PropertyNode::buildMetaData() {
    QMetaObjectBuilder builder;
    builder.setClassName("UIClassProxy");
    builder.setSuperClass(&QObject::staticMetaObject);
    for (auto field : fields) {
        auto typeName = QVariant::typeToName(field.type);
        auto signal = builder.addSignal(qPrintable(field.name + "Changed(" + typeName + ")"));
        signal.setParameterNames(QList<QByteArray>{field.name.toLocal8Bit()});
        auto propBuilder = builder.addProperty(field.name.toLocal8Bit(), typeName);
        propBuilder.setReadable(field.reader != nullptr);
        propBuilder.setWritable(field.writer != nullptr);
        propBuilder.setNotifySignal(signal);
    }
    for (auto method : methods) {
        auto name = method.name.toLocal8Bit();
        // sync
        QStringList args;
        for (int i = 0; i < method.argNum; ++i) {
            args.append("QJSValue");
        }
        QMetaMethodBuilder syncMethodBuilder = builder.addMethod(name + "(" + args.join(',').toLocal8Bit() + ")");
        syncMethodBuilder.setAccess(QMetaMethod::Public);
        syncMethodBuilder.setReturnType("QVariant");
        // async
        args.append("QJSValue");
        QMetaMethodBuilder asyncMethodBuilder = builder.addMethod(name + "(" + args.join(',').toLocal8Bit() + ")");
        asyncMethodBuilder.setAccess(QMetaMethod::Public);
        asyncMethodBuilder.setReturnType("QVariant");
    }
    for (auto node : subNodes) {
        auto propBuilder = builder.addProperty(node->nodeName.toLocal8Bit(), QVariant::typeToName(QMetaType::QObjectStar));
        propBuilder.setConstant(true); // avoid the warning of "depends on non-NOTIFYable properties"
        propBuilder.setReadable(true);
        propBuilder.setWritable(false);
        node->buildMetaData();
    }
    this->metadata = builder.toMetaObject();
    return this;
}

// 事件处理, 用于回调JS, 必须在main-event-loop中执行
bool PropertyNode::event(QEvent *event) {
    if (event->type() == QEvent::User) { // TODO If callback was deleted by GC, would this code crash?
        auto e = dynamic_cast<Event *>(event);
        auto engine = e->callback->engine();
        e->callback->call({engine->toScriptValue(*e->data)});
    }
    return QObject::event(event);
}

