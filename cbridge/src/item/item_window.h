//
// Created by sulin on 2017/9/29.
//

#ifndef UILIB_WINDOWITEM_H
#define UILIB_WINDOWITEM_H


#include <QQuickItem>
#include <QWindow>
#include <QQuickWindow>

/**
 * @brief 窗口标题栏控件, 会被用于原生拖拽等
 */
class WindowTitleItem : public QQuickItem {
    Q_OBJECT
    
public:
    explicit WindowTitleItem(QQuickItem *parent = Q_NULLPTR);
       
    ~WindowTitleItem();

    void mouseDoubleClickEvent(QMouseEvent *event);
  
    void mousePressEvent(QMouseEvent *event);
};

/**
 * @brief 顶级窗口控件, 采用native样式来装饰
 */
class WindowItem : public QQuickWindow {
    Q_OBJECT

private:
    qreal oldRatio; // 当前窗口当前采用的像素密度, MAC环境需要在像素密度变化时手动刷新窗口
    WindowTitleItem *title;
    
    int winBorderWidth; // Win平台的边框宽度
    void *macEventMonitor; // MAC平台注册的事件监听器
    void *macLastEvent; // MAC平台最近的事件

public:
    
    explicit WindowItem(QWindow *parent = nullptr);

    ~WindowItem();
    
    /**
     * 开始窗口拖拽, 无需指定Event, 内部直接采用最近的NativeEvent.
     * 可用于OSX环境
     */
    void startDrag();
    
    /**
     * 挂载标题栏, 如果重复操作则覆盖旧的标题栏
     * @param item 标题栏元素
     */
    void setTitleBar(WindowTitleItem *item);
    
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


#endif //UILIB_WINDOWITEM_H

