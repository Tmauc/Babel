/*
** EPITECH PROJECT, 2019
** Ui babel window class
** File description:
** babel window class ui
*/

#include "Ui_BabelWindow.hpp"

Ui_BabelWindow::Ui_BabelWindow(QWidget *p_w, QVBoxLayout *p_layout)
    : _w(p_w), _layout(p_layout)
{
    this->setup_connexion_menu();
};

void Ui_BabelWindow::setup_connexion_menu(void)
{
    TitleWindow = new QLabel("BABEL CLIENT");
    QFont TitleFont;
    TitleFont.setPointSize(35);
    TitleWindow->setFont(TitleFont);
    this->_layout->addWidget(TitleWindow);

    QFont LineFont;
    LineFont.setPointSize(17);

    LineUsername = new QLineEdit();
    LineUsername->setFont(LineFont);
    LineUsername->setPlaceholderText("USERNAME");
    this->_layout->addWidget(LineUsername);

    LineHostname = new QLineEdit();
    LineHostname->setFont(LineFont);
    LineHostname->setPlaceholderText("HOSTNAME");
    this->_layout->addWidget(LineHostname);

    LinePort = new QLineEdit();
    LinePort->setFont(LineFont);
    LinePort->setPlaceholderText("PORT");
    this->_layout->addWidget(LinePort);

    QFont ButtonFont;
    ButtonFont.setPointSize(20);

    ButtonConnect = new QPushButton("Connect");
    ButtonConnect->setFont(ButtonFont);
    this->_layout->addWidget(ButtonConnect);
}

void Ui_BabelWindow::setup_call_menu(void)
{
    QFont TitleFont;

    TitleCall = new QLabel("Calling Monsieur");
    TitleFont.setPointSize(35);
    TitleCall->setFont(TitleFont);
    this->_layout->addWidget(TitleCall);

    StatusCall = new QLabel("Pending...");
    TitleFont.setPointSize(25);
    StatusCall->setFont(TitleFont);
    this->_layout->addWidget(StatusCall);

    QFont ButtonFont;
    ButtonFont.setPointSize(20);

    ButtonQuit = new QPushButton("Quit Call");
    ButtonQuit->setFont(ButtonFont);
    this->_layout->addWidget(ButtonQuit);
}

void Ui_BabelWindow::update_contacts(std::map<std::string, std::pair<std::string, std::string>> allUser)
{
    this->_model = new QStandardItemModel();
    this->_model->setHorizontalHeaderLabels({ "Name", "Office" });
    this->_contacts = new QTableView();
    this->_contacts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (auto it = allUser.begin(); it != allUser.end(); ++it) {
        this->_items.clear();
        this->_items.append(new QStandardItem(QString(it->second.first.c_str())));
        this->_items.append(new QStandardItem(QString(it->second.second.c_str())));
        this->_model->appendRow(this->_items);
    }
    this->_contacts->setModel(this->_model);
    this->_scrollbarLayout->addWidget(this->_contacts);
    this->_contacts->verticalHeader()->hide();
    this->_contacts->horizontalHeader()->setStretchLastSection(true);
}

void Ui_BabelWindow::setup_main_menu(std::map<std::string, std::pair<std::string, std::string>> allUser, std::string username)
{
    this->_mmLayout = new QHBoxLayout;
    this->_scrollbarLayout = new QVBoxLayout;
    this->_titleLayout = new QVBoxLayout;
    this->_layout->addLayout(this->_mmLayout);
    this->_mmLayout->addLayout(this->_titleLayout);
    this->_mmLayout->addLayout(this->_scrollbarLayout);
    
    QFont TitleFont;
    std::string titleString = "Wecome to Babel, " + username + " !\nWho do you wanna call ?";

    WelcomeTitle = new QLabel(titleString.c_str());
    TitleFont.setPointSize(35);
    WelcomeTitle->setFont(TitleFont);
    this->_titleLayout->addWidget(WelcomeTitle);

    ContactTitle = new QLabel("Contacts");
    TitleFont.setPointSize(35);
    WelcomeTitle->setFont(TitleFont);
    this->_scrollbarLayout->addWidget(ContactTitle);
    
    update_contacts(allUser);

    QFont ButtonFont;
    ButtonFont.setPointSize(20);

    ButtonCall = new QPushButton("Go to Call interface");
    ButtonCall->setFont(ButtonFont);
    this->_layout->addWidget(ButtonCall);

    ButtonDisconnect = new QPushButton("Quit");   
    ButtonDisconnect->setFont(ButtonFont);
    this->_layout->addWidget(ButtonDisconnect);

}

void Ui_BabelWindow::clear(QLayout *layout)
{
    QLayoutItem *item;

    while (item = layout->takeAt(0)) {
        if (item->widget()) {
            delete item->widget();
        }
        if (item->layout()) {
            this->clear(item->layout());
            delete item->layout();
        }
    }
}
