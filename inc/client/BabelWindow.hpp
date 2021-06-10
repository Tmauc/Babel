/*
** EPITECH PROJECT, 2019
** MainWindow.hpp
** File description:
** mainwindow
*/

#ifndef BABELWINDOW_H
#define BABELWINDOW_H

#include "Ui_BabelWindow.hpp"
#include "BabelClient.hpp"

#include <QtGui>
#include <QWidget>
#include <QMainWindow>

namespace Ui {
class BabelWindow;
}

class BabelWindow : public QMainWindow
{
public:
    std::string _username;

    explicit BabelWindow(QWidget *parent = nullptr);
    ~BabelWindow() {};
    void show(void);
    void clear(void);
    void updateContact(std::map<std::string, std::pair<std::string, std::string>> allUser);

private:
    QWidget *_w;
    QVBoxLayout *_layout;
    Ui_BabelWindow *_ui;
    BabelClient *_client = nullptr;

    void setupEvents_connexion(void);
    void setupEvents_main(void);
    void setupEvents_call(void);
    void on_connect_clicked(void);
    void on_call_clicked(void);
    void try_call_clicked(void);
    void on_disconnect_clicked(void);
    void on_quit_clicked(void);
};

#endif // MAINWINDOW_H
