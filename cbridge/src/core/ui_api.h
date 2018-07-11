//
// Created by sulin on 2018/1/12.
//

#ifndef CLIENT_SHELL_UI_API_H
#define CLIENT_SHELL_UI_API_H

#include <QVariant>
#include <QVariantMap>
#include <QObject>
#include <QDateTime>
#include <QQmlApplicationEngine>

/**
 * 向UI暴露的系统工具API
 */
class UIApi : public QObject {
Q_OBJECT

    QQmlApplicationEngine *engine;

public:

    explicit UIApi(QQmlApplicationEngine *engine);

    /**
     * 清除QML组件缓存, 可用于强制刷新Loader
     */
    Q_INVOKABLE void clearComponentCache();

    /**
     * 根据指定data调用打印, data必须是Image
     */
    Q_INVOKABLE void print(QVariant data);
    
    /**
     * 设置剪切板的内容
     */
    Q_INVOKABLE void setClipboard(QString text);
    
    /**
     * exec保存文件的对话框, 此函数阻塞直至对话框结束
     */
    Q_INVOKABLE QVariantMap execSaveFileDialog(QString defaultName, QStringList nameFilters);
    
};


#endif //CLIENT_SHELL_UI_API_H
