/********************************************************************************
** Form generated from reading UI file 'setupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPDIALOG_H
#define UI_SETUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_SetupDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QComboBox *inputComboBox;
    QComboBox *outputComboBox;
    QLabel *inputLabel;
    QLabel *outputLabel;

    void setupUi(QDialog *SetupDialog)
    {
        if (SetupDialog->objectName().isEmpty())
            SetupDialog->setObjectName(QStringLiteral("SetupDialog"));
        SetupDialog->resize(360, 154);
        SetupDialog->setModal(true);
        buttonBox = new QDialogButtonBox(SetupDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(80, 110, 201, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        groupBox = new QGroupBox(SetupDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 341, 101));
        inputComboBox = new QComboBox(groupBox);
        inputComboBox->setObjectName(QStringLiteral("inputComboBox"));
        inputComboBox->setGeometry(QRect(90, 30, 241, 24));
        outputComboBox = new QComboBox(groupBox);
        outputComboBox->setObjectName(QStringLiteral("outputComboBox"));
        outputComboBox->setGeometry(QRect(90, 60, 241, 24));
        inputLabel = new QLabel(groupBox);
        inputLabel->setObjectName(QStringLiteral("inputLabel"));
        inputLabel->setGeometry(QRect(20, 34, 54, 20));
        outputLabel = new QLabel(groupBox);
        outputLabel->setObjectName(QStringLiteral("outputLabel"));
        outputLabel->setGeometry(QRect(20, 64, 54, 20));

        retranslateUi(SetupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SetupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SetupDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetupDialog);
    } // setupUi

    void retranslateUi(QDialog *SetupDialog)
    {
        SetupDialog->setWindowTitle(QApplication::translate("SetupDialog", "MIDI Setup", 0));
        groupBox->setTitle(QApplication::translate("SetupDialog", "MIDI Ports", 0));
        inputLabel->setText(QApplication::translate("SetupDialog", "Input:", 0));
        outputLabel->setText(QApplication::translate("SetupDialog", "Output", 0));
    } // retranslateUi

};

namespace Ui {
    class SetupDialog: public Ui_SetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPDIALOG_H
