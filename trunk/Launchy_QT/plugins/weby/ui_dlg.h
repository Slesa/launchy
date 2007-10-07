/********************************************************************************
** Form generated from reading ui file 'dlg.ui'
**
** Created: Sun Oct 7 16:12:05 2007
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_Dlg
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QLabel *label;
    QSpacerItem *spacerItem1;
    QCheckBox *booksFirefox;
    QCheckBox *booksIE;
    QTableWidget *table;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem2;
    QPushButton *tableNew;
    QSpacerItem *spacerItem3;
    QPushButton *tableRemove;
    QSpacerItem *spacerItem4;

    void setupUi(QWidget *Dlg)
    {
    if (Dlg->objectName().isEmpty())
        Dlg->setObjectName(QString::fromUtf8("Dlg"));
    Dlg->resize(411, 271);
    vboxLayout = new QVBoxLayout(Dlg);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    label = new QLabel(Dlg);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout);

    booksFirefox = new QCheckBox(Dlg);
    booksFirefox->setObjectName(QString::fromUtf8("booksFirefox"));

    vboxLayout->addWidget(booksFirefox);

    booksIE = new QCheckBox(Dlg);
    booksIE->setObjectName(QString::fromUtf8("booksIE"));

    vboxLayout->addWidget(booksIE);

    table = new QTableWidget(Dlg);
    table->setObjectName(QString::fromUtf8("table"));
    table->setColumnCount(3);

    vboxLayout->addWidget(table);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem2);

    tableNew = new QPushButton(Dlg);
    tableNew->setObjectName(QString::fromUtf8("tableNew"));

    hboxLayout1->addWidget(tableNew);

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem3);

    tableRemove = new QPushButton(Dlg);
    tableRemove->setObjectName(QString::fromUtf8("tableRemove"));

    hboxLayout1->addWidget(tableRemove);

    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem4);


    vboxLayout->addLayout(hboxLayout1);


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
