//
// Created by sulin on 2018/1/12.
//
#include <QClipboard>
#include <QWindow>
#include <QApplication>
#include <QNetworkProxy>
#include <QFileDialog>
#include <QDebug>

#include "ui_system.h"

UISystem::UISystem(QQmlApplicationEngine *engine) : QObject(engine) {
    this->engine = engine;
}

void UISystem::clearComponentCache() {
    engine->clearComponentCache();
}

void UISystem::setClipboard(QString text) {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

QVariantMap UISystem::execSaveFileDialog(QString defaultName, QStringList nameFilters){
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
