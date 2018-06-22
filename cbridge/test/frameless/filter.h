//
// Created by sulin on 2018/1/10.
//

#ifndef CLIENT_SHELL_FILTER_H
#define CLIENT_SHELL_FILTER_H

#include <QObject>
#include <QEvent>
#include <QDebug>
#include <QQuickView>
#include "frameless.h"

class Filter : public QObject {
Q_OBJECT

private:
    qreal old;
    QQuickWindow *win;

public:
    explicit Filter(QQuickWindow *win) : QObject(nullptr) {
        this->win = win;
        this->old = 0;
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        qreal newRatio = win->devicePixelRatio();
        if (old > 0 && old < 100 && win->devicePixelRatio() != old) {
            qDebug() << "ratio changed: " << old << newRatio;
            frameless_flush_ratio(win, newRatio);
        }
        old = newRatio;
        return QObject::eventFilter(obj, event);
    }

};

#endif //CLIENT_SHELL_FILTER_H
