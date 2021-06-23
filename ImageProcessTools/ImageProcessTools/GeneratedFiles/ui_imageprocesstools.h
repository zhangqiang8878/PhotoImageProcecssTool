/********************************************************************************
** Form generated from reading UI file 'imageprocesstools.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPROCESSTOOLS_H
#define UI_IMAGEPROCESSTOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "imageview.h"

QT_BEGIN_NAMESPACE

class Ui_ImageProcessToolsClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_region;
    QComboBox *comboBox_rmGroundFunc;
    ImageView *graphicsView;
    QPushButton *pushButton_trans;
    QPushButton *pushButton_rmGround;
    QPushButton *pushButton_return;
    QPushButton *pushButton_getText;
    QPushButton *pushButton_rotate;
    QPushButton *pushButton_project;
    QPushButton *pushButton_save;
    QPushButton *pushButton_load;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_merge;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageProcessToolsClass)
    {
        if (ImageProcessToolsClass->objectName().isEmpty())
            ImageProcessToolsClass->setObjectName(QStringLiteral("ImageProcessToolsClass"));
        ImageProcessToolsClass->resize(656, 702);
        ImageProcessToolsClass->setAcceptDrops(true);
        centralWidget = new QWidget(ImageProcessToolsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_region = new QPushButton(centralWidget);
        pushButton_region->setObjectName(QStringLiteral("pushButton_region"));
        pushButton_region->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        pushButton_region->setFont(font);

        gridLayout->addWidget(pushButton_region, 9, 1, 1, 1);

        comboBox_rmGroundFunc = new QComboBox(centralWidget);
        comboBox_rmGroundFunc->setObjectName(QStringLiteral("comboBox_rmGroundFunc"));
        comboBox_rmGroundFunc->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(comboBox_rmGroundFunc, 5, 1, 1, 1);

        graphicsView = new ImageView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 14, 1);

        pushButton_trans = new QPushButton(centralWidget);
        pushButton_trans->setObjectName(QStringLiteral("pushButton_trans"));
        pushButton_trans->setMinimumSize(QSize(0, 50));
        pushButton_trans->setFont(font);

        gridLayout->addWidget(pushButton_trans, 11, 1, 1, 1);

        pushButton_rmGround = new QPushButton(centralWidget);
        pushButton_rmGround->setObjectName(QStringLiteral("pushButton_rmGround"));
        pushButton_rmGround->setMinimumSize(QSize(0, 50));
        pushButton_rmGround->setFont(font);

        gridLayout->addWidget(pushButton_rmGround, 4, 1, 1, 1);

        pushButton_return = new QPushButton(centralWidget);
        pushButton_return->setObjectName(QStringLiteral("pushButton_return"));
        pushButton_return->setMinimumSize(QSize(0, 50));
        pushButton_return->setFont(font);

        gridLayout->addWidget(pushButton_return, 2, 1, 1, 1);

        pushButton_getText = new QPushButton(centralWidget);
        pushButton_getText->setObjectName(QStringLiteral("pushButton_getText"));
        pushButton_getText->setMinimumSize(QSize(0, 50));
        pushButton_getText->setFont(font);

        gridLayout->addWidget(pushButton_getText, 10, 1, 1, 1);

        pushButton_rotate = new QPushButton(centralWidget);
        pushButton_rotate->setObjectName(QStringLiteral("pushButton_rotate"));
        pushButton_rotate->setMinimumSize(QSize(0, 50));
        pushButton_rotate->setFont(font);

        gridLayout->addWidget(pushButton_rotate, 6, 1, 1, 1);

        pushButton_project = new QPushButton(centralWidget);
        pushButton_project->setObjectName(QStringLiteral("pushButton_project"));
        pushButton_project->setMinimumSize(QSize(0, 50));
        pushButton_project->setFont(font);

        gridLayout->addWidget(pushButton_project, 8, 1, 1, 1);

        pushButton_save = new QPushButton(centralWidget);
        pushButton_save->setObjectName(QStringLiteral("pushButton_save"));
        pushButton_save->setMinimumSize(QSize(0, 50));
        pushButton_save->setFont(font);

        gridLayout->addWidget(pushButton_save, 1, 1, 1, 1);

        pushButton_load = new QPushButton(centralWidget);
        pushButton_load->setObjectName(QStringLiteral("pushButton_load"));
        pushButton_load->setMinimumSize(QSize(0, 50));
        pushButton_load->setFont(font);

        gridLayout->addWidget(pushButton_load, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 13, 1, 1, 1);

        pushButton_merge = new QPushButton(centralWidget);
        pushButton_merge->setObjectName(QStringLiteral("pushButton_merge"));
        pushButton_merge->setMinimumSize(QSize(0, 50));
        pushButton_merge->setFont(font);

        gridLayout->addWidget(pushButton_merge, 3, 1, 1, 1);

        ImageProcessToolsClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ImageProcessToolsClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImageProcessToolsClass->setStatusBar(statusBar);

        retranslateUi(ImageProcessToolsClass);

        QMetaObject::connectSlotsByName(ImageProcessToolsClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageProcessToolsClass)
    {
        ImageProcessToolsClass->setWindowTitle(QApplication::translate("ImageProcessToolsClass", "\345\233\276\345\203\217\345\244\204\347\220\206\345\267\245\345\205\267\343\200\220\351\235\236\345\225\206\347\224\250\343\200\221@1206", Q_NULLPTR));
        pushButton_region->setText(QApplication::translate("ImageProcessToolsClass", "\345\214\272\345\237\237\346\210\252\345\217\226", Q_NULLPTR));
        pushButton_trans->setText(QApplication::translate("ImageProcessToolsClass", "\351\200\217\346\230\216\345\214\226", Q_NULLPTR));
        pushButton_rmGround->setText(QApplication::translate("ImageProcessToolsClass", "\345\216\273\350\203\214\346\231\257", Q_NULLPTR));
        pushButton_return->setText(QApplication::translate("ImageProcessToolsClass", "\346\222\244\351\224\200", Q_NULLPTR));
        pushButton_getText->setText(QApplication::translate("ImageProcessToolsClass", "\346\226\207\345\255\227\346\217\220\345\217\226", Q_NULLPTR));
        pushButton_rotate->setText(QApplication::translate("ImageProcessToolsClass", "\351\241\265\351\235\242\350\275\254\346\255\243", Q_NULLPTR));
        pushButton_project->setText(QApplication::translate("ImageProcessToolsClass", "\346\212\225\345\275\261\345\217\230\346\215\242", Q_NULLPTR));
        pushButton_save->setText(QApplication::translate("ImageProcessToolsClass", "\344\277\235\345\255\230\345\233\276\345\203\217", Q_NULLPTR));
        pushButton_load->setText(QApplication::translate("ImageProcessToolsClass", "\346\211\223\345\274\200\345\233\276\345\203\217", Q_NULLPTR));
        pushButton_merge->setText(QApplication::translate("ImageProcessToolsClass", "\345\220\210\345\271\266\345\233\276\345\203\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImageProcessToolsClass: public Ui_ImageProcessToolsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPROCESSTOOLS_H
