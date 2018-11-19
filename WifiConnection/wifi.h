#ifndef WIFI_H
#define WIFI_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QInputDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QFile>
#include <QProcess>
#include <QTreeWidgetItem>

namespace Ui {
class WIFI;
}

class WIFI : public QWidget
{
    Q_OBJECT

public:
    explicit WIFI(QWidget *parent = 0);
    ~WIFI();
public:

    QString DataAsString;
    QStringList m_SplitString;
    QNetworkConfiguration netcfg;
    QStringList WiFisList;
    QList<QNetworkConfiguration> netcfgList;
public slots:
    void findActiveWirelesses();
    void findActiveWirelessesSSID();
private slots:
    void on_m_button_connect_clicked();

    //void on_treeWidgetWiFis_pressed(const QModelIndex &index);

    void on_treeWidgetWiFis_itemClicked(QTreeWidgetItem *item, int column);

    void on_m_button_stop_clicked();

    void on_m_button_check_clicked();

    void on_m_button_delete_clicked();

    void on_m_button_disconnect_clicked();

private:
    enum {Wifi_Number = 0,Wifi_Name,Wifi_Security,Wifi_Rule1,Wifi_Security2,Wifi_Rule2};
    enum {Security_OPEN = 0,Security_SHARED,Security_WPAPSK,Security_WPA2PSK,Security_WPAWPA2PSK};
    Ui::WIFI *ui;
    QTimer *findTimer;
    QStandardItemModel* listModel;
    QNetworkSession *session;

    QTimer *findSSIDTimer;
};

#endif // WIFI_H
