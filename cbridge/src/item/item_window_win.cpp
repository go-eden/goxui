#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <objidl.h>
#include <gdiplus.h>
#include <GdiPlusColor.h>

#pragma comment (lib, "Dwmapi.lib")
#pragma comment (lib, "user32.lib")

#include "item_window.h"

// 计算指定控件内部占用鼠标的子控件数量
int useMouseNum(QQuickItem *item, QPointF &gpos) {
    if (item == nullptr || !item->isEnabled() || !item->contains(item->mapFromGlobal(gpos))) {
        return 0;
    }
    int num = item->acceptedMouseButtons() == Qt::LeftButton ? 0 : 1;
    QList<QQuickItem *>	children = item->childItems();
    for (int i=0; i<children.size() && num <= 1; i++) {
        num += useMouseNum(children[i], gpos);
    }
    return num;
}

// 初始化标题栏, 将自己挂载在窗口之上
WindowTitleItem::WindowTitleItem(QQuickItem *parent) : QQuickItem(parent){
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

// 响应双击, 什么也不需要做
void WindowTitleItem::mouseDoubleClickEvent(QMouseEvent *e) {
    QQuickItem::mouseDoubleClickEvent(e);
}

// 响应鼠标按下, 什么也不需要做
void WindowTitleItem::mousePressEvent(QMouseEvent *event) {
    QQuickItem::mousePressEvent(event);
}

// 初始化窗口
WindowItem::WindowItem(QWindow *parent) : QQuickWindow(parent) {
    this->title = nullptr;
    this->winBorderWidth = 5;

    HWND hwnd = (HWND) this->winId();

    LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyle = (WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
    SetWindowLong(hwnd, GWL_STYLE, lStyle);

    LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
}

// 窗口释放
WindowItem::~WindowItem() {
}

// 开始拖拽
void WindowItem::startDrag() {
}

// 挂载标题栏, 并根据标题栏来初始化 NSTitlebarAccessoryViewController
void WindowItem::setTitleBar(WindowTitleItem *item) {
    this->title = item;
}

// 监听事件
bool WindowItem::event(QEvent *e) {
    if (e->type() == QEvent::FocusIn || e->type() == QEvent::FocusOut) {
        QRegion region(0, 0, width(), height());
        QQuickWindow::exposeEvent(new QExposeEvent(region));
    }
//    if (e->type() != QEvent::MouseMove) {
//        qDebug() << e;
//    }
    return QQuickWindow::event(e);
}

// 原生事件的处理, mac貌似不支持
bool WindowItem::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    MSG* msg = (MSG *)message;

//    qDebug() << msg->message;
    switch (msg->message) {
    case WM_NCCALCSIZE: // 窗口重绘时忽略边框、标题栏
        *result = 0;
        return true;

    case WM_NCHITTEST:{ // 判断当前坐标是标题栏、边框、系统按钮等
        *result = 0;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        // 判断是否在边框上, 支持resize
        LONG border_width = this->winBorderWidth * devicePixelRatio();
        if(border_width > 0) {
            bool resizeWidth = minimumWidth() != maximumWidth();
            bool resizeHeight = minimumHeight() != maximumHeight();
            bool hitLeft = resizeWidth && x >= winrect.left && x < winrect.left + border_width;
            bool hitRight = resizeWidth && x < winrect.right && x >= winrect.right - border_width;
            bool hitTop = resizeHeight && y >= winrect.top && y < winrect.top + border_width;
            bool hitBottom = resizeHeight && y < winrect.bottom && y >= winrect.bottom - border_width;

            if (hitBottom && hitLeft)
                *result = HTBOTTOMLEFT;
            else if (hitBottom && hitRight)
                *result = HTBOTTOMRIGHT;
            else if (hitTop && hitLeft)
                *result = HTTOPLEFT;
            else if (hitTop && hitRight)
                *result = HTTOPRIGHT;
            else if (hitLeft)
                *result = HTLEFT;
            else if (hitRight)
                *result = HTRIGHT;
            else if (hitBottom)
                *result = HTBOTTOM;
            else if (hitTop)
                *result = HTTOP;
        }

        // 判断是否是标题栏
        QPointF gpos = QPointF(x/devicePixelRatio(), y/devicePixelRatio());
        if (0 == *result && useMouseNum(title, gpos) == 1) {
            *result = HTCAPTION;
        }

        return 0 != *result;
    }

    default:
        return QQuickWindow::nativeEvent(eventType, message, result);
    }
}
