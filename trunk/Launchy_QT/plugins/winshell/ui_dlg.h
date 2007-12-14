/********************************************************************************
** Form generated from reading ui file 'dlg.ui'
**
** Created: Fri Dec 14 12:16:17 2007
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DLG_H
#define UI_DLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
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
    QTableWidget *table;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem2;
    QPushButton *tableNew;
    QSpacerItem *spacerItem3;
    QPushButton *tableRemove;
    QSpacerItem *spacerItem4;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem5;
    QLabel *label_2;
    QSpacerItem *spacerItem6;

    void setupUi(QWidget *Dlg)
    {
    if (Dlg->objectName().isEmpty())
        Dlg->setObjectName(QString::fromUtf8("Dlg"));
    Dlg->resize(354, 273);
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

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem5);

    label_2 = new QLabel(Dlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    hboxLayout2->addWidget(label_2);

    spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem6);


    vboxLayout->addLayout(hboxLayout2);


    retranslateUi(Dlg);

    QMetaObject::connectSlotsByName(Dlg);
    } // setupUi

    void retranslateUi(QWidget *Dlg)
    {
    Dlg->setWindowTitle(QApplication::translate("Dlg", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dlg", "winshell - Launch custom commands", 0, QApplication::UnicodeUTF8));
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
