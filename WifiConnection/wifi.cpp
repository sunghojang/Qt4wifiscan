#include "wifi.h"
#include "ui_wifi.h"

WIFI::WIFI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WIFI)
{
    ui->setupUi(this);
    system("killall wpa_supplicant");
    system("ifconfig eth0 down");
    system("ifconfig wlan0 down");
    system("ifconfig wlan0 up");
//    findTimer = new QTimer();
//    findTimer->setInterval(1000);
//    connect(findTimer,SIGNAL(timeout()),this,SLOT(findActiveWirelesses()));
//    findTimer->start();
//    findActiveWirelesses();
    findSSIDTimer = new QTimer();
    findSSIDTimer->setInterval(5000);
    connect(findSSIDTimer,SIGNAL(timeout()),this,SLOT(findActiveWirelessesSSID()));
    findSSIDTimer->start();
    findActiveWirelessesSSID();
}

WIFI::~WIFI()
{
    delete ui;
}
void WIFI::findActiveWirelesses()
{
    QNetworkConfigurationManager ncm;
    netcfgList = ncm.allConfigurations();

    WiFisList.clear();
    ui->m_label_connectshow->setText("...");
    for (int i=0;i< netcfgList.count();i++)
    {
        qDebug()<<netcfgList.at(i).name();
        if(netcfgList.at(i).name() == "wlan0"){
            system("ifconfig wlan0 up");
            ui->m_label_connectshow->setText("Connect!");
            break;
        }
    }
}
void WIFI::findActiveWirelessesSSID(){
    int foundCount = 0;
    QByteArray line;
    QTreeWidgetItem *item[125];

    system("rm wifilist.txt");
    //system("iwlist wlan0 scan | grep SSID >> wifilist.txt");
    system("iwlist wlan0 scan  >> wifilist.txt");
    QProcess::execute("gedit /home/oDx/Documents/a.txt");
    QFile file("wifilist.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    m_SplitString.clear();
    while (!file.atEnd()) {
        line = file.readLine();
        DataAsString = QString::fromAscii(line.data());
        //m_SplitString << DataAsString.section('"',1,1,QString::SectionSkipEmpty);
        if(!DataAsString.section("ESSID",1,1,QString::SectionSkipEmpty).isEmpty()){
            m_SplitString<<"tap";
            m_SplitString<<DataAsString.section("ESSID",1,1,QString::SectionSkipEmpty).section('"',1,1,QString::SectionSkipEmpty);
        }
        if(!DataAsString.section("IE:",1,1,QString::SectionSkipEmpty).isEmpty()){
            if(DataAsString.section("IE:",1,1).section("Version",0,0).section(' ',1,2).section('\n',0,0).section("Unknown",1,1).isEmpty()){
                m_SplitString<<DataAsString.section("IE:",1,1).section("Version",0,0).section(' ',1,2).section('\n',0,0);
            }else{
                ;//m_SplitString<<"Unknown";
            }
        }
        if(!DataAsString.section("Authentication Suites",1,1,QString::SectionSkipEmpty).isEmpty())
            m_SplitString<<DataAsString.section("Authentication Suites",1,1,QString::SectionSkipEmpty).section(':',1,1,QString::SectionSkipEmpty).section(' ',1,1).section('\n',0,0);
        if(!DataAsString.section("Signal level=-",1,1,QString::SectionSkipEmpty).isEmpty()){
            m_SplitString<<"SIGLEVEL";
            m_SplitString<<DataAsString.section("Signal level=-",1,1,QString::SectionSkipEmpty).section(' ',0,0);
        }
    }

    ui->treeWidgetWiFis->clear();
    int j =1;
    for(int k = 0 ; k < 125 ; k++){
        item[k] = new QTreeWidgetItem();
        item[k]->setTextAlignment(0,Qt::AlignVCenter);
        item[k]->setTextAlignment(1,Qt::AlignHCenter);
        item[k]->setTextAlignment(2,Qt::AlignHCenter);
        item[k]->setTextAlignment(3,Qt::AlignHCenter);
        item[k]->setTextAlignment(4,Qt::AlignHCenter);
        item[k]->setTextAlignment(5,Qt::AlignHCenter);
    }
    for(int l = 0 ; l < ui->treeWidgetWiFis->headerItem()->columnCount() ; l++){
        ui->treeWidgetWiFis->resizeColumnToContents(l);
    }
    for(int i=0; i<m_SplitString.size();i++)
    {
        //bool exist = false;
        //        for(int j=0; j< ui->treeWidgetWiFis->topLevelItemCount(); j++)
        //        {
        //            QTreeWidgetItem *index = ui->treeWidgetWiFis->topLevelItem(j);
        //            QString str = index->text(1);
        //            if(str == m_SplitString[i])
        //            {
        //                exist = true;
        //                break;
        //            }
        //        }
        //        if(!exist)
        //        {
        if(m_SplitString[i] == "tap" && i>0){
            foundCount++;
            item[foundCount]->setText(0,QString::number(foundCount));
            ui->treeWidgetWiFis->addTopLevelItem(item[foundCount]);
            j = 1;
            continue;
        }else if(i == 0){
            continue;
        }else if(m_SplitString[i] == "SIGLEVEL"){
            i++;
            if(m_SplitString[i].toInt() < 67){
                item[foundCount]->setBackgroundColor(5,Qt::blue);
            }else{
                item[foundCount]->setBackgroundColor(5,Qt::red);
            }
            item[foundCount]->setText(5,m_SplitString[i]);
            item[foundCount]->setTextColor(5,Qt::yellow);
            continue;
        }
        item[foundCount]->setText(j++,m_SplitString[i]);
        //        }
    }
}
void WIFI::on_m_button_connect_clicked()
{
    QString temp;
    QByteArray line;
    QStringList tempList;
    system("killall wpa_supplicant");
//    qDebug()<<"ifconfig wlan0 192.168.1.18 netmask 255.255.255.0";
//    system("ifconfig wlan0 192.168.1.18 netmask 255.255.255.0");
//    qDebug()<<"route add default gw 192.168.1.1 dev wlan0";
//    system("route add default gw 192.168.1.1 dev wlan0");

//    qDebug()<<"ifconfig wlan0 192.168.111.18 netmask 255.255.255.0";
//    system("ifconfig wlan0 192.168.111.18 netmask 255.255.255.0");
//    qDebug()<<"route add default gw 192.168.111.1 dev wlan0";
//    system("route add default gw 192.168.111.1 dev wlan0");
    system("ifconfig");
    system("iwconfig");

    //./wpa_passphrase essid password >> 파일명.conf
    system("rm temp");
    temp = "wpa_passphrase "+ui->m_label_selectSSID->text()+" "+ui->lineedit_password->text()+" >>"+"temp";
    qDebug()<<temp;
    system(temp.toAscii());
    system("ifconfig");
    system("iwconfig");

    temp = "iwconfig wlan0 essid "+ui->m_label_selectSSID->text();
    qDebug()<<temp;
    system(temp.toAscii());
    system("ifconfig");
    system("iwconfig");
//    QFile file2("temp");
//    if (!file2.open(QIODevice::ReadWrite | QIODevice::Text))
//        return;
//    while (!file2.atEnd()) {
//        line = file2.readLine();
//        qDebug()<<line;
//        temp = QString::fromAscii(line.data());
//        tempList << temp;
//    }
//    file2.close();

//    qDebug()<<tempList.size();
//    for(int i = 0 ; i < tempList.size() ; i++){
//        qDebug()<<i<<tempList.at(i);
//    }
//    tempList.insert(2,"\tproto=WPA2");
//    tempList.insert(2,"\tkey_mgmt=WPA-PSK");
//    qDebug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[");
//    for(int i = 0 ; i < tempList.size() ; i++){
//        qDebug() << tempList.at(i);
//    }
    //    QFile file3("/root/wifi/"+m_label_selectSSID->text()+".conf");
    //    if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))
    //        return;
    //    for(int i =0 ; i <tempList.size() ; i++){
    //        file3.write(tempList.at(i).data());
    //    }
temp = "wpa_supplicant -iwlan0 -c temp &";
qDebug()<<temp;
system(temp.toAscii());
system("ifconfig");
system("iwconfig");
system("dhcpcd wlan0");
system("ifconfig");
system("iwconfig");
}

void WIFI::on_treeWidgetWiFis_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->m_label_selectSSID->setText(item->data(1,0).toString());
}

void WIFI::on_m_button_stop_clicked()
{
    findSSIDTimer->stop();
}

void WIFI::on_m_button_check_clicked()
{
    system("ping -W 3 -c 5 192.168.34.1");
    system("ping -W 3 -c 5 192.168.1.1");
}

void WIFI::on_m_button_delete_clicked()
{
    system("ifconfig");
    system("iwconfig");
}

void WIFI::on_m_button_disconnect_clicked()
{
    system("killall wpa_supplicant");
    system("iwconfig wlan0 essid NNN");
}
