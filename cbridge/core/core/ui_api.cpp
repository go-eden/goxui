//
// Created by sulin on 2018/1/12.
//
#include <QClipboard>
#include <QPainter>
//#include <QPrinter>
//#include <QPrintDialog>
#include <QThread>
#include <QWindow>
#include <QImage>
#include <QApplication>
#include <QNetworkProxy>
#include <QFileDialog>
#include <QDebug>
#include "ui_api.h"

UIApi::UIApi(QQmlApplicationEngine *engine) : QObject(engine) {
    this->engine = engine;
}

void UIApi::clearComponentCache() {
    engine->clearComponentCache();
}

void UIApi::print(QVariant data){
//    QImage img = qvariant_cast<QImage>(data);
//    QPrinter printer;
//    QPrintDialog dialog(&printer, nullptr);
//    if(dialog.exec() == QDialog::Accepted) {
//        QPainter painter(&printer);
//        painter.drawImage(QPoint(0,0), img);
//        painter.end();
//    }
}

void UIApi::setClipboard(QString text) {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

QVariantMap UIApi::execSaveFileDialog(QString defaultName, QStringList nameFilters){
    QVariantMap result;
    
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("html");
    dialog.selectFile(defaultName);
    dialog.setNameFilters(nameFilters);
    
    result["accept"] = dialog.exec();
    result["file"] = dialog.selectedFiles();
    result["nameFilter"] = dialog.selectedNameFilter();
    return result;
}
