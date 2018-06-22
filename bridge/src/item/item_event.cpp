//
// Created by sulin on 2017/10/6.
//

#include "item_event.h"

QMultiMap<QString, EventItem *> EventItem::ReceverMap;

EventItem::EventItem(QObject *parent) : QObject(parent) {
    ReceverMap.insert(key, this);
}

EventItem::~EventItem() {
    ReceverMap.remove(key, this); // Is this thread safe?
}

void EventItem::notify(QVariant &data) {
    emit this->active(data);
}
