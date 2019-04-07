#ifndef GOXUI_P_H
#define GOXUI_P_H

#include <QVariant>
#include <QJsonDocument>
#include <QQmlApplicationEngine>

// use WebEngine optionally
#ifdef WEB
    #include <QtWebEngine/qtwebengineglobal.h>
#endif

// use SingleApplication
#ifndef QAPPLICATION_CLASS
  #define QAPPLICATION_CLASS QApplication
#endif
#include "singleapplication.h"

#include "core/ui_property.h"
#include "goxui.h"

static PropertyNode *root = nullptr;
static SingleApplication *app = nullptr;
static QQmlApplicationEngine *engine = nullptr;

// convert string data to specified type
inline void convertStrToVar(char *data, int type, QVariant &ptr) {
    QByteArray array(data);
    switch (type) {
        case UI_TYPE_VOID:
            break;
        case UI_TYPE_BOOL:
            ptr.setValue(array == "0" || array.toLower() == "true");
            break;
        case UI_TYPE_INT:
            ptr.setValue(array.toInt());
            break;
        case UI_TYPE_LONG:
            ptr.setValue(array.toLongLong());
            break;
        case UI_TYPE_DOUBLE:
            ptr.setValue(array.toDouble());
            break;
        case UI_TYPE_OBJECT:
            ptr.setValue(QJsonDocument::fromJson(array).toVariant());
            break;
        default:
            ptr.setValue(QString(array));
    }
}

// 将字符串转换为指定类型的变量, 并赋值给指定指针
inline void convertStrToPtr(char *data, int type, void *ptr) {
    QByteArray array(data);
    switch (type) {
        case UI_TYPE_BOOL:
            *reinterpret_cast< bool *>(ptr) = array == "0" || array.toLower() == "true";
            break;
        case UI_TYPE_INT:
            *reinterpret_cast< qint32 *>(ptr) = array.toInt();
            break;
        case UI_TYPE_LONG:
            *reinterpret_cast< qint64 *>(ptr) = array.toLongLong();
            break;
        case UI_TYPE_DOUBLE:
            *reinterpret_cast< double *>(ptr) = array.toDouble();
            break;
        case UI_TYPE_OBJECT:
            *reinterpret_cast< QVariant *>(ptr) = QJsonDocument::fromJson(array).toVariant();
            break;
        default:
            *reinterpret_cast< QString *>(ptr) = QString(data);
    }
}

// 将指针按照指定类型格式化为字符串
inline QByteArray convertPtrToStr(void *arg, int type) {
    QByteArray result;
    switch (type) {
        case UI_TYPE_BOOL:
            result.setNum(*reinterpret_cast< bool *>(arg));
            break;
        case UI_TYPE_INT:
            result.setNum(*reinterpret_cast< qint32 *>(arg));
            break;
        case UI_TYPE_LONG:
            result.setNum(*reinterpret_cast< qint64 *>(arg));
            break;
        case UI_TYPE_DOUBLE:
            result.setNum(*reinterpret_cast< double *>(arg));
            break;
        case UI_TYPE_OBJECT:
            result.append(QJsonDocument::fromVariant(*reinterpret_cast< QVariant *>(arg)).toJson(QJsonDocument::Compact).data());
            break;
        default:
            result.append(*reinterpret_cast< QString *>(arg));
    }
    return result;
}

#endif // GOXUI_P_H
