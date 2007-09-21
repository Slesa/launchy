/********************************************************************************
** Form generated from reading ui file 'options.ui'
**
** Created: Thu Sep 20 22:34:46 2007
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
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

class Ui_OptionsDlg
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_3;
    QComboBox *genModifierBox;
    QComboBox *genKeyBox;
    QGroupBox *groupBox_4;
    QCheckBox *genAlwaysShow;
    QCheckBox *genAlwaysTop;
    QLineEdit *genNumResults;
    QLabel *label_8;
    QCheckBox *genHideFocus;
    QLabel *label_6;
    QCheckBox *genCondensed;
    QGroupBox *groupBox_5;
    QLineEdit *genUpMinutes;
    QLabel *label_5;
    QCheckBox *genFastIndex;
    QLabel *label_7;
    QGroupBox *groupBox_6;
    QCheckBox *genUpdateCheck;
    QCheckBox *genPortable;
    QCheckBox *genShowHidden;
    QWidget *Skins;
    QLabel *label_9;
    QLabel *authorInfo;
    QListWidget *skinList;
    QLabel *skinPreview;
    QWidget *tab_2;
    QPushButton *catRescan;
    QProgressBar *catProgress;
    QGroupBox *groupBox;
    QPushButton *catDirMinus;
    QListWidget *catDirectories;
    QPushButton *catDirPlus;
    QGroupBox *groupBox_2;
    QPushButton *catTypesMinus;
    QLineEdit *catTypeEdit;
    QPushButton *catTypesPlus;
    QListWidget *catTypes;
    QCheckBox *catCheckDirs;
    QCheckBox *catCheckBinaries;
    QSpinBox *catDepth;
    QLabel *label_10;
    QLabel *catSize;
    QWidget *tab_4;
    QListWidget *plugList;
    QLabel *label_11;
    QGroupBox *plugBox;

    void setupUi(QDialog *OptionsDlg)
    {
    if (OptionsDlg->objectName().isEmpty())
        OptionsDlg->setObjectName(QString::fromUtf8("OptionsDlg"));
    OptionsDlg->setWindowModality(Qt::NonModal);
    OptionsDlg->resize(582, 379);
    buttonBox = new QDialogButtonBox(OptionsDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(350, 340, 161, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    tabWidget = new QTabWidget(OptionsDlg);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setGeometry(QRect(10, 10, 561, 321));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    groupBox_3 = new QGroupBox(tab);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setGeometry(QRect(280, 20, 221, 71));
    genModifierBox = new QComboBox(groupBox_3);
    genModifierBox->setObjectName(QString::fromUtf8("genModifierBox"));
    genModifierBox->setGeometry(QRect(10, 30, 91, 22));
    genKeyBox = new QComboBox(groupBox_3);
    genKeyBox->setObjectName(QString::fromUtf8("genKeyBox"));
    genKeyBox->setGeometry(QRect(130, 30, 81, 22));
    groupBox_4 = new QGroupBox(tab);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    groupBox_4->setGeometry(QRect(10, 20, 251, 161));
    genAlwaysShow = new QCheckBox(groupBox_4);
    genAlwaysShow->setObjectName(QString::fromUtf8("genAlwaysShow"));
    genAlwaysShow->setGeometry(QRect(10, 20, 191, 18));
    genAlwaysTop = new QCheckBox(groupBox_4);
    genAlwaysTop->setObjectName(QString::fromUtf8("genAlwaysTop"));
    genAlwaysTop->setEnabled(false);
    genAlwaysTop->setGeometry(QRect(10, 60, 101, 18));
    genNumResults = new QLineEdit(groupBox_4);
    genNumResults->setObjectName(QString::fromUtf8("genNumResults"));
    genNumResults->setGeometry(QRect(10, 110, 41, 20));
    label_8 = new QLabel(groupBox_4);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(60, 110, 137, 21));
    genHideFocus = new QCheckBox(groupBox_4);
    genHideFocus->setObjectName(QString::fromUtf8("genHideFocus"));
    genHideFocus->setGeometry(QRect(10, 40, 191, 18));
    label_6 = new QLabel(groupBox_4);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(60, 130, 131, 16));
    genCondensed = new QCheckBox(groupBox_4);
    genCondensed->setObjectName(QString::fromUtf8("genCondensed"));
    genCondensed->setGeometry(QRect(10, 80, 191, 18));
    groupBox_5 = new QGroupBox(tab);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    groupBox_5->setGeometry(QRect(280, 120, 271, 81));
    genUpMinutes = new QLineEdit(groupBox_5);
    genUpMinutes->setObjectName(QString::fromUtf8("genUpMinutes"));
    genUpMinutes->setGeometry(QRect(10, 20, 41, 20));
    label_5 = new QLabel(groupBox_5);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(60, 20, 162, 21));
    genFastIndex = new QCheckBox(groupBox_5);
    genFastIndex->setObjectName(QString::fromUtf8("genFastIndex"));
    genFastIndex->setGeometry(QRect(10, 58, 211, 18));
    label_7 = new QLabel(groupBox_5);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(60, 40, 141, 16));
    groupBox_6 = new QGroupBox(tab);
    groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
    groupBox_6->setGeometry(QRect(10, 190, 251, 81));
    genUpdateCheck = new QCheckBox(groupBox_6);
    genUpdateCheck->setObjectName(QString::fromUtf8("genUpdateCheck"));
    genUpdateCheck->setGeometry(QRect(10, 20, 171, 18));
    genPortable = new QCheckBox(groupBox_6);
    genPortable->setObjectName(QString::fromUtf8("genPortable"));
    genPortable->setGeometry(QRect(10, 60, 151, 18));
    genShowHidden = new QCheckBox(groupBox_6);
    genShowHidden->setObjectName(QString::fromUtf8("genShowHidden"));
    genShowHidden->setGeometry(QRect(10, 40, 191, 18));
    tabWidget->addTab(tab, QString());
    Skins = new QWidget();
    Skins->setObjectName(QString::fromUtf8("Skins"));
    label_9 = new QLabel(Skins);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(40, 10, 61, 16));
    authorInfo = new QLabel(Skins);
    authorInfo->setObjectName(QString::fromUtf8("authorInfo"));
    authorInfo->setGeometry(QRect(160, 30, 321, 101));
    authorInfo->setScaledContents(true);
    authorInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    skinList = new QListWidget(Skins);
    skinList->setObjectName(QString::fromUtf8("skinList"));
    skinList->setGeometry(QRect(10, 30, 131, 101));
    skinPreview = new QLabel(Skins);
    skinPreview->setObjectName(QString::fromUtf8("skinPreview"));
    skinPreview->setGeometry(QRect(40, 140, 481, 151));
    skinPreview->setScaledContents(false);
    skinPreview->setAlignment(Qt::AlignCenter);
    tabWidget->addTab(Skins, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    catRescan = new QPushButton(tab_2);
    catRescan->setObjectName(QString::fromUtf8("catRescan"));
    catRescan->setGeometry(QRect(10, 244, 91, 23));
    catProgress = new QProgressBar(tab_2);
    catProgress->setObjectName(QString::fromUtf8("catProgress"));
    catProgress->setGeometry(QRect(10, 270, 529, 20));
    catProgress->setTextVisible(false);
    catProgress->setOrientation(Qt::Horizontal);
    groupBox = new QGroupBox(tab_2);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 10, 401, 211));
    catDirMinus = new QPushButton(groupBox);
    catDirMinus->setObjectName(QString::fromUtf8("catDirMinus"));
    catDirMinus->setGeometry(QRect(180, 180, 75, 23));
    catDirectories = new QListWidget(groupBox);
    catDirectories->setObjectName(QString::fromUtf8("catDirectories"));
    catDirectories->setGeometry(QRect(10, 20, 381, 151));
    catDirPlus = new QPushButton(groupBox);
    catDirPlus->setObjectName(QString::fromUtf8("catDirPlus"));
    catDirPlus->setGeometry(QRect(90, 180, 75, 23));
    groupBox_2 = new QGroupBox(tab_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(440, 10, 101, 251));
    catTypesMinus = new QPushButton(groupBox_2);
    catTypesMinus->setObjectName(QString::fromUtf8("catTypesMinus"));
    catTypesMinus->setGeometry(QRect(10, 160, 81, 16));
    catTypeEdit = new QLineEdit(groupBox_2);
    catTypeEdit->setObjectName(QString::fromUtf8("catTypeEdit"));
    catTypeEdit->setGeometry(QRect(10, 110, 81, 20));
    catTypesPlus = new QPushButton(groupBox_2);
    catTypesPlus->setObjectName(QString::fromUtf8("catTypesPlus"));
    catTypesPlus->setGeometry(QRect(10, 140, 81, 16));
    catTypes = new QListWidget(groupBox_2);
    catTypes->setObjectName(QString::fromUtf8("catTypes"));
    catTypes->setGeometry(QRect(10, 20, 81, 91));
    catCheckDirs = new QCheckBox(groupBox_2);
    catCheckDirs->setObjectName(QString::fromUtf8("catCheckDirs"));
    catCheckDirs->setGeometry(QRect(10, 200, 81, 18));
    catCheckBinaries = new QCheckBox(groupBox_2);
    catCheckBinaries->setObjectName(QString::fromUtf8("catCheckBinaries"));
    catCheckBinaries->setGeometry(QRect(10, 180, 81, 18));
    catDepth = new QSpinBox(groupBox_2);
    catDepth->setObjectName(QString::fromUtf8("catDepth"));
    catDepth->setGeometry(QRect(50, 220, 42, 22));
    catDepth->setMaximum(100000);
    catDepth->setValue(100);
    label_10 = new QLabel(groupBox_2);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(10, 220, 41, 21));
    catSize = new QLabel(tab_2);
    catSize->setObjectName(QString::fromUtf8("catSize"));
    catSize->setGeometry(QRect(225, 252, 118, 16));
    tabWidget->addTab(tab_2, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    plugList = new QListWidget(tab_4);
    plugList->setObjectName(QString::fromUtf8("plugList"));
    plugList->setGeometry(QRect(10, 30, 101, 251));
    plugList->setSelectionRectVisible(false);
    label_11 = new QLabel(tab_4);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(20, 10, 79, 16));
    plugBox = new QGroupBox(tab_4);
    plugBox->setObjectName(QString::fromUtf8("plugBox"));
    plugBox->setGeometry(QRect(130, 10, 411, 271));
    tabWidget->addTab(tab_4, QString());

    retranslateUi(OptionsDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), OptionsDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), OptionsDlg, SLOT(reject()));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(OptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *OptionsDlg)
    {
    OptionsDlg->setWindowTitle(QApplication::translate("OptionsDlg", "Launchy options", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("OptionsDlg", "Launchy Hotkey", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("OptionsDlg", "User Interface", 0, QApplication::UnicodeUTF8));
    genAlwaysShow->setText(QApplication::translate("OptionsDlg", "Always display Launchy window", 0, QApplication::UnicodeUTF8));
    genAlwaysTop->setText(QApplication::translate("OptionsDlg", "Always on top", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("OptionsDlg", "Number of suggestions in list", 0, QApplication::UnicodeUTF8));
    genHideFocus->setText(QApplication::translate("OptionsDlg", "Hide Launchy when it loses focus", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("OptionsDlg", "(1 for no dropdown menu)", 0, QApplication::UnicodeUTF8));
    genCondensed->setText(QApplication::translate("OptionsDlg", "Condense suggestion list", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("OptionsDlg", "Performance", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("OptionsDlg", "Minutes between catalog updates", 0, QApplication::UnicodeUTF8));
    genFastIndex->setText(QApplication::translate("OptionsDlg", "Use fast indexer? (uses more memory)", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("OptionsDlg", "(0 for manual updates only)", 0, QApplication::UnicodeUTF8));
    groupBox_6->setTitle(QApplication::translate("OptionsDlg", "System Options", 0, QApplication::UnicodeUTF8));
    genUpdateCheck->setText(QApplication::translate("OptionsDlg", "Check for updates on startup", 0, QApplication::UnicodeUTF8));
    genPortable->setText(QApplication::translate("OptionsDlg", "Portable mode (USB stick)", 0, QApplication::UnicodeUTF8));
    genShowHidden->setText(QApplication::translate("OptionsDlg", "Show hidden files in browse mode", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("OptionsDlg", "General", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("OptionsDlg", "Select a Skin", 0, QApplication::UnicodeUTF8));
    authorInfo->setText(QString());
    skinPreview->setText(QString());
    tabWidget->setTabText(tabWidget->indexOf(Skins), QApplication::translate("OptionsDlg", "Skins", 0, QApplication::UnicodeUTF8));
    catRescan->setText(QApplication::translate("OptionsDlg", "Rescan Catalog", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("OptionsDlg", "Directories", 0, QApplication::UnicodeUTF8));
    catDirMinus->setText(QApplication::translate("OptionsDlg", "-", 0, QApplication::UnicodeUTF8));
    catDirPlus->setText(QApplication::translate("OptionsDlg", "+", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("OptionsDlg", "File Types", 0, QApplication::UnicodeUTF8));
    catTypesMinus->setText(QApplication::translate("OptionsDlg", "-", 0, QApplication::UnicodeUTF8));
    catTypesPlus->setText(QApplication::translate("OptionsDlg", "+", 0, QApplication::UnicodeUTF8));
    catCheckDirs->setText(QApplication::translate("OptionsDlg", "Directories", 0, QApplication::UnicodeUTF8));
    catCheckBinaries->setText(QApplication::translate("OptionsDlg", "Executables", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("OptionsDlg", "Depth:", 0, QApplication::UnicodeUTF8));
    catSize->setText(QApplication::translate("OptionsDlg", "Indexing 0 items", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("OptionsDlg", "Catalog", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("OptionsDlg", "Available Plugins", 0, QApplication::UnicodeUTF8));
    plugBox->setTitle(QApplication::translate("OptionsDlg", "Plugin Options", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("OptionsDlg", "Plugins", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(OptionsDlg);
    } // retranslateUi

};

namespace Ui {
    class OptionsDlg: public Ui_OptionsDlg {};
} // namespace Ui

#endif // UI_OPTIONS_H
