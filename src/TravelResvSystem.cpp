#pragma once
#include <iostream>
#include <string>
#include <random>
#include<vector>
#include "exctSQL.h"

using namespace std;

int mainMenu();//���˵�
int childMenu(const std::string& str);//�Ӳ˵�
int queryCustomer(string& ID);//��ѯ���޹˿�
int insertCustomer(string ID, string name);//����˿�
int insertReserv(string ID, int Type, string Key);//����Ԥ��
void ReserveService(string& ID);//�˿ͷ���/Ԥ��
void QueryService(string& ID);//��ѯԤ��
void DeleteService(string& ID);//ɾ��Ԥ��
void RouteService(string& ID);//��ѯ·��
void CheckService(string& ID);//���·�ߺ�����
int getRandomNum();//Ϊ������Ż�ȡ�����
void showAllOrder(int Type);//չʾ����������Ϣ
void showQueriedFlights(MYSQL_RES* res);//�Բ�ͬ��ʽչʾ��ѯ�����
void showQueriedBus(MYSQL_RES* res);
void showQueriedHotels(MYSQL_RES* res);
int exist(string city, vector<string> passCity);//�ж�city�Ƿ������·��

int main() {
    std::string ID;

    while (true) {
        cout << "------------------��ӭ������������Ԥ��ϵͳ------------------" << std::endl;
        cout << "�����������û�ID��0.�˳�����";
        cin >> ID;
        if (ID == "0") {
            cout << "���˳�����...\n\n";
            break;
        }
        else {
            // �������
            if (!queryCustomer(ID)) {
                cout << "��ӭ�¿ͻ�ʹ�ñ�ϵͳ" << std::endl;
                string name;
                cout << "����������";
                cin >> name;
                if (insertCustomer(ID, name)) {
                    std::cout << "������Ϣ�ѵǼ�qwq!" << std::endl;
                }
            }
            int ch1;
            do {
                ch1 = mainMenu();
                switch (ch1) {
                case 1:
                    ReserveService(ID);
                    break;
                case 2:
                    QueryService(ID);
                    break;
                case 3:
                    DeleteService(ID);
                    break;
                case 4:
                    RouteService(ID);
                    break;
                case 5:
                    CheckService(ID);
                    break;
                default:
                    std::cout << "---------------���˳��û�����--------------\n\n";
                    break;
                }
            } while (ch1 >= 1 && ch1 <= 5);
        }
    }
    return 0;
}

// ���˵�
// ѡ��Ԥ�� / ��ѯ / ȡ��Ԥ��
int mainMenu() {
    int choice;//����������ܻ��������

    while (true) {
        cout << "+--------------TravelReserve--------------+" << endl;
        cout << "|  1.Ԥ������/��ͳ�/�Ƶ�                 |" << endl;
        cout << "|  2.��ѯ����/��ͳ�/�Ƶ�Ԥ����Ϣ         |" << endl;
        cout << "|  3.ȡ������/��ͳ�/�Ƶ�Ԥ��             |" << endl;
        cout << "|  4.��ѯ������·                         |" << endl;
        cout << "|  5.�����·������                       |" << endl;
        cout << "|  0.�˳�                                 |" << endl;
        cout << "+-----------------------------------------+" << endl;
        cout << "��ѡ�����(0/1/2/3/4/5): ";

        cin >> choice;

        if (choice >= 0 && choice <= 5) {
            return choice;
        }
        else {
            cout << "����������" << endl;
        }
    }
}

int childMenu(const std::string& str) {
    int choice;
    while (true) {
        std::cout << "1." << str << "���� 2." << str << "��ͳ� 3." << str << "�Ƶ꣨0.�˳���:" << std::endl;
        std::cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            return choice; // ������Ч��ѡ��
        }
        else if (choice == 0) {
            return choice;
        }
        else {
            std::cout << "����������" << std::endl;
        }
    }
}

int queryCustomer(string& ID) {//��ѯ���ݿ����Ƿ��Ѿ��иù˿�
    //���в�ѯ
    string sql = "select * from customers where custID = '"+ID+"';";
    int res = exctQuerySQL_B(sql);
    //cout << res << end;
    return res;
}

int insertCustomer(string ID, string name) {//�����ݿ��в���ù˿�
    string sql = "insert into customers values('" + ID + "', '" + name + "');";
    int res = exctInsertSQL(sql);
    return res;//1ִ�гɹ� 0ִ��ʧ��
}

void ReserveService(string& ID){//�û�Ԥ������
    while (true) {
        int ch2 = childMenu("Ԥ��");
        if (ch2 == 1) {
            showAllOrder(ch2);
            string sql = "SELECT * FROM FLIGHTS";
            string FromCity, ArivCity;
            cout << "���������/�ִ����(FromCity ArivCity):";
            cin >> FromCity >> ArivCity;
            if (!FromCity.empty() && !ArivCity.empty()) {
                sql = "SELECT * FROM FLIGHTS WHERE FromCity = '" + FromCity + "' AND ArivCity = '" + ArivCity + "';";
            }
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ��û������Ҫ�ĺ���\n" << std::endl;
            }
            else {
                cout << "���к�����Ϣ����:" << std::endl;
                //�����������:flight
                int num = mysql_num_fields(res);
                //�õ������е�����, �������
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                for (int i = 0; i < num; ++i)
                {
                    if (i > 1 && i < 4) cout << fields[i].name << "\t";
                    else cout << fields[i].name << "\t\t";
                }
                printf("\n");
                //��������������е���
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL)
                {
                    // ����ǰ���е�ÿһ����Ϣ����
                    for (int i = 0; i < num; ++i)
                    {
                        cout << row[i] << "\t\t";
                    }
                    cout << endl;
                }
                //�ͷ���Դ - �����
                mysql_free_result(res);
                while (true) {
                    cout << "��ѡ�񺽰���(����exitȡ��):\n";
                    string flightNum;
                    cin >> flightNum;
                    if (flightNum == "exit") {
                        break;
                    }
                    else {
                        string choice;
                        cout << "�Ƿ�ѡ��ú���:" + flightNum + "(Y/N)?\n";
                        cin >> choice;
                        if (choice == "Y" || choice == "y") {
                            //����һ����������
                            insertReserv(ID, 1, flightNum);
                            cout << "Ԥ���ɹ���\n\n";
                            break;
                        }
                        else {
                            cout << "������ȡ����������ѡ��\n";
                        }
                    }
                }
            }
        }
        else if (ch2 == 2) {
            showAllOrder(ch2);
            string sql = "SELECT * FROM BUS";
            string Location;
            cout << "���������(Location):";
            cin >> Location;
            if (!Location.empty()) {
                sql = "SELECT * FROM BUS WHERE LOCATION = '" + Location + "';";
            }
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ��û������Ҫ�Ĵ��\n" << std::endl;
            }
            else {
                cout << "���д����Ϣ����:" << std::endl;
                //�����������:bus
                int num = mysql_num_fields(res);
                //�õ������е�����, �������
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                for (int i = 0; i < num; ++i)
                {
                    //printf("%s\t\t", fields[i].name);
                    if (i == 1 || i == 3) cout << fields[i].name << "\t";
                    else cout << fields[i].name << "\t\t";
                }
                printf("\n");
                //��������������е���
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL)
                {
                    // ����ǰ���е�ÿһ����Ϣ����
                    for (int i = 0; i < num; ++i)
                    {
                        cout << row[i] << "\t\t";
                    }
                    printf("\n");
                }
                //�ͷ���Դ - �����
                mysql_free_result(res);

                while (true) {
                    cout << "��ѡ���ͱ��(����exitȡ��):\n";
                    string busNum;
                    cin >> busNum;
                    if (busNum == "exit") {
                        break;
                    }
                    else {
                        string choice;
                        cout << "�Ƿ�ѡ��ô��: " + busNum + " (Y/N)? \n";
                        cin >> choice;
                        if (choice == "Y" || choice == "y") {
                            //����һ����������
                            insertReserv(ID, 3, busNum);//ע����
                            cout << "Ԥ���ɹ���\n\n";
                            break;
                        }
                        else {
                            cout << "������ȡ����������ѡ��\n";
                        }
                    }
                }
            }
        }
        else if (ch2==3) {
            showAllOrder(ch2);
            string sql = "SELECT * FROM HOTELS";
            string Location;
            cout << "���������(Location):";
            cin >> Location;
            if (!Location.empty()) {
                sql = "SELECT * FROM HOTELS WHERE LOCATION = '" + Location + "';";
            }
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ��û������Ҫ�ľƵ�\n" << std::endl;
            }
            else {
                cout << "���оƵ���Ϣ����:" << std::endl;
                //�����������:bus
                int num = mysql_num_fields(res);
                //�õ������е�����, �������
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                for (int i = 0; i < num; ++i)
                {
                    //printf("%s\t\t", fields[i].name);
                    if (i == 3) cout << fields[i].name << "\t\t";
                    else cout << fields[i].name << "\t\t";
                }
                printf("\n");
                //��������������е���
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL)
                {
                    // ����ǰ���е�ÿһ����Ϣ����
                    for (int i = 0; i < num; ++i)
                    {
                        cout << row[i] << "\t\t\t";
                    }
                    printf("\n");
                }
                //�ͷ���Դ - �����
                mysql_free_result(res);
                while (true) {
                    cout << "��ѡ��Ƶ���(����exitȡ��):\n";
                    string hotelNum;
                    cin >> hotelNum;
                    if (hotelNum == "exit") {
                        break;
                    }
                    else {
                        string choice;
                        cout << "�Ƿ�ѡ��þƵ�: " + hotelNum + " (Y/N)? \n";
                        cin >> choice;
                        if (choice == "Y" || choice == "y") {
                            //����һ����������
                            insertReserv(ID, 2, hotelNum);//ע����
                            cout << "Ԥ���ɹ���\n\n";
                            break;
                        }
                        else {
                            cout << "������ȡ����������ѡ��\n";
                        }
                    }
                }
            }
        }
        else {
        cout << "���˳�����...\n\n";
        break;
        }
    }
}

void QueryService(string& ID) {//��ѯ���е�Ԥ����Ϣ
    while (true) {
        int ch2 = childMenu("��ѯԤ��");
        if (ch2 == 1) {//��ѯ����Ԥ��
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as flightNum,FromCity,ArivCity FROM reservations NATURAL JOIN customers,flights WHERE resvType=1 AND custID='" + ID + "' AND resvKey=flightNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ������û��Ԥ���κκ���\n" << std::endl;
            }
            else {
                showQueriedFlights(res);
            }
        }
        else if (ch2 == 2) {//��ѯ�����Ϣ
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as busNum,location FROM reservations NATURAL JOIN customers,bus WHERE resvType=3 AND custID='" + ID + "' AND resvKey=busNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ����û��Ԥ���κδ��\n" << std::endl;
            }
            else {
                showQueriedBus(res);
            }
        }
        else if (ch2 == 3) {
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as hotelNum,location FROM reservations NATURAL JOIN customers,hotels WHERE resvType=2 AND custID='" + ID + "' AND resvKey=hotelNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ����û��Ԥ���κξƵ�\n" << std::endl;
            }
            else {
                showQueriedHotels(res);
            }
        }
        else {
            cout << "���˳�����...\n\n";
            break;
        }
    }
}

void DeleteService(string& ID) {//ɾ�����е�Ԥ����Ϣ
    while (true) {
        int ch2 = childMenu("ȡ��Ԥ��");
        string resvNum;
        if (ch2 != 0) {
            cout << "\n��������Ҫȡ����Ԥ�����(��λԤ����� 0.�˳�)��";
            cin >> resvNum;
            if (resvNum == "0") {
                cout << "���˳�����...\n\n";
                break;
            }
        }
        if (ch2 == 1) {//ɾ������Ԥ��
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as flightNum,FromCity,ArivCity FROM reservations NATURAL JOIN customers,flights WHERE resvType=1 AND custID='" + ID + "' AND resvNum='" + resvNum + "' AND resvKey=flightNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ��δ�鵽��Ԥ����Ϣ\n" << std::endl;
            }
            else {
                showQueriedFlights(res);
                cout << "�Ƿ�ȡ����ЩԤ������Y/N����";
                string choice;
                cin >> choice;
                if (choice == "Y" || choice == "y") {
                    sql = "DELETE FROM reservations WHERE resvNum='" + resvNum + "';";
                    exctDeleteSQL(sql);
                    cout << "��ȡ����Щ����Ԥ������л����ʹ�ã�\n\n";
                }
                else {
                    cout << "������ȡ��\n\n";
                }
            }
        }
        else if (ch2 == 2) {//ɾ�����Ԥ��
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as busNum,location FROM reservations NATURAL JOIN customers,bus WHERE resvType=3 AND custID='" + ID + "' AND resvNum='" + resvNum + "' AND resvKey=busNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ��δ�鵽��Ԥ����Ϣ\n" << std::endl;
            }
            else {
                showQueriedBus(res);
                cout << "�Ƿ�ȡ����ЩԤ������Y/N����";
                string choice;
                cin >> choice;
                if (choice == "Y" || choice == "y") {
                    sql = "DELETE FROM reservations WHERE resvNum='" + resvNum + "';";
                    exctDeleteSQL(sql);
                    cout << "��ȡ����Щ���Ԥ������л����ʹ�ã�\n\n";
                }
                else {
                    cout << "������ȡ��\n\n";
                }
            }
        }
        else if (ch2 == 3) {
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as hotelNum,location FROM reservations NATURAL JOIN customers,hotels WHERE resvType=2 AND custID='" + ID + "' AND resvNum='" + resvNum + "' AND resvKey=hotelNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            if (!mysql_num_rows(res)) {//û�в鵽���
                cout << "�ܱ�Ǹ��δ�鵽��Ԥ����Ϣ\n" << std::endl;
            }
            else {
                showQueriedHotels(res);
                cout << "�Ƿ�ȡ����ЩԤ������Y/N����";
                string choice;
                cin >> choice;
                if (choice == "Y" || choice == "y") {
                    sql = "DELETE FROM reservations WHERE resvNum='" + resvNum + "';";
                    exctDeleteSQL(sql);
                    cout << "��ȡ����Щ�Ƶ�Ԥ������л����ʹ�ã�\n\n";
                }
                else {
                    cout << "������ȡ��\n\n";
                }
            }
        }
        else {
            cout << "���˳�����...\n\n";
            break;
        }
    }
}

void RouteService(string& ID) {//��ѯ����·��
    string sql;
    sql = "select FromCity,ArivCity From reservations natural join customers,flights where resvKey=flightNum and resvType=1 and custID='" + ID + "';";
    MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
    if (!mysql_num_rows(res)) {//û�в鵽���
        cout << "�ܱ�Ǹ����û��Ԥ���κκ���\n" << std::endl;
    }
    else {
        cout << "\n��������·������:" << std::endl;

        ////�����������:flight
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        cout << "<-----��·��ʼ-----";
        while ((row = mysql_fetch_row(res)) != NULL)
        {   
            // ����ǰ���е�ÿһ����Ϣ����
            for (int i = 0; i < num; ++i)
            {
                cout << row[i] << "-----";
            }
        }
        cout << "��·����----->\n\n";
        //�ͷ���Դ - �����
        mysql_free_result(res);
    }
}

void CheckService(string& ID) {//���·�ߺ�����-----how????
    vector<string> passCity;//�洢����;���ĳ�����Ϣ
    //�Ȳ�ѯ������Ϣ
    string sql;
    sql = "select FromCity,ArivCity From reservations natural join customers,flights where resvKey=flightNum and resvType=1 and custID='" + ID + "';";
    MYSQL_RES* res = exctQuerySQL(sql);
    if (!mysql_num_rows(res)) {//û�в鵽���
        cout << "�ܱ�Ǹ������û��Ԥ���κκ���\n" << std::endl;
        return;
    }
    else {
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // ����ǰ���е�ÿһ����Ϣ����
            for (int i = 0; i < num; ++i)
            {
                passCity.push_back(row[i]);
            }
        }
    }

    //��ѯ�����Ϣ
    sql = "SELECT location FROM reservations NATURAL JOIN customers,bus WHERE resvType=3 AND custID='" + ID + "' AND resvKey=busNum;";
    res = exctQuerySQL(sql);
    if (!mysql_num_rows(res)) {//û�в鵽���
        cout << "\n�ܱ�Ǹ������û��Ԥ���κδ�ͣ�����·�߲�����\n" << std::endl;
        return;
    }
    else {
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // ����ǰ���е�ÿһ����Ϣ����
            for (int i = 0; i < num; ++i)
            {
                string temp = row[i];
                if (exist(temp, passCity) == 0) {
                    cout << "\n����·�߲�������������;��" + temp + "����ȴԤ���˴��......\n" << endl;
                    return;
                }
            }
        }
    }

    //��ѯ�Ƶ���Ϣ
    sql = "SELECT location FROM reservations NATURAL JOIN customers,hotels WHERE resvType=2 AND custID='" + ID + "' AND resvKey=hotelNum;";
    res = exctQuerySQL(sql);
    if (!mysql_num_rows(res)) {//û�в鵽���
        cout << "\n�ܱ�Ǹ������û��Ԥ���κξƵ꣬����·�߲�����\n" << std::endl;
        return;
    }
    else {
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // ����ǰ���е�ÿһ����Ϣ����
            for (int i = 0; i < num; ++i)
            {
                string temp = row[i];
                if (exist(temp, passCity) == 0) {
                    cout << "\n����·�߲�������������;��" + temp + "����ȴԤ���˾Ƶ�......\n" << endl;
                    return;
                }
            }
        }
    }
}

int insertReserv(string ID, int Type, string Key) {//����һ��Ԥ����Ϣ
    int Num = getRandomNum();
    string sql = "insert into reservations values('" +to_string(Num)+"','" + ID + "', " + to_string(Type) + ",'" + Key + "');";
    //ע���������ͣ�TypeΪint����
    int res = exctInsertSQL(sql);
    return res;//1ִ�гɹ� 0ִ��ʧ��
}

int getRandomNum() {
    random_device rd;  // ���ڻ�ȡ���������
    mt19937 gen(rd()); // ʹ�����ӳ�ʼ��Mersenne Twister������
    uniform_int_distribution<> dis(100000, 999999); // ����ֲ���Χ
    // ���������λ��
    int number = dis(gen);
    return number;
}

void showAllOrder(int Type) {
    if (Type == 1) {
        string sql = "select * from flights;";
        MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            cout << "\nȫ��������Ϣ����:" << std::endl;

            //�����������:flight
            int num = mysql_num_fields(res);
            //�õ������е�����, �������
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            for (int i = 0; i < num; ++i)
            {
                //printf("%s\t\t", fields[i].name);
                if (i > 1 && i < 4) cout << fields[i].name << "\t";
                else cout << fields[i].name << "\t\t";
            }
            printf("\n");
            //��������������е���
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                // ����ǰ���е�ÿһ����Ϣ����
                for (int i = 0; i < num; ++i)
                {
                    cout << row[i] << "\t\t";
                }
                printf("\n");
            }
            //�ͷ���Դ - �����
            mysql_free_result(res);
    }
    else if (Type == 2) {
            string sql = "select * from bus;";
            MYSQL_RES* res = exctQuerySQL(sql);//���в�ѯ
            cout << "\n���г��д����Ϣ����:" << std::endl;

            //�����������:bus
            int num = mysql_num_fields(res);
            //�õ������е�����, �������
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            for (int i = 0; i < num; ++i)
            {
                //printf("%s\t\t", fields[i].name);
                if (i == 1 || i == 3) cout << fields[i].name << "\t";
                else cout << fields[i].name << "\t\t";
            }
            printf("\n");
            //��������������е���
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                // ����ǰ���е�ÿһ����Ϣ����
                for (int i = 0; i < num; ++i)
                {
                    cout << row[i] << "\t\t";
                }
                printf("\n");
            }
            //�ͷ���Դ - �����
            mysql_free_result(res);
    }
    else {
        string sql = "select * from hotels;";
        MYSQL_RES* res = exctQuerySQL(sql);
        cout << "\n���г��оƵ���Ϣ����:" << std::endl;

        //�����������:bus
        int num = mysql_num_fields(res);
        //�õ������е�����, �������
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        for (int i = 0; i < num; ++i)
        {
            //printf("%s\t\t", fields[i].name);
            if (i == 3) cout << fields[i].name << "\t\t";
            else cout << fields[i].name << "\t\t";
        }
        printf("\n");
        //��������������е���
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // ����ǰ���е�ÿһ����Ϣ����
            for (int i = 0; i < num; ++i)
            {
                cout << row[i] << "\t\t\t";
            }
            printf("\n");
        }
        //�ͷ���Դ - �����
        mysql_free_result(res);
    }
}

void showQueriedFlights(MYSQL_RES* res) {
    cout << "\n�����Ѿ�Ԥ���ĺ�����Ϣ����:" << std::endl;

    //�����������:hotel
    int num = mysql_num_fields(res);
    //�õ������е�����, �������
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        //printf("%s\t\t", fields[i].name);
        if (i > 1 && i < 3) cout << fields[i].name << "\t";
        else cout << fields[i].name << "\t\t";
    }
    printf("\n");
    //��������������е���
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        // ����ǰ���е�ÿһ����Ϣ����
        for (int i = 0; i < num; ++i)
        {
            if (i == 4) cout << row[i] << "\t\t\t";
            else cout << row[i] << "\t\t";
        }
        printf("\n\n");
    }
    //�ͷ���Դ - �����
    mysql_free_result(res);
}

void showQueriedBus(MYSQL_RES* res) {//չʾԤ�������Ϣ
    cout << "\n�����Ѿ�Ԥ���Ĵ����Ϣ����:" << std::endl;

    //�����������:bus
    int num = mysql_num_fields(res);
    //�õ������е�����, �������
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        //printf("%s\t\t", fields[i].name);
        if (i > 1 && i < 3) cout << fields[i].name << "\t";
        else cout << fields[i].name << "\t\t";
    }
    printf("\n");
    //��������������е���
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        // ����ǰ���е�ÿһ����Ϣ����
        for (int i = 0; i < num; ++i)
        {
           cout << row[i] << "\t\t";
        }
        printf("\n\n");
    }
    //�ͷ���Դ - �����
    mysql_free_result(res);
}

void showQueriedHotels(MYSQL_RES* res) {
    cout << "\n�����Ѿ�Ԥ���ľƵ���Ϣ����:" << std::endl;

    //�����������:hotel
    int num = mysql_num_fields(res);
    //�õ������е�����, �������
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        //printf("%s\t\t", fields[i].name);
        if (i > 1 && i < 3) cout << fields[i].name << "\t";
        else cout << fields[i].name << "\t\t";
    }
    printf("\n");
    //��������������е���
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        // ����ǰ���е�ÿһ����Ϣ����
        for (int i = 0; i < num; ++i)
        {
            if (i == 3) cout << row[i] << "\t\t\t";
            else cout << row[i] << "\t\t";
        }
        printf("\n\n");
    }
    //�ͷ���Դ - �����
    mysql_free_result(res);
}

int exist(string city, vector<string> passCity) {
    auto it = passCity.begin();
    while (it != passCity.end())
    {
        if (city == *it) return 1;//����
        it++;
    }
    return 0;//������
}