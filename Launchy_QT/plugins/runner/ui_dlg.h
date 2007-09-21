/********************************************************************************
** Form generated from reading ui file 'dlg.ui'
**
** Created: Thu Sep 20 22:34:25 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DLG_H
#define UI_DLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_Dlg
{
public:
    QLabel *label;
    QTableWidget *table;
    QPushButton *tableNew;
    QPushButton *tableRemove;
    QLabel *label_2;

    void setupUi(QWidget *Dlg)
    {
    if (Dlg->objectName().isEmpty())
        Dlg->setObjectName(QString::fromUtf8("Dlg"));
    Dlg->resize(411, 271);
    label = new QLabel(Dlg);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(120, 10, 187, 16));
    table = new QTableWidget(Dlg);
    table->setObjectName(QString::fromUtf8("table"));
    table->setGeometry(QRect(10, 30, 391, 181));
    table->setColumnCount(3);
    tableNew = new QPushButton(Dlg);
    tableNew->setObjectName(QString::fromUtf8("tableNew"));
    tableNew->setGeometry(QRect(40, 220, 75, 23));
    tableRemove = new QPushButton(Dlg);
    tableRemove->setObjectName(QString::fromUtf8("tableRemove"));
    tableRemove->setGeometry(QRect(290, 220, 75, 23));
    label_2 = new QLabel(Dlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(80, 250, 281, 21));

    retranslateUi(Dlg);

    QMetaObject::connectSlotsByName(Dlg);
    } // setupUi

    void retranslateUi(QWidget *Dlg)
    {
    Dlg->setWindowTitle(QApplication::translate("Dlg", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dlg", "Runner - Launch custom commands", 0, QApplication::UnicodeUTF8));
    if (table->columnCount() < 3)
        table->setColumnCount(3);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("Dlg", "Name", 0, QApplication::UnicodeUTF8));
    table->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("Dlg", "Program", 0, QApplication::UnicodeUTF8));
    table->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("Dlg", "Arguments", 0, QApplication::UnicodeUTF8));
    table->setHorizontalHeaderItem(2, __colItem2);
    tableNew->setText(QApplication::translate("Dlg", "+", 0, QApplication::UnicodeUTF8));
    tableRemove->setText(QApplication::translate("Dlg", "-", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Dlg", "Hint: Use $$ symbols to mark locations for user input", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Dlg);
    } // retranslateUi

};

namespace Ui {
    class Dlg: public Ui_Dlg {};
} // namespace Ui

#endif // UI_DLG_H
