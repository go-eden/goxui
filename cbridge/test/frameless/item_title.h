//
// Created by sulin on 2017/9/30.
//

#ifndef UILIB_WINDOWTITLE_H
#define UILIB_WINDOWTITLE_H


#include <QQuickItem>
#include <QQuickWindow>

class TitleItem : public QQuickItem {
Q_OBJECT

private:
    bool flag = false;
    int fromX, fromY, fromWinX, fromWinY;

public:

    explicit TitleItem(QQuickItem *parent = Q_NULLPTR);

    bool event(QEvent *event) override;

//    void mousePressEvent(QMouseEvent *e) override;
//
//    void mouseMoveEvent(QMouseEvent *event) override;
//
//    void mouseReleaseEvent(QMouseEvent *e) override;
//
//    void mouseDoubleClickEvent(QMouseEvent *e) override;

};


#endif //UILIB_WINDOWTITLE_H
