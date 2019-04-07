//
// Created by sulin on 2019/4/7.
//
#include <QClipboard>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QImage>
#include <QDebug>

#include "goxui.h"
#include "goxui_print_p.h"

void Printer::print(QVariant data){
    QImage img = qvariant_cast<QImage>(data);
    QPrinter printer;
    QPrintDialog dialog(&printer, nullptr);
    if(dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.drawImage(QPoint(0,0), img);
        painter.end();
    }
}

// exec init
API void ui_init_print() {
    Printer* printer = new Printer();
    ui_add_object(const_cast<char *>("Printer"), printer);
}
