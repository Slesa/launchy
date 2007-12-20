/********************************************************************************
** Form generated from reading ui file 'options.ui'
**
** Created: Wed Dec 19 12:27:23 2007
**      by: Qt User Interface Compiler version 4.3.2
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
#include <QtGui/QSlider>
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
    QVBoxLayout *vboxLayout2;
    QGroupBox *groupBox_4;
    QVBoxLayout *vboxLayout3;
    QCheckBox *genAlwaysShow;
    QCheckBox *genHideFocus;
    QCheckBox *genAlwaysTop;
    QCheckBox *genCondensed;
    QCheckBox *genCenter;
    QHBoxLayout *hboxLayout2;
    QLabel *label;
    QSpacerItem *spacerItem;
    QComboBox *genModifierBox;
    QSpacerItem *spacerItem1;
    QComboBox *genKeyBox;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;
    QGroupBox *groupBox_10;
    QVBoxLayout *vboxLayout4;
    QHBoxLayout *hboxLayout3;
    QVBoxLayout *vboxLayout5;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label_4;
    QVBoxLayout *vboxLayout6;
    QSlider *genOpaqueness;
    QSlider *genFadeIn;
    QSlider *genFadeOut;
    QHBoxLayout *hboxLayout4;
    QLabel *label_9;
    QSpacerItem *spacerItem4;
    QLineEdit *genMaxViewable;
    QSpacerItem *spacerItem5;
    QHBoxLayout *hboxLayout5;
    QLabel *label_8;
    QSpacerItem *spacerItem6;
    QLineEdit *genNumResults;
    QSpacerItem *spacerItem7;
    QHBoxLayout *hboxLayout6;
    QGroupBox *groupBox_6;
    QVBoxLayout *vboxLayout7;
    QCheckBox *genUpdateCheck;
    QCheckBox *genShowHidden;
    QCheckBox *genPortable;
    QCheckBox *genFastIndex;
    QHBoxLayout *hboxLayout7;
    QLabel *label_5;
    QLineEdit *genUpMinutes;
    QLabel *label_7;
    QSpacerItem *spacerItem8;
    QSpacerItem *spacerItem9;
    QSpacerItem *spacerItem10;
    QWidget *Skins;
    QVBoxLayout *vboxLayout8;
    QHBoxLayout *hboxLayout8;
    QGroupBox *groupBox_3;
    QHBoxLayout *hboxLayout9;
    QListWidget *skinList;
    QGroupBox *groupBox_8;
    QVBoxLayout *vboxLayout9;
    QLabel *authorInfo;
    QSpacerItem *spacerItem11;
    QGroupBox *groupBox_9;
    QVBoxLayout *vboxLayout10;
    QLabel *skinPreview;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout11;
    QHBoxLayout *hboxLayout10;
    QVBoxLayout *vboxLayout12;
    QGroupBox *groupBox_2;
    QVBoxLayout *vboxLayout13;
    QListWidget *catTypes;
    QLineEdit *catTypeEdit;
    QPushButton *catTypesPlus;
    QPushButton *catTypesMinus;
    QCheckBox *catCheckBinaries;
    QCheckBox *catCheckDirs;
    QHBoxLayout *hboxLayout11;
    QLabel *label_10;
    QSpinBox *catDepth;
    QSpacerItem *spacerItem12;
    QVBoxLayout *vboxLayout14;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout15;
    QListWidget *catDirectories;
    QHBoxLayout *hboxLayout12;
    QSpacerItem *spacerItem13;
    QPushButton *catDirPlus;
    QSpacerItem *spacerItem14;
    QPushButton *catDirMinus;
    QSpacerItem *spacerItem15;
    QSpacerItem *spacerItem16;
    QSpacerItem *spacerItem17;
    QVBoxLayout *vboxLayout16;
    QHBoxLayout *hboxLayout13;
    QPushButton *catRescan;
    QSpacerItem *spacerItem18;
    QLabel *catSize;
    QSpacerItem *spacerItem19;
    QProgressBar *catProgress;
    QWidget *tab_4;
    QVBoxLayout *vboxLayout17;
    QHBoxLayout *hboxLayout14;
    QGroupBox *groupBox_7;
    QVBoxLayout *vboxLayout18;
    QListWidget *plugList;
    QGroupBox *plugBox;
    QVBoxLayout *vboxLayout19;
    QSpacerItem *spacerItem20;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout20;
    QSpacerItem *spacerItem21;
    QHBoxLayout *hboxLayout15;
    QSpacerItem *spacerItem22;
    QLabel *aboutVer;
    QSpacerItem *spacerItem23;
    QHBoxLayout *hboxLayout16;
    QSpacerItem *spacerItem24;
    QLabel *label_2;
    QSpacerItem *spacerItem25;
    QSpacerItem *spacerItem26;
    QHBoxLayout *hboxLayout17;
    QSpacerItem *spacerItem27;
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
    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    groupBox_4 = new QGroupBox(tab);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    vboxLayout3 = new QVBoxLayout(groupBox_4);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    genAlwaysShow = new QCheckBox(groupBox_4);
    genAlwaysShow->setObjectName(QString::fromUtf8("genAlwaysShow"));

    vboxLayout3->addWidget(genAlwaysShow);

    genHideFocus = new QCheckBox(groupBox_4);
    genHideFocus->setObjectName(QString::fromUtf8("genHideFocus"));

    vboxLayout3->addWidget(genHideFocus);

    genAlwaysTop = new QCheckBox(groupBox_4);
    genAlwaysTop->setObjectName(QString::fromUtf8("genAlwaysTop"));
    genAlwaysTop->setEnabled(false);

    vboxLayout3->addWidget(genAlwaysTop);

    genCondensed = new QCheckBox(groupBox_4);
    genCondensed->setObjectName(QString::fromUtf8("genCondensed"));

    vboxLayout3->addWidget(genCondensed);

    genCenter = new QCheckBox(groupBox_4);
    genCenter->setObjectName(QString::fromUtf8("genCenter"));

    vboxLayout3->addWidget(genCenter);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    label = new QLabel(groupBox_4);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout2->addWidget(label);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem);

    genModifierBox = new QComboBox(groupBox_4);
    genModifierBox->setObjectName(QString::fromUtf8("genModifierBox"));

    hboxLayout2->addWidget(genModifierBox);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);

    genKeyBox = new QComboBox(groupBox_4);
    genKeyBox->setObjectName(QString::fromUtf8("genKeyBox"));

    hboxLayout2->addWidget(genKeyBox);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem2);


    vboxLayout3->addLayout(hboxLayout2);


    vboxLayout2->addWidget(groupBox_4);

    spacerItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(spacerItem3);


    hboxLayout1->addLayout(vboxLayout2);

    groupBox_10 = new QGroupBox(tab);
    groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
    vboxLayout4 = new QVBoxLayout(groupBox_10);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    vboxLayout5 = new QVBoxLayout();
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    label_6 = new QLabel(groupBox_10);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    vboxLayout5->addWidget(label_6);

    label_3 = new QLabel(groupBox_10);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    vboxLayout5->addWidget(label_3);

    label_4 = new QLabel(groupBox_10);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    vboxLayout5->addWidget(label_4);


    hboxLayout3->addLayout(vboxLayout5);

    vboxLayout6 = new QVBoxLayout();
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    genOpaqueness = new QSlider(groupBox_10);
    genOpaqueness->setObjectName(QString::fromUtf8("genOpaqueness"));
    genOpaqueness->setMaximum(100);
    genOpaqueness->setOrientation(Qt::Horizontal);

    vboxLayout6->addWidget(genOpaqueness);

    genFadeIn = new QSlider(groupBox_10);
    genFadeIn->setObjectName(QString::fromUtf8("genFadeIn"));
    genFadeIn->setMaximum(1000);
    genFadeIn->setSingleStep(10);
    genFadeIn->setOrientation(Qt::Horizontal);

    vboxLayout6->addWidget(genFadeIn);

    genFadeOut = new QSlider(groupBox_10);
    genFadeOut->setObjectName(QString::fromUtf8("genFadeOut"));
    genFadeOut->setMaximum(1000);
    genFadeOut->setSingleStep(10);
    genFadeOut->setOrientation(Qt::Horizontal);
    genFadeOut->setTickPosition(QSlider::NoTicks);

    vboxLayout6->addWidget(genFadeOut);


    hboxLayout3->addLayout(vboxLayout6);


    vboxLayout4->addLayout(hboxLayout3);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    label_9 = new QLabel(groupBox_10);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    hboxLayout4->addWidget(label_9);

    spacerItem4 = new QSpacerItem(16, 16, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem4);

    genMaxViewable = new QLineEdit(groupBox_10);
    genMaxViewable->setObjectName(QString::fromUtf8("genMaxViewable"));
    genMaxViewable->setMaximumSize(QSize(40, 16777215));

    hboxLayout4->addWidget(genMaxViewable);

    spacerItem5 = new QSpacerItem(16, 16, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem5);


    vboxLayout4->addLayout(hboxLayout4);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    label_8 = new QLabel(groupBox_10);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    hboxLayout5->addWidget(label_8);

    spacerItem6 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem6);

    genNumResults = new QLineEdit(groupBox_10);
    genNumResults->setObjectName(QString::fromUtf8("genNumResults"));
    genNumResults->setMaximumSize(QSize(40, 16777215));

    hboxLayout5->addWidget(genNumResults);

    spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem7);


    vboxLayout4->addLayout(hboxLayout5);


    hboxLayout1->addWidget(groupBox_10);


    vboxLayout1->addLayout(hboxLayout1);

    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    groupBox_6 = new QGroupBox(tab);
    groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
    vboxLayout7 = new QVBoxLayout(groupBox_6);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    genUpdateCheck = new QCheckBox(groupBox_6);
    genUpdateCheck->setObjectName(QString::fromUtf8("genUpdateCheck"));

    vboxLayout7->addWidget(genUpdateCheck);

    genShowHidden = new QCheckBox(groupBox_6);
    genShowHidden->setObjectName(QString::fromUtf8("genShowHidden"));

    vboxLayout7->addWidget(genShowHidden);

    genPortable = new QCheckBox(groupBox_6);
    genPortable->setObjectName(QString::fromUtf8("genPortable"));

    vboxLayout7->addWidget(genPortable);

    genFastIndex = new QCheckBox(groupBox_6);
    genFastIndex->setObjectName(QString::fromUtf8("genFastIndex"));
    genFastIndex->setEnabled(false);

    vboxLayout7->addWidget(genFastIndex);

    hboxLayout7 = new QHBoxLayout();
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    label_5 = new QLabel(groupBox_6);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    hboxLayout7->addWidget(label_5);

    genUpMinutes = new QLineEdit(groupBox_6);
    genUpMinutes->setObjectName(QString::fromUtf8("genUpMinutes"));
    genUpMinutes->setMaximumSize(QSize(40, 16777215));

    hboxLayout7->addWidget(genUpMinutes);

    label_7 = new QLabel(groupBox_6);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    hboxLayout7->addWidget(label_7);

    spacerItem8 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout7->addItem(spacerItem8);


    vboxLayout7->addLayout(hboxLayout7);


    hboxLayout6->addWidget(groupBox_6);

    spacerItem9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout6->addItem(spacerItem9);


    vboxLayout1->addLayout(hboxLayout6);

    spacerItem10 = new QSpacerItem(524, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem10);

    tabWidget->addTab(tab, QString());
    Skins = new QWidget();
    Skins->setObjectName(QString::fromUtf8("Skins"));
    vboxLayout8 = new QVBoxLayout(Skins);
    vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
    hboxLayout8 = new QHBoxLayout();
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    groupBox_3 = new QGroupBox(Skins);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setMinimumSize(QSize(0, 0));
    groupBox_3->setMaximumSize(QSize(160, 16777215));
    hboxLayout9 = new QHBoxLayout(groupBox_3);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    skinList = new QListWidget(groupBox_3);
    skinList->setObjectName(QString::fromUtf8("skinList"));
    skinList->setMaximumSize(QSize(150, 16777215));

    hboxLayout9->addWidget(skinList);


    hboxLayout8->addWidget(groupBox_3);

    groupBox_8 = new QGroupBox(Skins);
    groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
    groupBox_8->setMinimumSize(QSize(300, 166));
    vboxLayout9 = new QVBoxLayout(groupBox_8);
    vboxLayout9->setObjectName(QString::fromUtf8("vboxLayout9"));
    authorInfo = new QLabel(groupBox_8);
    authorInfo->setObjectName(QString::fromUtf8("authorInfo"));
    authorInfo->setScaledContents(true);
    authorInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    vboxLayout9->addWidget(authorInfo);

    spacerItem11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    vboxLayout9->addItem(spacerItem11);


    hboxLayout8->addWidget(groupBox_8);


    vboxLayout8->addLayout(hboxLayout8);

    groupBox_9 = new QGroupBox(Skins);
    groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
    groupBox_9->setMinimumSize(QSize(0, 150));
    vboxLayout10 = new QVBoxLayout(groupBox_9);
    vboxLayout10->setObjectName(QString::fromUtf8("vboxLayout10"));
    skinPreview = new QLabel(groupBox_9);
    skinPreview->setObjectName(QString::fromUtf8("skinPreview"));
    skinPreview->setScaledContents(false);
    skinPreview->setAlignment(Qt::AlignCenter);

    vboxLayout10->addWidget(skinPreview);


    vboxLayout8->addWidget(groupBox_9);

    tabWidget->addTab(Skins, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    vboxLayout11 = new QVBoxLayout(tab_2);
    vboxLayout11->setObjectName(QString::fromUtf8("vboxLayout11"));
    hboxLayout10 = new QHBoxLayout();
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    vboxLayout12 = new QVBoxLayout();
    vboxLayout12->setObjectName(QString::fromUtf8("vboxLayout12"));
    groupBox_2 = new QGroupBox(tab_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
    groupBox_2->setSizePolicy(sizePolicy);
    groupBox_2->setMaximumSize(QSize(115, 16777215));
    vboxLayout13 = new QVBoxLayout(groupBox_2);
    vboxLayout13->setObjectName(QString::fromUtf8("vboxLayout13"));
    catTypes = new QListWidget(groupBox_2);
    catTypes->setObjectName(QString::fromUtf8("catTypes"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(catTypes->sizePolicy().hasHeightForWidth());
    catTypes->setSizePolicy(sizePolicy1);
    catTypes->setMaximumSize(QSize(16777215, 16777215));

    vboxLayout13->addWidget(catTypes);

    catTypeEdit = new QLineEdit(groupBox_2);
    catTypeEdit->setObjectName(QString::fromUtf8("catTypeEdit"));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(catTypeEdit->sizePolicy().hasHeightForWidth());
    catTypeEdit->setSizePolicy(sizePolicy2);
    catTypeEdit->setMaximumSize(QSize(16777215, 16777215));

    vboxLayout13->addWidget(catTypeEdit);

    catTypesPlus = new QPushButton(groupBox_2);
    catTypesPlus->setObjectName(QString::fromUtf8("catTypesPlus"));
    catTypesPlus->setMaximumSize(QSize(16777215, 16));

    vboxLayout13->addWidget(catTypesPlus);

    catTypesMinus = new QPushButton(groupBox_2);
    catTypesMinus->setObjectName(QString::fromUtf8("catTypesMinus"));
    catTypesMinus->setMaximumSize(QSize(16777215, 16));

    vboxLayout13->addWidget(catTypesMinus);

    catCheckBinaries = new QCheckBox(groupBox_2);
    catCheckBinaries->setObjectName(QString::fromUtf8("catCheckBinaries"));

    vboxLayout13->addWidget(catCheckBinaries);

    catCheckDirs = new QCheckBox(groupBox_2);
    catCheckDirs->setObjectName(QString::fromUtf8("catCheckDirs"));

    vboxLayout13->addWidget(catCheckDirs);

    hboxLayout11 = new QHBoxLayout();
    hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
    label_10 = new QLabel(groupBox_2);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    hboxLayout11->addWidget(label_10);

    catDepth = new QSpinBox(groupBox_2);
    catDepth->setObjectName(QString::fromUtf8("catDepth"));
    catDepth->setMaximum(100000);
    catDepth->setValue(100);

    hboxLayout11->addWidget(catDepth);


    vboxLayout13->addLayout(hboxLayout11);


    vboxLayout12->addWidget(groupBox_2);

    spacerItem12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout12->addItem(spacerItem12);


    hboxLayout10->addLayout(vboxLayout12);

    vboxLayout14 = new QVBoxLayout();
    vboxLayout14->setObjectName(QString::fromUtf8("vboxLayout14"));
    groupBox = new QGroupBox(tab_2);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout15 = new QVBoxLayout(groupBox);
    vboxLayout15->setObjectName(QString::fromUtf8("vboxLayout15"));
    catDirectories = new QListWidget(groupBox);
    catDirectories->setObjectName(QString::fromUtf8("catDirectories"));

    vboxLayout15->addWidget(catDirectories);

    hboxLayout12 = new QHBoxLayout();
    hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
    spacerItem13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout12->addItem(spacerItem13);

    catDirPlus = new QPushButton(groupBox);
    catDirPlus->setObjectName(QString::fromUtf8("catDirPlus"));

    hboxLayout12->addWidget(catDirPlus);

    spacerItem14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout12->addItem(spacerItem14);

    catDirMinus = new QPushButton(groupBox);
    catDirMinus->setObjectName(QString::fromUtf8("catDirMinus"));

    hboxLayout12->addWidget(catDirMinus);

    spacerItem15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout12->addItem(spacerItem15);


    vboxLayout15->addLayout(hboxLayout12);


    vboxLayout14->addWidget(groupBox);

    spacerItem16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout14->addItem(spacerItem16);


    hboxLayout10->addLayout(vboxLayout14);


    vboxLayout11->addLayout(hboxLayout10);

    spacerItem17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout11->addItem(spacerItem17);

    vboxLayout16 = new QVBoxLayout();
    vboxLayout16->setObjectName(QString::fromUtf8("vboxLayout16"));
    hboxLayout13 = new QHBoxLayout();
    hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
    catRescan = new QPushButton(tab_2);
    catRescan->setObjectName(QString::fromUtf8("catRescan"));

    hboxLayout13->addWidget(catRescan);

    spacerItem18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout13->addItem(spacerItem18);

    catSize = new QLabel(tab_2);
    catSize->setObjectName(QString::fromUtf8("catSize"));

    hboxLayout13->addWidget(catSize);

    spacerItem19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout13->addItem(spacerItem19);


    vboxLayout16->addLayout(hboxLayout13);

    catProgress = new QProgressBar(tab_2);
    catProgress->setObjectName(QString::fromUtf8("catProgress"));
    catProgress->setTextVisible(false);
    catProgress->setOrientation(Qt::Horizontal);

    vboxLayout16->addWidget(catProgress);


    vboxLayout11->addLayout(vboxLayout16);

    tabWidget->addTab(tab_2, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    vboxLayout17 = new QVBoxLayout(tab_4);
    vboxLayout17->setObjectName(QString::fromUtf8("vboxLayout17"));
    hboxLayout14 = new QHBoxLayout();
    hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
    groupBox_7 = new QGroupBox(tab_4);
    groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
    groupBox_7->setMaximumSize(QSize(141, 16777215));
    vboxLayout18 = new QVBoxLayout(groupBox_7);
    vboxLayout18->setObjectName(QString::fromUtf8("vboxLayout18"));
    plugList = new QListWidget(groupBox_7);
    plugList->setObjectName(QString::fromUtf8("plugList"));
    plugList->setSelectionRectVisible(false);

    vboxLayout18->addWidget(plugList);


    hboxLayout14->addWidget(groupBox_7);

    plugBox = new QGroupBox(tab_4);
    plugBox->setObjectName(QString::fromUtf8("plugBox"));
    vboxLayout19 = new QVBoxLayout(plugBox);
    vboxLayout19->setObjectName(QString::fromUtf8("vboxLayout19"));
    spacerItem20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    vboxLayout19->addItem(spacerItem20);


    hboxLayout14->addWidget(plugBox);


    vboxLayout17->addLayout(hboxLayout14);

    tabWidget->addTab(tab_4, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    vboxLayout20 = new QVBoxLayout(tab_3);
    vboxLayout20->setObjectName(QString::fromUtf8("vboxLayout20"));
    spacerItem21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout20->addItem(spacerItem21);

    hboxLayout15 = new QHBoxLayout();
    hboxLayout15->setObjectName(QString::fromUtf8("hboxLayout15"));
    spacerItem22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout15->addItem(spacerItem22);

    aboutVer = new QLabel(tab_3);
    aboutVer->setObjectName(QString::fromUtf8("aboutVer"));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);
    aboutVer->setFont(font);
    aboutVer->setWordWrap(false);

    hboxLayout15->addWidget(aboutVer);

    spacerItem23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout15->addItem(spacerItem23);


    vboxLayout20->addLayout(hboxLayout15);

    hboxLayout16 = new QHBoxLayout();
    hboxLayout16->setObjectName(QString::fromUtf8("hboxLayout16"));
    spacerItem24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout16->addItem(spacerItem24);

    label_2 = new QLabel(tab_3);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setWordWrap(false);
    label_2->setOpenExternalLinks(true);

    hboxLayout16->addWidget(label_2);

    spacerItem25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout16->addItem(spacerItem25);


    vboxLayout20->addLayout(hboxLayout16);

    spacerItem26 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout20->addItem(spacerItem26);

    tabWidget->addTab(tab_3, QString());

    vboxLayout->addWidget(tabWidget);

    hboxLayout17 = new QHBoxLayout();
    hboxLayout17->setObjectName(QString::fromUtf8("hboxLayout17"));
    spacerItem27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout17->addItem(spacerItem27);

    buttonBox = new QDialogButtonBox(OptionsDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    hboxLayout17->addWidget(buttonBox);


    vboxLayout->addLayout(hboxLayout17);

    QWidget::setTabOrder(genAlwaysShow, genHideFocus);
    QWidget::setTabOrder(genHideFocus, genAlwaysTop);
    QWidget::setTabOrder(genAlwaysTop, genCondensed);
    QWidget::setTabOrder(genCondensed, genCenter);
    QWidget::setTabOrder(genCenter, genModifierBox);
    QWidget::setTabOrder(genModifierBox, genKeyBox);
    QWidget::setTabOrder(genKeyBox, genUpdateCheck);
    QWidget::setTabOrder(genUpdateCheck, genShowHidden);
    QWidget::setTabOrder(genShowHidden, genPortable);
    QWidget::setTabOrder(genPortable, genFastIndex);
    QWidget::setTabOrder(genFastIndex, genUpMinutes);
    QWidget::setTabOrder(genUpMinutes, genOpaqueness);
    QWidget::setTabOrder(genOpaqueness, genFadeIn);
    QWidget::setTabOrder(genFadeIn, genFadeOut);
    QWidget::setTabOrder(genFadeOut, genMaxViewable);
    QWidget::setTabOrder(genMaxViewable, genNumResults);
    QWidget::setTabOrder(genNumResults, catTypes);
    QWidget::setTabOrder(catTypes, catTypeEdit);
    QWidget::setTabOrder(catTypeEdit, catTypesPlus);
    QWidget::setTabOrder(catTypesPlus, catTypesMinus);
    QWidget::setTabOrder(catTypesMinus, catCheckBinaries);
    QWidget::setTabOrder(catCheckBinaries, catCheckDirs);
    QWidget::setTabOrder(catCheckDirs, catDepth);
    QWidget::setTabOrder(catDepth, catDirectories);
    QWidget::setTabOrder(catDirectories, catDirPlus);
    QWidget::setTabOrder(catDirPlus, catDirMinus);
    QWidget::setTabOrder(catDirMinus, catRescan);
    QWidget::setTabOrder(catRescan, skinList);
    QWidget::setTabOrder(skinList, plugList);
    QWidget::setTabOrder(plugList, tabWidget);
    QWidget::setTabOrder(tabWidget, buttonBox);

    retranslateUi(OptionsDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), OptionsDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), OptionsDlg, SLOT(reject()));

    tabWidget->setCurrentIndex(0);


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
    genCenter->setText(QApplication::translate("OptionsDlg", "Always center Launchy", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("OptionsDlg", "Hotkey:", 0, QApplication::UnicodeUTF8));
    groupBox_10->setTitle(QApplication::translate("OptionsDlg", "GUI", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("OptionsDlg", "Opaqueness", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("OptionsDlg", "Fade in time", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("OptionsDlg", "Fade out time", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("OptionsDlg", "Simultaneously visible alternate suggestions", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("OptionsDlg", "Max. number of alternate suggestions", 0, QApplication::UnicodeUTF8));
    groupBox_6->setTitle(QApplication::translate("OptionsDlg", "System Options", 0, QApplication::UnicodeUTF8));
    genUpdateCheck->setText(QApplication::translate("OptionsDlg", "Check for updates on startup", 0, QApplication::UnicodeUTF8));
    genShowHidden->setText(QApplication::translate("OptionsDlg", "Show hidden files in browse mode", 0, QApplication::UnicodeUTF8));
    genPortable->setText(QApplication::translate("OptionsDlg", "Portable mode (USB stick)", 0, QApplication::UnicodeUTF8));
    genFastIndex->setText(QApplication::translate("OptionsDlg", "Use testing indexer?", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("OptionsDlg", "Minutes between catalog updates", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("OptionsDlg", "(0 for manual updates only)", 0, QApplication::UnicodeUTF8));
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
