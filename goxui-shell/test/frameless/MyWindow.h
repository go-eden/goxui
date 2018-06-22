//
// Created by sulin on 2018/1/14.
//

#ifndef CLIENT_SHELL_MYWINDOW_H
#define CLIENT_SHELL_MYWINDOW_H


#include <QQuickWindow>

class MyWindow : public QQuickWindow {
Q_OBJECT

private:
    qreal oldRatio;

public:
    explicit MyWindow(QWindow *parent = nullptr);

    /**
     * 响应QT事件, 某些系统在适应分辨率变化时需要监听此消息
     * @return 是否已处理
     */
    bool event(QEvent *event) override;

    /**
     * 响应系统原生事件, 某些系统需要处理native消息
     * @return 是否已处理
     */
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

};

#endif //CLIENT_SHELL_MYWINDOW_H
