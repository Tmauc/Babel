/*
** EPITECH PROJECT, 2019
** babelwindow babel client
** File description:
** babelwindow
*/

#include "BabelWindow.hpp"

BabelWindow::BabelWindow(QWidget *parent) : QMainWindow(parent)
{   
    this->_w = new QWidget(parent, Qt::Window);
    this->_layout = new QVBoxLayout;
    this->_w->setLayout(this->_layout);
    this->_w->resize(1200, 800);
    this->_w->setWindowTitle("Babel Client");
    this->_ui = new Ui_BabelWindow(this->_w, this->_layout);
    this->setupEvents_connexion();
};

void BabelWindow::setupEvents_connexion(void)
{
    QObject::connect(this->_ui->ButtonConnect, &QPushButton::clicked,
                     this, &BabelWindow::on_connect_clicked);
};

void BabelWindow::setupEvents_main(void)
{
    QObject::connect(this->_ui->ButtonDisconnect, &QPushButton::clicked,
                    this, &BabelWindow::on_disconnect_clicked);
    QObject::connect(this->_ui->ButtonCall, &QPushButton::clicked,
                    this, &BabelWindow::on_call_clicked);
    QObject::connect(this->_ui->_contacts, &QTableView::doubleClicked,
                     this, &BabelWindow::try_call_clicked);
};

void BabelWindow::setupEvents_call(void)
{
    QObject::connect(this->_ui->ButtonQuit, &QPushButton::clicked,
                     this, &BabelWindow::on_quit_clicked);
};

void BabelWindow::show(void)
{
    return this->_w->show();
};

void BabelWindow::on_connect_clicked()
{
    std::string hostname = this->_ui->LineHostname->text().toUtf8().constData();
    std::string port = this->_ui->LinePort->text().toUtf8().constData();
    std::string username = this->_ui->LineUsername->text().toUtf8().constData();

    //!\ VERIFY IF HOSTNAME / PORT
    try {
        this->_client = new BabelClient(hostname, port, this);
        if (this->_client->login(username)) {
            this->_ui->clear(this->_layout);
            this->_username = username;
            this->_ui->setup_main_menu(this->_client->getAllUser(), this->_username);
            this->setupEvents_main();
        }
    } catch (std::exception &e) {
    }
};

void BabelWindow::on_disconnect_clicked()
{
    if (this->_client->logout(this->_username)) {
        this->_ui->clear(this->_layout);
        this->_w->close();
        delete _client;
    }
};

void BabelWindow::on_quit_clicked()
{
    this->_client->end_call();
    this->_ui->clear(this->_layout);
    this->_ui->setup_main_menu(this->_client->getAllUser(), this->_username);
    this->setupEvents_main();
};

void BabelWindow::try_call_clicked()
{
    int row_id = this->_ui->_contacts->selectionModel()->currentIndex().row();
    std::string username = this->_ui->_contacts->selectionModel()->selectedRows(row_id).value(0).data().toString().toStdString();
    this->_client->call_someone(username, "12056");
};

void BabelWindow::on_call_clicked()
{
    this->_ui->clear(this->_layout);
    this->_ui->setup_call_menu();
    this->setupEvents_call();
};

void BabelWindow::updateContact(std::map<std::string, std::pair<std::string, std::string>> allUser)
{
    _ui->update_contacts(allUser);
}
