/********************************************************************************
** Form generated from reading ui file 'options.ui'
**
** Created: Tue Sep 25 18:29:39 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_OptionsDlg
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout1;
    QGroupBox *groupBox_4;
    QVBoxLayout *vboxLayout2;
    QCheckBox *genAlwaysShow;
    QCheckBox *genHideFocus;
    QCheckBox *genAlwaysTop;
    QCheckBox *genCondensed;
    QHBoxLayout *hboxLayout2;
    QLabel *label_8;
    QSpacerItem *spacerItem;
    QLineEdit *genNumResults;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout3;
    QLabel *label;
    QSpacerItem *spacerItem2;
    QComboBox *genModifierBox;
    QSpacerItem *spacerItem3;
    QComboBox *genKeyBox;
    QSpacerItem *spacerItem4;
    QSpacerItem *spacerItem5;
    QVBoxLayout *vboxLayout3;
    QGroupBox *groupBox_6;
    QVBoxLayout *vboxLayout4;
    QCheckBox *genUpdateCheck;
    QCheckBox *genShowHidden;
    QCheckBox *genPortable;
    QSpacerItem *spacerItem6;
    QHBoxLayout *hboxLayout4;
    QGroupBox *groupBox_5;
    QVBoxLayout *vboxLayout5;
    QHBoxLayout *hboxLayout5;
    QLabel *label_5;
    QLineEdit *genUpMinutes;
    QSpacerItem *spacerItem7;
    QLabel *label_7;
    QCheckBox *genFastIndex;
    QSpacerItem *spacerItem8;
    QSpacerItem *spacerItem9;
    QWidget *Skins;
    QVBoxLayout *vboxLayout6;
    QHBoxLayout *hboxLayout6;
    QGroupBox *groupBox_3;
    QHBoxLayout *hboxLayout7;
    QListWidget *skinList;
    QGroupBox *groupBox_8;
    QVBoxLayout *vboxLayout7;
    QLabel *authorInfo;
    QSpacerItem *spacerItem10;
    QGroupBox *groupBox_9;
    QVBoxLayout *vboxLayout8;
    QLabel *skinPreview;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout9;
    QHBoxLayout *hboxLayout8;
    QVBoxLayout *vboxLayout10;
    QGroupBox *groupBox_2;
    QVBoxLayout *vboxLayout11;
    QListWidget *catTypes;
    QLineEdit *catTypeEdit;
    QPushButton *catTypesPlus;
    QPushButton *catTypesMinus;
    QCheckBox *catCheckBinaries;
    QCheckBox *catCheckDirs;
    QHBoxLayout *hboxLayout9;
    QLabel *label_10;
    QSpinBox *catDepth;
    QSpacerItem *spacerItem11;
    QVBoxLayout *vboxLayout12;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout13;
    QListWidget *catDirectories;
    QHBoxLayout *hboxLayout10;
    QSpacerItem *spacerItem12;
    QPushButton *catDirPlus;
    QSpacerItem *spacerItem13;
    QPushButton *catDirMinus;
    QSpacerItem *spacerItem14;
    QSpacerItem *spacerItem15;
    QSpacerItem *spacerItem16;
    QVBoxLayout *vboxLayout14;
    QHBoxLayout *hboxLayout11;
    QPushButton *catRescan;
    QSpacerItem *spacerItem17;
    QLabel *catSize;
    QSpacerItem *spacerItem18;
    QProgressBar *catProgress;
    QWidget *tab_4;
    QVBoxLayout *vboxLayout15;
    QHBoxLayout *hboxLayout12;
    QGroupBox *groupBox_7;
    QVBoxLayout *vboxLayout16;
    QListWidget *plugList;
    QGroupBox *plugBox;
    QVBoxLayout *vboxLayout17;
    QSpacerItem *spacerItem19;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout18;
    QSpacerItem *spacerItem20;
    QHBoxLayout *hboxLayout13;
    QSpacerItem *spacerItem21;
    QLabel *aboutVer;
    QSpacerItem *spacerItem22;
    QHBoxLayout *hboxLayout14;
    QSpacerItem *spacerItem23;
    QLabel *label_2;
    QSpacerItem *spacerItem24;
    QSpacerItem *spacerItem25;
    QHBoxLayout *hboxLayout15;
    QSpacerItem *spacerItem26;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OptionsDlg)
    {
    if (OptionsDlg->objectName().isEmpty())
        OptionsDlg->setObjectName(QString::fromUtf8("OptionsDlg"));
    OptionsDlg->setWindowModality(Qt::NonModal);
    OptionsDlg->resize(566, 453);
    vboxLayout = new QVBoxLayout(OptionsDlg);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

    vboxLayout->addLayout(hboxLayout);

    tabWidget = new QTabWidget(OptionsDlg);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    vboxLayout1 = new QVBoxLayout(tab);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    groupBox_4 = new QGroupBox(tab);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    vboxLayout2 = new QVBoxLayout(groupBox_4);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    genAlwaysShow = new QCheckBox(groupBox_4);
    genAlwaysShow->setObjectName(QString::fromUtf8("genAlwaysShow"));

    vboxLayout2->addWidget(genAlwaysShow);

    genHideFocus = new QCheckBox(groupBox_4);
    genHideFocus->setObjectName(QString::fromUtf8("genHideFocus"));

    vboxLayout2->addWidget(genHideFocus);

    genAlwaysTop = new QCheckBox(groupBox_4);
    genAlwaysTop->setObjectName(QString::fromUtf8("genAlwaysTop"));
    genAlwaysTop->setEnabled(false);

    vboxLayout2->addWidget(genAlwaysTop);

    genCondensed = new QCheckBox(groupBox_4);
    genCondensed->setObjectName(QString::fromUtf8("genCondensed"));

    vboxLayout2->addWidget(genCondensed);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    label_8 = new QLabel(groupBox_4);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    hboxLayout2->addWidget(label_8);

    spacerItem = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem);

    genNumResults = new QLineEdit(groupBox_4);
    genNumResults->setObjectName(QString::fromUtf8("genNumResults"));

    hboxLayout2->addWidget(genNumResults);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);


    vboxLayout2->addLayout(hboxLayout2);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label = new QLabel(groupBox_4);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout3->addWidget(label);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem2);

    genModifierBox = new QComboBox(groupBox_4);
    genModifierBox->setObjectName(QString::fromUtf8("genModifierBox"));

    hboxLayout3->addWidget(genModifierBox);

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem3);

    genKeyBox = new QComboBox(groupBox_4);
    genKeyBox->setObjectName(QString::fromUtf8("genKeyBox"));

    hboxLayout3->addWidget(genKeyBox);

    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem4);


    vboxLayout2->addLayout(hboxLayout3);


    hboxLayout1->addWidget(groupBox_4);

    spacerItem5 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem5);

    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    groupBox_6 = new QGroupBox(tab);
    groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
    vboxLayout4 = new QVBoxLayout(groupBox_6);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    genUpdateCheck = new QCheckBox(groupBox_6);
    genUpdateCheck->setObjectName(QString::fromUtf8("genUpdateCheck"));

    vboxLayout4->addWidget(genUpdateCheck);

    genShowHidden = new QCheckBox(groupBox_6);
    genShowHidden->setObjectName(QString::fromUtf8("genShowHidden"));

    vboxLayout4->addWidget(genShowHidden);

    genPortable = new QCheckBox(groupBox_6);
    genPortable->setObjectName(QString::fromUtf8("genPortable"));

    vboxLayout4->addWidget(genPortable);


    vboxLayout3->addWidget(groupBox_6);

    spacerItem6 = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout3->addItem(spacerItem6);


    hboxLayout1->addLayout(vboxLayout3);


    vboxLayout1->addLayout(hboxLayout1);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    groupBox_5 = new QGroupBox(tab);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    vboxLayout5 = new QVBoxLayout(groupBox_5);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    label_5 = new QLabel(groupBox_5);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    hboxLayout5->addWidget(label_5);

    genUpMinutes = new QLineEdit(groupBox_5);
    genUpMinutes->setObjectName(QString::fromUtf8("genUpMinutes"));

    hboxLayout5->addWidget(genUpMinutes);

    spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem7);


    vboxLayout5->addLayout(hboxLayout5);

    label_7 = new QLabel(groupBox_5);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    vboxLayout5->addWidget(label_7);

    genFastIndex = new QCheckBox(groupBox_5);
    genFastIndex->setObjectName(QString::fromUtf8("genFastIndex"));

    vboxLayout5->addWidget(genFastIndex);


    hboxLayout4->addWidget(groupBox_5);

    spacerItem8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem8);


    vboxLayout1->addLayout(hboxLayout4);

    spacerItem9 = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem9);

    tabWidget->addTab(tab, QString());
    Skins = new QWidget();
    Skins->setObjectName(QString::fromUtf8("Skins"));
    vboxLayout6 = new QVBoxLayout(Skins);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    groupBox_3 = new QGroupBox(Skins);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setMinimumSize(QSize(0, 0));
    groupBox_3->setMaximumSize(QSize(160, 16777215));
    hboxLayout7 = new QHBoxLayout(groupBox_3);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    skinList = new QListWidget(groupBox_3);
    skinList->setObjectName(QString::fromUtf8("skinList"));
    skinList->setMaximumSize(QSize(150, 16777215));

    hboxLayout7->addWidget(skinList);


    hboxLayout6->addWidget(groupBox_3);

    groupBox_8 = new QGroupBox(Skins);
    groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
    groupBox_8->setMinimumSize(QSize(300, 166));
    vboxLayout7 = new QVBoxLayout(groupBox_8);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    authorInfo = new QLabel(groupBox_8);
    authorInfo->setObjectName(QString::fromUtf8("authorInfo"));
    authorInfo->setScaledContents(true);
    authorInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    vboxLayout7->addWidget(authorInfo);

    spacerItem10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    vboxLayout7->addItem(spacerItem10);


    hboxLayout6->addWidget(groupBox_8);


    vboxLayout6->addLayout(hboxLayout6);

    groupBox_9 = new QGroupBox(Skins);
    groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
    groupBox_9->setMinimumSize(QSize(0, 150));
    vboxLayout8 = new QVBoxLayout(groupBox_9);
    vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
    skinPreview = new QLabel(groupBox_9);
    skinPreview->setObjectName(QString::fromUtf8("skinPreview"));
    skinPreview->setScaledContents(false);
    skinPreview->setAlignment(Qt::AlignCenter);

    vboxLayout8->addWidget(skinPreview);


    vboxLayout6->addWidget(groupBox_9);

    tabWidget->addTab(Skins, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    vboxLayout9 = new QVBoxLayout(tab_2);
    vboxLayout9->setObjectName(QString::fromUtf8("vboxLayout9"));
    hboxLayout8 = new QHBoxLayout();
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    vboxLayout10 = new QVBoxLayout();
    vboxLayout10->setObjectName(QString::fromUtf8("vboxLayout10"));
    groupBox_2 = new QGroupBox(tab_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
    groupBox_2->setSizePolicy(sizePolicy);
    groupBox_2->setMaximumSize(QSize(115, 16777215));
    vboxLayout11 = new QVBoxLayout(groupBox_2);
    vboxLayout11->setObjectName(QString::fromUtf8("vboxLayout11"));
    catTypes = new QListWidget(groupBox_2);
    catTypes->setObjectName(QString::fromUtf8("catTypes"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(catTypes->sizePolicy().hasHeightForWidth());
    catTypes->setSizePolicy(sizePolicy1);
    catTypes->setMaximumSize(QSize(16777215, 16777215));

    vboxLayout11->addWidget(catTypes);

    catTypeEdit = new QLineEdit(groupBox_2);
    catTypeEdit->setObjectName(QString::fromUtf8("catTypeEdit"));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(catTypeEdit->sizePolicy().hasHeightForWidth());
    catTypeEdit->setSizePolicy(sizePolicy2);
    catTypeEdit->setMaximumSize(QSize(16777215, 16777215));

    vboxLayout11->addWidget(catTypeEdit);

    catTypesPlus = new QPushButton(groupBox_2);
    catTypesPlus->setObjectName(QString::fromUtf8("catTypesPlus"));
    catTypesPlus->setMaximumSize(QSize(16777215, 16777215));

    vboxLayout11->addWidget(catTypesPlus);

    catTypesMinus = new QPushButton(groupBox_2);
    catTypesMinus->setObjectName(QString::fromUtf8("catTypesMinus"));

    vboxLayout11->addWidget(catTypesMinus);

    catCheckBinaries = new QCheckBox(groupBox_2);
    catCheckBinaries->setObjectName(QString::fromUtf8("catCheckBinaries"));

    vboxLayout11->addWidget(catCheckBinaries);

    catCheckDirs = new QCheckBox(groupBox_2);
    catCheckDirs->setObjectName(QString::fromUtf8("catCheckDirs"));

    vboxLayout11->addWidget(catCheckDirs);

    hboxLayout9 = new QHBoxLayout();
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    label_10 = new QLabel(groupBox_2);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    hboxLayout9->addWidget(label_10);

    catDepth = new QSpinBox(groupBox_2);
    catDepth->setObjectName(QString::fromUtf8("catDepth"));
    catDepth->setMaximum(100000);
    catDepth->setValue(100);

    hboxLayout9->addWidget(catDepth);


    vboxLayout11->addLayout(hboxLayout9);


    vboxLayout10->addWidget(groupBox_2);

    spacerItem11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout10->addItem(spacerItem11);


    hboxLayout8->addLayout(vboxLayout10);

    vboxLayout12 = new QVBoxLayout();
    vboxLayout12->setObjectName(QString::fromUtf8("vboxLayout12"));
    groupBox = new QGroupBox(tab_2);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout13 = new QVBoxLayout(groupBox);
    vboxLayout13->setObjectName(QString::fromUtf8("vboxLayout13"));
    catDirectories = new QListWidget(groupBox);
    catDirectories->setObjectName(QString::fromUtf8("catDirectories"));

    vboxLayout13->addWidget(catDirectories);

    hboxLayout10 = new QHBoxLayout();
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    spacerItem12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout10->addItem(spacerItem12);

    catDirPlus = new QPushButton(groupBox);
    catDirPlus->setObjectName(QString::fromUtf8("catDirPlus"));

    hboxLayout10->addWidget(catDirPlus);

    spacerItem13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout10->addItem(spacerItem13);

    catDirMinus = new QPushButton(groupBox);
    catDirMinus->setObjectName(QString::fromUtf8("catDirMinus"));

    hboxLayout10->addWidget(catDirMinus);

    spacerItem14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout10->addItem(spacerItem14);


    vboxLayout13->addLayout(hboxLayout10);


    vboxLayout12->addWidget(groupBox);

    spacerItem15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout12->addItem(spacerItem15);


    hboxLayout8->addLayout(vboxLayout12);


    vboxLayout9->addLayout(hboxLayout8);

    spacerItem16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout9->addItem(spacerItem16);

    vboxLayout14 = new QVBoxLayout();
    vboxLayout14->setObjectName(QString::fromUtf8("vboxLayout14"));
    hboxLayout11 = new QHBoxLayout();
    hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
    catRescan = new QPushButton(tab_2);
    catRescan->setObjectName(QString::fromUtf8("catRescan"));

    hboxLayout11->addWidget(catRescan);

    spacerItem17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout11->addItem(spacerItem17);

    catSize = new QLabel(tab_2);
    catSize->setObjectName(QString::fromUtf8("catSize"));

    hboxLayout11->addWidget(catSize);

    spacerItem18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout11->addItem(spacerItem18);


    vboxLayout14->addLayout(hboxLayout11);

    catProgress = new QProgressBar(tab_2);
    catProgress->setObjectName(QString::fromUtf8("catProgress"));
    catProgress->setTextVisible(false);
    catProgress->setOrientation(Qt::Horizontal);

    vboxLayout14->addWidget(catProgress);


    vboxLayout9->addLayout(vboxLayout14);

    tabWidget->addTab(tab_2, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    vboxLayout15 = new QVBoxLayout(tab_4);
    vboxLayout15->setObjectName(QString::fromUtf8("vboxLayout15"));
    hboxLayout12 = new QHBoxLayout();
    hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
    groupBox_7 = new QGroupBox(tab_4);
    groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
    groupBox_7->setMaximumSize(QSize(141, 16777215));
    vboxLayout16 = new QVBoxLayout(groupBox_7);
    vboxLayout16->setObjectName(QString::fromUtf8("vboxLayout16"));
    plugList = new QListWidget(groupBox_7);
    plugList->setObjectName(QString::fromUtf8("plugList"));
    plugList->setSelectionRectVisible(false);

    vboxLayout16->addWidget(plugList);


    hboxLayout12->addWidget(groupBox_7);

    plugBox = new QGroupBox(tab_4);
    plugBox->setObjectName(QString::fromUtf8("plugBox"));
    vboxLayout17 = new QVBoxLayout(plugBox);
    vboxLayout17->setObjectName(QString::fromUtf8("vboxLayout17"));
    spacerItem19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    vboxLayout17->addItem(spacerItem19);


    hboxLayout12->addWidget(plugBox);


    vboxLayout15->addLayout(hboxLayout12);

    tabWidget->addTab(tab_4, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    vboxLayout18 = new QVBoxLayout(tab_3);
    vboxLayout18->setObjectName(QString::fromUtf8("vboxLayout18"));
    spacerItem20 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout18->addItem(spacerItem20);

    hboxLayout13 = new QHBoxLayout();
    hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
    spacerItem21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout13->addItem(spacerItem21);

    aboutVer = new QLabel(tab_3);
    aboutVer->setObjectName(QString::fromUtf8("aboutVer"));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);
    aboutVer->setFont(font);
    aboutVer->setWordWrap(false);

    hboxLayout13->addWidget(aboutVer);

    spacerItem22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout13->addItem(spacerItem22);


    vboxLayout18->addLayout(hboxLayout13);

    hboxLayout14 = new QHBoxLayout();
    hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
    spacerItem23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout14->addItem(spacerItem23);

    label_2 = new QLabel(tab_3);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setWordWrap(false);
    label_2->setOpenExternalLinks(true);

    hboxLayout14->addWidget(label_2);

    spacerItem24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout14->addItem(spacerItem24);


    vboxLayout18->addLayout(hboxLayout14);

    spacerItem25 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout18->addItem(spacerItem25);

    tabWidget->addTab(tab_3, QString());

    vboxLayout->addWidget(tabWidget);

    hboxLayout15 = new QHBoxLayout();
    hboxLayout15->setObjectName(QString::fromUtf8("hboxLayout15"));
    spacerItem26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout15->addItem(spacerItem26);

    buttonBox = new QDialogButtonBox(OptionsDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    hboxLayout15->addWidget(buttonBox);


    vboxLayout->addLayout(hboxLayout15);

    QWidget::setTabOrder(genAlwaysShow, genHideFocus);
    QWidget::setTabOrder(genHideFocus, genAlwaysTop);
    QWidget::setTabOrder(genAlwaysTop, genCondensed);
    QWidget::setTabOrder(genCondensed, genNumResults);
    QWidget::setTabOrder(genNumResults, genModifierBox);
    QWidget::setTabOrder(genModifierBox, genKeyBox);
    QWidget::setTabOrder(genKeyBox, genUpdateCheck);
    QWidget::setTabOrder(genUpdateCheck, genShowHidden);
    QWidget::setTabOrder(genShowHidden, genPortable);
    QWidget::setTabOrder(genPortable, genUpMinutes);
    QWidget::setTabOrder(genUpMinutes, genFastIndex);
    QWidget::setTabOrder(genFastIndex, skinList);
    QWidget::setTabOrder(skinList, catRescan);
    QWidget::setTabOrder(catRescan, tabWidget);
    QWidget::setTabOrder(tabWidget, catDirMinus);
    QWidget::setTabOrder(catDirMinus, catDirectories);
    QWidget::setTabOrder(catDirectories, catDirPlus);
    QWidget::setTabOrder(catDirPlus, catTypesMinus);
    QWidget::setTabOrder(catTypesMinus, catTypeEdit);
    QWidget::setTabOrder(catTypeEdit, catTypesPlus);
    QWidget::setTabOrder(catTypesPlus, catTypes);
    QWidget::setTabOrder(catTypes, catCheckDirs);
    QWidget::setTabOrder(catCheckDirs, catCheckBinaries);
    QWidget::setTabOrder(catCheckBinaries, catDepth);
    QWidget::setTabOrder(catDepth, plugList);
    QWidget::setTabOrder(plugList, buttonBox);

    retranslateUi(OptionsDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), OptionsDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), OptionsDlg, SLOT(reject()));

    tabWidget->setCurrentIndex(4);


    QMetaObject::connectSlotsByName(OptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *OptionsDlg)
    {
    OptionsDlg->setWindowTitle(QApplication::translate("OptionsDlg", "Launchy options", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("OptionsDlg", "User Interface", 0, QApplication::UnicodeUTF8));
    genAlwaysShow->setText(QApplication::translate("OptionsDlg", "Always display Launchy window", 0, QApplication::UnicodeUTF8));
    genHideFocus->setText(QApplication::translate("OptionsDlg", "Hide Launchy when it loses focus", 0, QApplication::UnicodeUTF8));
    genAlwaysTop->setText(QApplication::translate("OptionsDlg", "Always on top", 0, QApplication::UnicodeUTF8));
    genCondensed->setText(QApplication::translate("OptionsDlg", "Condense suggestion list", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("OptionsDlg", "Number of alternate suggestions", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("OptionsDlg", "Hotkey:", 0, QApplication::UnicodeUTF8));
    groupBox_6->setTitle(QApplication::translate("OptionsDlg", "System Options", 0, QApplication::UnicodeUTF8));
    genUpdateCheck->setText(QApplication::translate("OptionsDlg", "Check for updates on startup", 0, QApplication::UnicodeUTF8));
    genShowHidden->setText(QApplication::translate("OptionsDlg", "Show hidden files in browse mode", 0, QApplication::UnicodeUTF8));
    genPortable->setText(QApplication::translate("OptionsDlg", "Portable mode (USB stick)", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("OptionsDlg", "Performance", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("OptionsDlg", "Minutes between catalog updates", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("OptionsDlg", "(0 for manual updates only)", 0, QApplication::UnicodeUTF8));
    genFastIndex->setText(QApplication::translate("OptionsDlg", "Use fast indexer? (uses more memory)", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("OptionsDlg", "General", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("OptionsDlg", "Select a skin", 0, QApplication::UnicodeUTF8));
    groupBox_8->setTitle(QApplication::translate("OptionsDlg", "Author information", 0, QApplication::UnicodeUTF8));
    authorInfo->setText(QString());
    groupBox_9->setTitle(QApplication::translate("OptionsDlg", "Skin preview", 0, QApplication::UnicodeUTF8));
    skinPreview->setText(QString());
    tabWidget->setTabText(tabWidget->indexOf(Skins), QApplication::translate("OptionsDlg", "Skins", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("OptionsDlg", "File Types", 0, QApplication::UnicodeUTF8));
    catTypesPlus->setText(QApplication::translate("OptionsDlg", "+", 0, QApplication::UnicodeUTF8));
    catTypesMinus->setText(QApplication::translate("OptionsDlg", "-", 0, QApplication::UnicodeUTF8));
    catCheckBinaries->setText(QApplication::translate("OptionsDlg", "Executables", 0, QApplication::UnicodeUTF8));
    catCheckDirs->setText(QApplication::translate("OptionsDlg", "Directories", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("OptionsDlg", "Depth:", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("OptionsDlg", "Directories", 0, QApplication::UnicodeUTF8));
    catDirPlus->setText(QApplication::translate("OptionsDlg", "+", 0, QApplication::UnicodeUTF8));
    catDirMinus->setText(QApplication::translate("OptionsDlg", "-", 0, QApplication::UnicodeUTF8));
    catRescan->setText(QApplication::translate("OptionsDlg", "Rescan Catalog", 0, QApplication::UnicodeUTF8));
    catSize->setText(QApplication::translate("OptionsDlg", "Indexing 0 items", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("OptionsDlg", "Catalog", 0, QApplication::UnicodeUTF8));
    groupBox_7->setTitle(QApplication::translate("OptionsDlg", "Available plugins", 0, QApplication::UnicodeUTF8));
    plugBox->setTitle(QApplication::translate("OptionsDlg", "Plugin options", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("OptionsDlg", "Plugins", 0, QApplication::UnicodeUTF8));
    aboutVer->setText(QString());
    label_2->setText(QApplication::translate("OptionsDlg", "Launchy's official homepage is http://www.launchy.net/", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("OptionsDlg", "About Launchy", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(OptionsDlg);
    } // retranslateUi

};

namespace Ui {
    class OptionsDlg: public Ui_OptionsDlg {};
} // namespace Ui

#endif // UI_OPTIONS_H
