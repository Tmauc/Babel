/*
** EPITECH PROJECT, 2019
** Ui_babel window class
** File description:
** class
*/

#include <iostream>

#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include <QScrollBar>
#include <QKeyEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

class Ui_BabelWindow
{
public:
    // CALL MENU
    QLabel *TitleCall;
    QLabel *StatusCall;
    QPushButton *ButtonQuit;

    // CONNEXION MENU
    QLabel *TitleWindow;
    QLineEdit *LineHostname;
    QLineEdit *LineUsername;
    QLineEdit *LinePort;
    QPushButton *ButtonConnect;

    // MAIN MENU
    QPushButton *ButtonCall;
    QPushButton *ButtonDisconnect;
    QLabel *WelcomeTitle;
    QLabel *ContactTitle;
    QLabel *Ex1;
    QLabel *Ex2;
    QStandardItemModel *_model = nullptr;
    QTableView *_contacts;
    QList<QStandardItem *> _items;

    Ui_BabelWindow(QWidget *p_w, QVBoxLayout *p_layout);
    ~Ui_BabelWindow(void);
    void setup_main_menu(std::map<std::string, std::pair<std::string, std::string>> allUser, std::string username);
    void update_contacts(std::map<std::string, std::pair<std::string, std::string>> allUser);
    void setup_connexion_menu(void);
    void setup_call_menu(void);
    void clear(QLayout *layout);

private:
    QWidget *_w;
    QVBoxLayout *_layout;
    QHBoxLayout *_mmLayout;
    QVBoxLayout *_titleLayout;
    QVBoxLayout *_scrollbarLayout;
    std::map<std::string, std::pair<std::string, std::string>> _allUser;
};
