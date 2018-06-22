//
// Created by sulin on 2018/1/14.
//
#include <QDebug>
#include <Cocoa/Cocoa.h>
#include <QCoreApplication>
#include <QQuickItem>
#include "frameless.h"
#include "MyWindow.h"


MyWindow::MyWindow(QWindow *parent) : QQuickWindow(parent) {
    frameless_init(this);
    oldRatio = 0;
    qDebug() << "init...";
}

bool MyWindow::event(QEvent *event) {
    qreal newRatio = devicePixelRatio();
    if (oldRatio > 0 && oldRatio < 100 && oldRatio != newRatio) {
        qDebug() << "devicePixelRatio changed, force flush" << oldRatio << newRatio;
        frameless_flush_ratio(this, newRatio); // 主动刷新ratio
        QRegion region(0, 0, width(), height());
        QWindow::exposeEvent(new QExposeEvent(region)); // 必须通过QWindow来expose, QQuickWindow会吞掉它
    }
//    qDebug() << "what??? " << event->type();
    if (event->type() == QEvent::MouseButtonPress) {
        if (this->mouseGrabberItem()) {
            qDebug() << "mouseGrabberItem: " << this->mouseGrabberItem()->objectName();
        } else {
            qDebug() << "mouseGrabberItem: nil";
        }
    }
    oldRatio = newRatio;
    return QQuickWindow::event(event);
}

bool MyWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    qDebug() << "what??? " << eventType;
    return QQuickWindow::nativeEvent(eventType, message, result);
}
