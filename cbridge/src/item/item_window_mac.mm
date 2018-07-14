//
// Created by sulin on 2017/9/29.
//

#include <Cocoa/Cocoa.h>
#include <QtConcurrent>
#include "item_window.h"

// 调整窗口的标题栏, 窗口初始化、设置标题栏、大小调整等事件需要进行这个操作
void adjustTitle(WindowItem *w, WindowTitleItem *titleItem) {
    NSView *view = (NSView *) w->winId();
    if (view == nullptr || view.window == nullptr || titleItem == nullptr) {
        return;
    }
    NSWindow *win = view.window;
    static NSWindowButton arr[]{NSWindowCloseButton, NSWindowMiniaturizeButton, NSWindowZoomButton, NSWindowFullScreenButton};
    for (int i=0, idx=0; i<4; i++) {
        NSButton *btn = [win standardWindowButton:arr[i]];
        if (btn == nullptr) {
            continue;
        }
        NSRect frame = btn.frame;
        frame.origin.y = titleItem->height() / 2 - NSHeight(frame) / 2;
        frame.origin.x = frame.origin.y + idx * (NSWidth(frame) + 6);
        [btn setFrame:frame];
        idx ++;
    }
}

// 初始化标题栏, 将自己挂载在窗口之上
WindowTitleItem::WindowTitleItem(QQuickItem *parent) : QQuickItem(parent) {
    this->setAcceptedMouseButtons(Qt::LeftButton);
    QObject::connect(this, &QQuickItem::windowChanged, [=](QQuickWindow *w) {
        if (WindowItem* window = dynamic_cast<WindowItem*>(w)) {
            window->setTitleBar(this);
        }
    });
}

// 标题栏卸载
WindowTitleItem::~WindowTitleItem() {
    if (this->window() == nullptr) {
        return;
    }
    if (WindowItem* window = dynamic_cast<WindowItem*>(this->window())) {
        window->setTitleBar(nullptr);
    }
}

// 响应双击, 改变窗口大小
void WindowTitleItem::mouseDoubleClickEvent(QMouseEvent *){
    QWindow *win = this->window();
    if (win == nullptr) {
        return;
    }
    if (win->visibility() == QWindow::Maximized) {
        win->showNormal();
    } else if (win->visibility() != QWindow::FullScreen) {
        win->showMaximized();
    }
}

// 响应鼠标按下, 自动开始窗口的拖拽
void WindowTitleItem::mousePressEvent(QMouseEvent *event){
    if (WindowItem* window = dynamic_cast<WindowItem*>(this->window())) {
        window->startDrag();
    } else {
        QQuickItem::mousePressEvent(event); // 不让子元素感知
    }
}

// 初始化窗口, 并且注册MouseDown事件监听
WindowItem::WindowItem(QWindow *parent) : QQuickWindow(parent) {
    this->oldRatio = 0;
    this->title = nullptr;
    this->macLastEvent = nullptr;
    this->macEventMonitor = nullptr;
    this->setFlag(Qt::WindowFullscreenButtonHint, true); // 全屏按钮
    // 初始化窗口, 这样做还不够
    NSView *view = (NSView *)this->winId();
    NSWindow *window = view.window;
    window.styleMask |= NSFullSizeContentViewWindowMask | NSTitledWindowMask;
    window.titleVisibility = NSWindowTitleHidden;
    window.titlebarAppearsTransparent = true;
    // 监听鼠标拖拽并转发给NSWindow
    this->macEventMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSLeftMouseDownMask handler:^(NSEvent *e) {
        this->macLastEvent = e;
        return e;
    }];
    adjustTitle(this, title);
}

// 窗口释放时, 需要删除事件监听
WindowItem::~WindowItem() {
    if (this->macEventMonitor != nullptr) {
        id tmp = (id)this->macEventMonitor;
        [NSEvent removeMonitor:tmp];
    }
}

// 开始拖拽
void WindowItem::startDrag() {
    NSView *view = (NSView *)this->winId();
    if (!this->macLastEvent || !view || !view.window) {
        return;
    }
    NSEvent *e = (NSEvent *)this->macLastEvent; // 内存是否会被提前释放?
    [view.window performWindowDragWithEvent:e];
}

// 挂载标题栏, 并根据标题栏来初始化 NSTitlebarAccessoryViewController
void WindowItem::setTitleBar(WindowTitleItem *item) {
    NSView *mainView = (NSView *)this->winId();
    if (mainView == nullptr || mainView.window == nullptr) {
        return;
    }
    if (this->title) {
        [mainView.window removeTitlebarAccessoryViewControllerAtIndex:0]; // 卸载标题栏
    }
    this->title = item;
    NSView *view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 10, item->height() - 22)]; // 减去默认标题栏高度22
    NSTitlebarAccessoryViewController* titleCtl = [NSTitlebarAccessoryViewController new];
    titleCtl.view = view;
    titleCtl.fullScreenMinHeight = 0;
    [mainView.window addTitlebarAccessoryViewController:titleCtl];
    adjustTitle(this, title);
}

// 监听事件
bool WindowItem::event(QEvent *event) {
    // 释放之前缓存的MouseDown事件缓存
    if(event->type() != QEvent::UpdateRequest && event->type() != QEvent::Timer && event->type() != QEvent::MouseMove) {
        qDebug() << "event: " << event;
    }
    if (event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::FocusOut) {
        this->macLastEvent = nullptr;
    }
    NSView *view = (NSView *)this->winId();
    qreal newRatio = this->devicePixelRatio();
    if (view != nullptr && view.layer != nil && oldRatio != newRatio) {
        qDebug() << "devicePixelRatio changed, force flush" << oldRatio << newRatio;
        [view.layer setContentsScale: newRatio]; // 主动刷新ratio, 如果失败则下次再刷新
        QRegion region(0, 0, width(), height());
        QWindow::exposeEvent(new QExposeEvent(region));
        oldRatio = newRatio;
    }
    if (event->type() == QEvent::Resize) {
        adjustTitle(this, title);
    }
    if (event->type() == QEvent::WindowStateChange) {
        NSWindow *window = view.window;
        window.styleMask |= NSFullSizeContentViewWindowMask | NSTitledWindowMask;
        window.titleVisibility = NSWindowTitleHidden;
        window.titlebarAppearsTransparent = true;
        adjustTitle(this, title);
    }
    return QQuickWindow::event(event);
}

// 原生事件的处理, mac貌似不支持
bool WindowItem::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    return QQuickWindow::nativeEvent(eventType, message, result);
}
