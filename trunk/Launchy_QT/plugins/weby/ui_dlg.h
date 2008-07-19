/********************************************************************************
** Form generated from reading ui file 'dlg.ui'
**
** Created: Sat Jul 19 14:13:36 2008
**      by: Qt User Interface Compiler version 4.4.0
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

QT_BEGIN_NAMESPACE

class Ui_Dlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QLabel *label;
    QSpacerItem *spacerItem1;
    QCheckBox *booksFirefox;
    QCheckBox *booksIE;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLabel *label_default;
    QTableWidget *table;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *spacerItem2;
    QPushButton *tableNew;
    QSpacerItem *spacer;
    QPushButton *pushDefault;
    QSpacerItem *spacerItem3;
    QPushButton *tableRemove;
    QSpacerItem *spacerItem4;

    void setupUi(QWidget *Dlg)
    {
    if (Dlg->objectName().isEmpty())
        Dlg->setObjectName(QString::fromUtf8("Dlg"));
    Dlg->resize(411, 271);
    verticalLayout = new QVBoxLayout(Dlg);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    label = new QLabel(Dlg);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);


    verticalLayout->addLayout(hboxLayout);

    booksFirefox = new QCheckBox(Dlg);
    booksFirefox->setObjectName(QString::fromUtf8("booksFirefox"));

    verticalLayout->addWidget(booksFirefox);

    booksIE = new QCheckBox(Dlg);
    booksIE->setObjectName(QString::fromUtf8("booksIE"));

    verticalLayout->addWidget(booksIE);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label_2 = new QLabel(Dlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setMaximumSize(QSize(101, 16777215));

    horizontalLayout->addWidget(label_2);

    label_default = new QLabel(Dlg);
    label_default->setObjectName(QString::fromUtf8("label_default"));
    label_default->setMinimumSize(QSize(201, 0));

    horizontalLayout->addWidget(label_default);


    verticalLayout->addLayout(horizontalLayout);

    table = new QTableWidget(Dlg);
    if (table->columnCount() < 3)
        table->setColumnCount(3);
    QTableWidgetItem *__colItem = new QTableWidgetItem();
    table->setHorizontalHeaderItem(0, __colItem);
    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    table->setHorizontalHeaderItem(1, __colItem1);
    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    table->setHorizontalHeaderItem(2, __colItem2);
    table->setObjectName(QString::fromUtf8("table"));
    table->setColumnCount(3);

    verticalLayout->addWidget(table);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    spacerItem2 = new QSpacerItem(28, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacerItem2);

    tableNew = new QPushButton(Dlg);
    tableNew->setObjectName(QString::fromUtf8("tableNew"));

    horizontalLayout_2->addWidget(tableNew);

    spacer = new QSpacerItem(28, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacer);

    pushDefault = new QPushButton(Dlg);
    pushDefault->setObjectName(QString::fromUtf8("pushDefault"));

    horizontalLayout_2->addWidget(pushDefault);

    spacerItem3 = new QSpacerItem(28, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacerItem3);

    tableRemove = new QPushButton(Dlg);
    tableRemove->setObjectName(QString::fromUtf8("tableRemove"));

    horizontalLayout_2->addWidget(tableRemove);

    spacerItem4 = new QSpacerItem(28, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacerItem4);


    verticalLayout->addLayout(horizontalLayout_2);


    retranslateUi(Dlg);

    QMetaObject::connectSlotsByName(Dlg);
    } // setupUi

    void retranslateUi(QWidget *Dlg)
    {
    Dlg->setWindowTitle(QApplication::translate("Dlg", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dlg", "Weby - Launch your favorite websites", 0, QApplication::UnicodeUTF8));
    booksFirefox->setText(QApplication::translate("Dlg", "Firefox Bookmarks (See manual for Firefox 3)", 0, QApplication::UnicodeUTF8));
    booksIE->setText(QApplication::translate("Dlg", "Internet Explorer Bookmarks (Windows only)", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Dlg", "Default Search:", 0, QApplication::UnicodeUTF8));
    label_default->setText(QString());
    table->horizontalHeaderItem(0)->setText(QApplication::translate("Dlg", "Name", 0, QApplication::UnicodeUTF8));
    table->horizontalHeaderItem(1)->setText(QApplication::translate("Dlg", "URL", 0, QApplication::UnicodeUTF8));
    table->horizontalHeaderItem(2)->setText(QApplication::translate("Dlg", "Query", 0, QApplication::UnicodeUTF8));
    tableNew->setText(QApplication::translate("Dlg", "+", 0, QApplication::UnicodeUTF8));
    pushDefault->setText(QApplication::translate("Dlg", "Make Default", 0, QApplication::UnicodeUTF8));
    tableRemove->setText(QApplication::translate("Dlg", "-", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Dlg);
    } // retranslateUi

};

namespace Ui {
    class Dlg: public Ui_Dlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_H
