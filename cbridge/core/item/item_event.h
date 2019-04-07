//
// Created by sulin on 2017/10/6.
//

#ifndef UILIB_CONNECT_H
#define UILIB_CONNECT_H

#include <QObject>
#include <QArgument>
#include <QJSValue>
#include <QMultiMap>
#include <utility>
#include <QVariant>

class EventItem : public QObject {
Q_OBJECT
    Q_PROPERTY(QString key READ getKey WRITE setKey)
    Q_PROPERTY(bool enable MEMBER enable)

public:
    static QMultiMap<QString, EventItem *> ReceverMap;

private:
    QString key;
    bool enable;

public:
    explicit EventItem(QObject *parent = Q_NULLPTR);

    ~EventItem() override;

    QString getKey() {
        return key;
    }

    void setKey(QString key) {
        ReceverMap.remove(this->key, this);
        this->key = std::move(key);
        ReceverMap.insert(this->key, this);
    }

    void notify(QVariant &data);

signals:

    void active(QVariant data);

};


#endif //UILIB_CONNECT_H
