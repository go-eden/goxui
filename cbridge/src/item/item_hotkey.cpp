//
// Created by sulin on 2018/1/9.
//

#include "item_hotkey.h"

HotKeyItem::HotKeyItem(QObject *parent) : QObject(parent) {
    QObject::connect(&hotkey, &QHotkey::activated, [=]() {
        emit this->activated();
    });
}

QString HotKeyItem::getSequence() {
    return this->sequence;
}

void HotKeyItem::setSequence(QString key) {
    this->sequence = key;
    this->hotkey.setShortcut(QKeySequence(key), true);
}
