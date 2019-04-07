//
// Created by sulin on 2019/4/7.
//

#ifndef GOXUI_PRINT_P_H
#define GOXUI_PRINT_P_H

#include <QVariant>
#include <QVariantMap>
#include <QObject>

/**
 * 向UI暴露的系统工具API
 */
class Printer : public QObject {
Q_OBJECT

public:

    explicit Printer();

    /**
     * Execute print, data's type must be Image
     */
    Q_INVOKABLE void print(QVariant data);

};

#endif //GOXUI_PRINT_P_H
