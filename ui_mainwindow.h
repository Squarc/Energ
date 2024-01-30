/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblSignalPower;
    QLabel *lblOperator;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *cbPort;
    QPushButton *pbConnectModem;
    QCheckBox *cbDisableSync;
    QCheckBox *cbDisableErrors;
    QCheckBox *cbDisableWarnings;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLabel *lblGEN1;
    QLabel *lblGEN2;
    QLabel *label_7;
    QCheckBox *cbG11;
    QLabel *label_8;
    QLabel *label_5;
    QCheckBox *cbG12;
    QLabel *lblGEN3;
    QCheckBox *cbG23;
    QCheckBox *cbG14;
    QCheckBox *cbG22;
    QCheckBox *cbG24;
    QCheckBox *cbG13;
    QCheckBox *cbG21;
    QCheckBox *cbG33;
    QCheckBox *cbG31;
    QCheckBox *cbG34;
    QCheckBox *cbG32;
    QLabel *label;
    QLabel *lblTemp1;
    QLabel *lblTemp2;
    QLabel *lblTemp3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QComboBox *cbLanguage;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *twEvents;
    QPushButton *btnTest;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(464, 400);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lblSignalPower = new QLabel(centralWidget);
        lblSignalPower->setObjectName(QStringLiteral("lblSignalPower"));
        lblSignalPower->setMinimumSize(QSize(24, 24));
        lblSignalPower->setMaximumSize(QSize(24, 24));
        lblSignalPower->setPixmap(QPixmap(QString::fromUtf8(":/connect0.png")));
        lblSignalPower->setScaledContents(true);

        horizontalLayout_3->addWidget(lblSignalPower);

        lblOperator = new QLabel(centralWidget);
        lblOperator->setObjectName(QStringLiteral("lblOperator"));

        horizontalLayout_3->addWidget(lblOperator);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        cbPort = new QComboBox(centralWidget);
        cbPort->setObjectName(QStringLiteral("cbPort"));

        horizontalLayout_2->addWidget(cbPort);

        pbConnectModem = new QPushButton(centralWidget);
        pbConnectModem->setObjectName(QStringLiteral("pbConnectModem"));

        horizontalLayout_2->addWidget(pbConnectModem);


        verticalLayout->addLayout(horizontalLayout_2);

        cbDisableSync = new QCheckBox(centralWidget);
        cbDisableSync->setObjectName(QStringLiteral("cbDisableSync"));

        verticalLayout->addWidget(cbDisableSync);

        cbDisableErrors = new QCheckBox(centralWidget);
        cbDisableErrors->setObjectName(QStringLiteral("cbDisableErrors"));

        verticalLayout->addWidget(cbDisableErrors);

        cbDisableWarnings = new QCheckBox(centralWidget);
        cbDisableWarnings->setObjectName(QStringLiteral("cbDisableWarnings"));

        verticalLayout->addWidget(cbDisableWarnings);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 0, 2, 1, 1);

        lblGEN1 = new QLabel(centralWidget);
        lblGEN1->setObjectName(QStringLiteral("lblGEN1"));

        gridLayout->addWidget(lblGEN1, 1, 0, 1, 1);

        lblGEN2 = new QLabel(centralWidget);
        lblGEN2->setObjectName(QStringLiteral("lblGEN2"));

        gridLayout->addWidget(lblGEN2, 2, 0, 1, 1);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 0, 3, 1, 1);

        cbG11 = new QCheckBox(centralWidget);
        cbG11->setObjectName(QStringLiteral("cbG11"));

        gridLayout->addWidget(cbG11, 1, 1, 1, 1);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 0, 4, 1, 1);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 1, 1, 1);

        cbG12 = new QCheckBox(centralWidget);
        cbG12->setObjectName(QStringLiteral("cbG12"));

        gridLayout->addWidget(cbG12, 1, 2, 1, 1);

        lblGEN3 = new QLabel(centralWidget);
        lblGEN3->setObjectName(QStringLiteral("lblGEN3"));

        gridLayout->addWidget(lblGEN3, 3, 0, 1, 1);

        cbG23 = new QCheckBox(centralWidget);
        cbG23->setObjectName(QStringLiteral("cbG23"));

        gridLayout->addWidget(cbG23, 2, 3, 1, 1);

        cbG14 = new QCheckBox(centralWidget);
        cbG14->setObjectName(QStringLiteral("cbG14"));

        gridLayout->addWidget(cbG14, 1, 4, 1, 1);

        cbG22 = new QCheckBox(centralWidget);
        cbG22->setObjectName(QStringLiteral("cbG22"));

        gridLayout->addWidget(cbG22, 2, 2, 1, 1);

        cbG24 = new QCheckBox(centralWidget);
        cbG24->setObjectName(QStringLiteral("cbG24"));

        gridLayout->addWidget(cbG24, 2, 4, 1, 1);

        cbG13 = new QCheckBox(centralWidget);
        cbG13->setObjectName(QStringLiteral("cbG13"));

        gridLayout->addWidget(cbG13, 1, 3, 1, 1);

        cbG21 = new QCheckBox(centralWidget);
        cbG21->setObjectName(QStringLiteral("cbG21"));

        gridLayout->addWidget(cbG21, 2, 1, 1, 1);

        cbG33 = new QCheckBox(centralWidget);
        cbG33->setObjectName(QStringLiteral("cbG33"));

        gridLayout->addWidget(cbG33, 3, 3, 1, 1);

        cbG31 = new QCheckBox(centralWidget);
        cbG31->setObjectName(QStringLiteral("cbG31"));

        gridLayout->addWidget(cbG31, 3, 1, 1, 1);

        cbG34 = new QCheckBox(centralWidget);
        cbG34->setObjectName(QStringLiteral("cbG34"));

        gridLayout->addWidget(cbG34, 3, 4, 1, 1);

        cbG32 = new QCheckBox(centralWidget);
        cbG32->setObjectName(QStringLiteral("cbG32"));

        gridLayout->addWidget(cbG32, 3, 2, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 5, 1, 1);

        lblTemp1 = new QLabel(centralWidget);
        lblTemp1->setObjectName(QStringLiteral("lblTemp1"));

        gridLayout->addWidget(lblTemp1, 1, 5, 1, 1);

        lblTemp2 = new QLabel(centralWidget);
        lblTemp2->setObjectName(QStringLiteral("lblTemp2"));

        gridLayout->addWidget(lblTemp2, 2, 5, 1, 1);

        lblTemp3 = new QLabel(centralWidget);
        lblTemp3->setObjectName(QStringLiteral("lblTemp3"));

        gridLayout->addWidget(lblTemp3, 3, 5, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        cbLanguage = new QComboBox(centralWidget);
        cbLanguage->setObjectName(QStringLiteral("cbLanguage"));

        horizontalLayout->addWidget(cbLanguage);


        verticalLayout->addLayout(horizontalLayout);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        twEvents = new QTableWidget(groupBox);
        if (twEvents->columnCount() < 4)
            twEvents->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        twEvents->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        twEvents->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        twEvents->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        twEvents->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        twEvents->setObjectName(QStringLiteral("twEvents"));
        twEvents->setDragDropOverwriteMode(false);
        twEvents->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(twEvents);


        verticalLayout->addWidget(groupBox);

        btnTest = new QPushButton(centralWidget);
        btnTest->setObjectName(QStringLiteral("btnTest"));

        verticalLayout->addWidget(btnTest);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\255\320\275\320\265\321\200\320\263\320\276\320\261\320\273\320\276\320\272", Q_NULLPTR));
        lblSignalPower->setText(QString());
        lblOperator->setText(QApplication::translate("MainWindow", "\320\235\320\265\321\202 \321\201\320\265\321\202\320\270", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\320\237\320\276\321\200\321\202 \320\274\320\276\320\264\320\265\320\274\320\260:", Q_NULLPTR));
        pbConnectModem->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", Q_NULLPTR));
        cbDisableSync->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \321\201\320\270\321\200\320\265\320\275\321\203 \321\201\320\270\320\275\321\205\321\200\320\276\320\275\320\270\320\267\320\260\321\206\320\270\320\270", Q_NULLPTR));
        cbDisableErrors->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \321\201\320\270\321\200\320\265\320\275\321\203 \320\276\321\210\320\270\320\261\320\272\320\270 \320\264\320\262\320\270\320\263\320\260\321\202\320\265\320\273\320\265\320\271", Q_NULLPTR));
        cbDisableWarnings->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \321\201\320\270\321\200\320\265\320\275\321\203 \320\277\321\200\320\265\320\264\321\203\320\277\321\200\320\265\320\266\320\264\320\265\320\275\320\270\321\217", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\320\236\320\261\321\200.\321\201\320\262.\320\220", Q_NULLPTR));
        lblGEN1->setText(QApplication::translate("MainWindow", "\320\220\320\263\321\200\320\265\320\263\320\260\321\202 1", Q_NULLPTR));
        lblGEN2->setText(QApplication::translate("MainWindow", "\320\220\320\263\321\200\320\265\320\263\320\260\321\202 2", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\320\236\321\210\320\270\320\261\320\272\320\260", Q_NULLPTR));
        cbG11->setText(QString());
        label_8->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264-\320\265", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\320\236\320\261\321\200.\321\201\320\262.", Q_NULLPTR));
        cbG12->setText(QString());
        lblGEN3->setText(QApplication::translate("MainWindow", "\320\220\320\263\321\200\320\265\320\263\320\260\321\202 3", Q_NULLPTR));
        cbG23->setText(QString());
        cbG14->setText(QString());
        cbG22->setText(QString());
        cbG24->setText(QString());
        cbG13->setText(QString());
        cbG21->setText(QString());
        cbG33->setText(QString());
        cbG31->setText(QString());
        cbG34->setText(QString());
        cbG32->setText(QString());
        label->setText(QApplication::translate("MainWindow", "\320\242\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\320\260", Q_NULLPTR));
        lblTemp1->setText(QString());
        lblTemp2->setText(QString());
        lblTemp3->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "\320\257\320\267\321\213\320\272", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\320\241\320\276\320\276\320\261\321\211\320\265\320\275\320\270\321\217", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = twEvents->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\320\242\320\270\320\277", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = twEvents->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = twEvents->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "\320\233\320\276\320\272\320\260\321\206\320\270\321\217", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = twEvents->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "\320\242\320\265\320\272\321\201\321\202", Q_NULLPTR));
        btnTest->setText(QApplication::translate("MainWindow", "\320\242\320\265\321\201\321\202 \321\201\320\270\321\200\320\265\320\275\321\213", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
