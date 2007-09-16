/********************************************************************************
** Form generated from reading ui file 'dlg.ui'
**
** Created: Sun Sep 16 13:15:16 2007
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
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_Dlg
{
public:
    QLabel *label;
    QCheckBox *booksFirefox;
    QCheckBox *booksIE;
    QTableWidget *table;
    QPushButton *tableNew;
    QPushButton *tableRemove;

    void setupUi(QWidget *Dlg)
    {
    if (Dlg->objectName().isEmpty())
        Dlg->setObjectName(QString::fromUtf8("Dlg"));
    Dlg->resize(411, 271);
    label = new QLabel(Dlg);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(120, 10, 187, 16));
    booksFirefox = new QCheckBox(Dlg);
    booksFirefox->setObjectName(QString::fromUtf8("booksFirefox"));
    booksFirefox->setGeometry(QRect(10, 30, 111, 18));
    booksIE = new QCheckBox(Dlg);
    booksIE->setObjectName(QString::fromUtf8("booksIE"));
    booksIE->setGeometry(QRect(10, 50, 161, 18));
    table = new QTableWidget(Dlg);
    table->setObjectName(QString::fromUtf8("table"));
    table->setGeometry(QRect(10, 80, 391, 151));
    table->setColumnCount(3);
    tableNew = new QPushButton(Dlg);
    tableNew->setObjectName(QString::fromUtf8("tableNew"));
    tableNew->setGeometry(QRect(50, 240, 75, 23));
    tableRemove = new QPushButton(Dlg);
    tableRemove->setObjectName(QString::fromUtf8("tableRemove"));
    tableRemove->setGeometry(QRect(280, 240, 75, 23));

    retranslateUi(Dlg);

    QMetaObject::connectSlotsByName(Dlg);
    } // setupUi

    void retranslateUi(QWidget *Dlg)
    {
    Dlg->setWindowTitle(QApplication::translate("Dlg", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dlg", "Weby - Launch your favorite websites!", 0, QApplication::UnicodeUTF8));
    booksFirefox->setText(QApplication::translate("Dlg", "Firefox Bookmarks", 0, QApplication::UnicodeUTF8));
    booksIE->setText(QApplication::translate("Dlg", "Internet Explorer Bookmarks", 0, QApplication::UnicodeUTF8));
    if (table->columnCount() < 3)
        table->setColumnCount(3);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("Dlg", "Name", 0, QApplication::UnicodeUTF8));
    table->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("Dlg", "URL", 0, QApplication::UnicodeUTF8));
    table->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("Dlg", "Query", 0, QApplication::UnicodeUTF8));
    table->setHorizontalHeaderItem(2, __colItem2);
    tableNew->setText(QApplication::translate("Dlg", "+", 0, QApplication::UnicodeUTF8));
    tableRemove->setText(QApplication::translate("Dlg", "-", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Dlg);
    } // retranslateUi

};

namespace Ui {
    class Dlg: public Ui_Dlg {};
} // namespace Ui

#endif // UI_DLG_H
