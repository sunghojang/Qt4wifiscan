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
    findActiveWirelesses();

    //findActiveWirelessesSSID();
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
    //QProcess::execute("gedit /home/oDx/Documents/a.txt");
    QFile file("wifilist.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    m_SplitString.clear();
    while (!file.atEnd()) {
        line = file.readLine();
        DataAsString = QString::fromAscii(line.data());
        //m_SplitString << DataAsString.section('"',1,1,QString::SectionSkipEmpty);
        if(!DataAsString.section("ESSID",1,1,QString::SectionSkipEmpty).isEmpty()){ //iwlist 로 얻은 wifi 리스트를 구분하기 위해서 QStringList 에 "tap"을 인위적으로 넣어서 ESSID 구분
            m_SplitString<<"tap";
            m_SplitString<<DataAsString.section("ESSID",1,1,QString::SectionSkipEmpty).section('"',1,1,QString::SectionSkipEmpty);
            foundCount++;
        }
        if(!DataAsString.section("IE:",1,1,QString::SectionSkipEmpty).isEmpty()){   //보안정보를 얻기 위해서 "IE:"로 먼저 구분하고, "Version"으로 재구분
            if(DataAsString.section("IE:",1,1).section("Version",0,0).section(' ',1,2).section('\n',0,0).section("Unknown",1,1).isEmpty()){
                m_SplitString<<DataAsString.section("IE:",1,1).section("Version",0,0).section(' ',1,2).section('\n',0,0);
            }else{
                ;//m_SplitString<<"Unknown";    //와이파이 보안이 없을겨우 해당 else 문으로 빠집니다.
            }
        }
        if(!DataAsString.section("Authentication Suites",1,1,QString::SectionSkipEmpty).isEmpty())  //기타정보 얻기위한 "Authentication Suites" 로 구분
            m_SplitString<<DataAsString.section("Authentication Suites",1,1,QString::SectionSkipEmpty).section(':',1,1,QString::SectionSkipEmpty).section(' ',1,1).section('\n',0,0);
        if(!DataAsString.section("Signal level=-",1,1,QString::SectionSkipEmpty).isEmpty()){
            m_SplitString<<"SIGLEVEL";
            m_SplitString<<DataAsString.section("Signal level=-",1,1,QString::SectionSkipEmpty).section(' ',0,0);
        }
    }
    ui->treeWidgetWiFis->clear(); //TreeWidget Clear
    int j =1;
    for(int k = 0 ; k < foundCount ; k++){  //wifi를 찾은 만큼 아이템 갯수 추가
        item[k] = new QTreeWidgetItem();
        item[k]->setTextAlignment(Wifi_Number,Qt::AlignVCenter);
        item[k]->setTextAlignment(Wifi_Name,Qt::AlignHCenter);
        item[k]->setTextAlignment(Wifi_Security,Qt::AlignHCenter);
        item[k]->setTextAlignment(Wifi_Rule1,Qt::AlignHCenter);
        item[k]->setTextAlignment(Wifi_Security2,Qt::AlignHCenter);
        item[k]->setTextAlignment(Wifi_Rule2,Qt::AlignHCenter);
    }


    for(int l = 0 ; l < ui->treeWidgetWiFis->headerItem()->columnCount() ; l++){
        ui->treeWidgetWiFis->resizeColumnToContents(l);
    }
//    for(int i=0; i<m_SplitString.size();i++)
//    {
//        //bool exist = false;
//        //        for(int j=0; j< ui->treeWidgetWiFis->topLevelItemCount(); j++)
//        //        {
//        //            QTreeWidgetItem *index = ui->treeWidgetWiFis->topLevelItem(j);
//        //            QString str = index->text(1);
//        //            if(str == m_SplitString[i])
//        //            {
//        //                exist = true;
//        //                break;
//        //            }
//        //        }
//        //        if(!exist)
//        //        {
//        if(m_SplitString[i] == "tap" && i>0){
//            foundCount++;
//            item[foundCount]->setText(0,QString::number(foundCount));
//            ui->treeWidgetWiFis->addTopLevelItem(item[foundCount]);
//            j = 1;
//            continue;
//        }else if(i == 0){
//            continue;
//        }else if(m_SplitString[i] == "SIGLEVEL"){
//            i++;
//            if(m_SplitString[i].toInt() < 67){
//                item[foundCount]->setBackgroundColor(5,Qt::blue);
//            }else{
//                item[foundCount]->setBackgroundColor(5,Qt::red);
//            }
//            item[foundCount]->setText(5,m_SplitString[i]);
//            item[foundCount]->setTextColor(5,Qt::yellow);
//            continue;
//        }
//        item[foundCount]->setText(j++,m_SplitString[i]);
//        //        }
//    }
    foundCount = 0; //wifi를 찾은 만큼 아이템 갯수 추가
    for(int i=0; i<m_SplitString.size();i++)
    {
        if(m_SplitString[i] == "tap" && i>0){

            item[foundCount]->setText(0,QString::number(foundCount));
            ui->treeWidgetWiFis->addTopLevelItem(item[foundCount]);
            foundCount++;
            j = 1;
            continue;
        }else if(i == 0){
            continue;
        }else if(m_SplitString[i] == "SIGLEVEL"){
            i++;
            if(m_SplitString[i].toInt() > 100){
                item[foundCount]->setIcon(Wifi_Name,QIcon(QPixmap(":/wifi/signal/ping0.png")));
            }
            else if(m_SplitString[i].toInt() > 90){
                item[foundCount]->setIcon(Wifi_Name,QIcon(QPixmap(":/wifi/signal/ping1.png")));
            }
            else if(m_SplitString[i].toInt() > 80){
                item[foundCount]->setIcon(Wifi_Name,QIcon(QPixmap(":/wifi/signal/ping2.png")));
            }
            else if(m_SplitString[i].toInt() > 70){
                item[foundCount]->setIcon(Wifi_Name,QIcon(QPixmap(":/wifi/signal/ping3.png")));
            }
            else{
                item[foundCount]->setIcon(Wifi_Name,QIcon(QPixmap(":/wifi/signal/ping4.png")));
            }
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
//    system("ping -W 3 -c 5 192.168.34.1");
//    system("ping -W 3 -c 5 192.168.1.1");
    bool flag_wificonnect = true;
    int try_wificonnect = 1;
    QString DataAsString;
    QByteArray line;

    system("killall wpa_supplicant");
    system("iwconfig wlan0 essid NNN");
    system("wpa_supplicant -iwlan0 -c temp &");//wifi 설정을 저장한 temp 파일로 와이파이 활성화


    while(flag_wificonnect){
        system("iwconfig wlan0 > tempiw");
        QFile file("tempiw");

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            flag_wificonnect =false;
            ui->m_label_state->setText("Not connected F");
            //emit signal_iswificonnection(false);
        }else{
            while (!file.atEnd()) {
                line = file.readLine();
                DataAsString = QString::fromAscii(line.data());
                if(DataAsString.section("wlan0",1,1).section(" ",0,0,QString::SectionSkipEmpty).compare("unassociated")<0){
                    flag_wificonnect =false;
                    ui->m_label_state->setText("Connected");
                    //emit signal_iswificonnection(true);
                    break;
                }else{
                    break;
                }
            }
        }
        qDebug()<<"Try Wifi connect check"<<try_wificonnect;
        if(try_wificonnect >= 10){
            flag_wificonnect =false;
            ui->m_label_state->setText("Not connected ");
            //emit signal_iswificonnection(false);
        }
        sleep(1);
        try_wificonnect++;
    }
}

void WIFI::on_m_button_delete_clicked()
{
    findSSIDTimer = new QTimer();
    findSSIDTimer->setInterval(8000);
    connect(findSSIDTimer,SIGNAL(timeout()),this,SLOT(findActiveWirelessesSSID()));
    findSSIDTimer->start();
}

void WIFI::on_m_button_disconnect_clicked()
{
    system("killall wpa_supplicant");
    system("iwconfig wlan0 essid NNN");
}
