//
// Created by sulin on 2017/9/30.
//

#include "item_title.h"

TitleItem::TitleItem(QQuickItem *parent) : QQuickItem(parent) {
    setAcceptedMouseButtons(Qt::LeftButton);
    this->setAcceptHoverEvents(true);
}

bool TitleItem::event(QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);
        qDebug() << "event: " << event->type() << this->childAt(e->x(), e->y());
    }
    return QQuickItem::event(event);
}

//
//void TitleItem::mousePressEvent(QMouseEvent *e) {
//    flag = true;
//    fromX = e->globalX();
//    fromY = e->globalY();
//    fromWinX = window()->x();
//    fromWinY = window()->y();
//    setCursor(Qt::SizeAllCursor);
//}
//
//void TitleItem::mouseMoveEvent(QMouseEvent *e) {
//    if (flag) {
//        auto x = fromWinX + e->globalX() - fromX;
//        auto y = fromWinY + e->globalY() - fromY;
//        window()->setPosition(x, y);
//    }
//    QQuickItem::mouseMoveEvent(e);
//}
//
//void TitleItem::mouseReleaseEvent(QMouseEvent *e) {
//    if (flag) {
//        flag = false;
//        unsetCursor();
//    }
//    QQuickItem::mouseReleaseEvent(e);
//}
//
//void TitleItem::mouseDoubleClickEvent(QMouseEvent *e) {
//    QQuickItem::mouseDoubleClickEvent(e);
//}
