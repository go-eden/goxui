//
// Created by sulin on 2018/1/12.
//

#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include <QVariant>
#include <QVariantMap>
#include <QObject>
#include <QDateTime>
#include <QQmlApplicationEngine>

/**
 * 向UI暴露的系统工具API
 */
class UISystem : public QObject {
Q_OBJECT

    QQmlApplicationEngine *engine;

public:

    explicit UISystem(QQmlApplicationEngine *engine);

    /**
     * 清除QML组件缓存, 可用于强制刷新Loader
     */
    Q_INVOKABLE void clearComponentCache();
    
    /**
     * 设置剪切板的内容
     */
    Q_INVOKABLE void setClipboard(QString text);
    
    /**
     * exec保存文件的对话框, 此函数阻塞直至对话框结束
     */
    Q_INVOKABLE QVariantMap execSaveFileDialog(QString defaultName, QStringList nameFilters);
    
};


#endif //UI_SYSTEM_H
