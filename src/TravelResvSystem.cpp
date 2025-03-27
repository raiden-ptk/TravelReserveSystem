#pragma once
#include <iostream>
#include <string>
#include <random>
#include<vector>
#include "exctSQL.h"

using namespace std;

int mainMenu();//主菜单
int childMenu(const std::string& str);//子菜单
int queryCustomer(string& ID);//查询有无顾客
int insertCustomer(string ID, string name);//插入顾客
int insertReserv(string ID, int Type, string Key);//插入预订
void ReserveService(string& ID);//顾客服务/预订
void QueryService(string& ID);//查询预订
void DeleteService(string& ID);//删除预订
void RouteService(string& ID);//查询路线
void CheckService(string& ID);//检查路线合理性
int getRandomNum();//为订单编号获取随机数
void showAllOrder(int Type);//展示表中所有信息
void showQueriedFlights(MYSQL_RES* res);//以不同格式展示查询结果集
void showQueriedBus(MYSQL_RES* res);
void showQueriedHotels(MYSQL_RES* res);
int exist(string city, vector<string> passCity);//判断city是否存在于路线

int main() {
    std::string ID;

    while (true) {
        cout << "------------------欢迎进入西电旅行预订系统------------------" << std::endl;
        cout << "请输入您的用户ID（0.退出）：";
        cin >> ID;
        if (ID == "0") {
            cout << "已退出服务...\n\n";
            break;
        }
        else {
            // 服务界面
            if (!queryCustomer(ID)) {
                cout << "欢迎新客户使用本系统" << std::endl;
                string name;
                cout << "您的姓名：";
                cin >> name;
                if (insertCustomer(ID, name)) {
                    std::cout << "您的信息已登记qwq!" << std::endl;
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
                    std::cout << "---------------已退出用户服务--------------\n\n";
                    break;
                }
            } while (ch1 >= 1 && ch1 <= 5);
        }
    }
    return 0;
}

// 主菜单
// 选择预定 / 查询 / 取消预定
int mainMenu() {
    int choice;//这里输入可能会出现问题

    while (true) {
        cout << "+--------------TravelReserve--------------+" << endl;
        cout << "|  1.预定航班/大巴车/酒店                 |" << endl;
        cout << "|  2.查询航班/大巴车/酒店预定信息         |" << endl;
        cout << "|  3.取消航班/大巴车/酒店预定             |" << endl;
        cout << "|  4.查询旅行线路                         |" << endl;
        cout << "|  5.检查线路合理性                       |" << endl;
        cout << "|  0.退出                                 |" << endl;
        cout << "+-----------------------------------------+" << endl;
        cout << "请选择服务(0/1/2/3/4/5): ";

        cin >> choice;

        if (choice >= 0 && choice <= 5) {
            return choice;
        }
        else {
            cout << "请重新输入" << endl;
        }
    }
}

int childMenu(const std::string& str) {
    int choice;
    while (true) {
        std::cout << "1." << str << "航班 2." << str << "大巴车 3." << str << "酒店（0.退出）:" << std::endl;
        std::cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            return choice; // 返回有效的选择
        }
        else if (choice == 0) {
            return choice;
        }
        else {
            std::cout << "请重新输入" << std::endl;
        }
    }
}

int queryCustomer(string& ID) {//查询数据库中是否已经有该顾客
    //进行查询
    string sql = "select * from customers where custID = '"+ID+"';";
    int res = exctQuerySQL_B(sql);
    //cout << res << end;
    return res;
}

int insertCustomer(string ID, string name) {//向数据库中插入该顾客
    string sql = "insert into customers values('" + ID + "', '" + name + "');";
    int res = exctInsertSQL(sql);
    return res;//1执行成功 0执行失败
}

void ReserveService(string& ID){//用户预订操作
    while (true) {
        int ch2 = childMenu("预订");
        if (ch2 == 1) {
            showAllOrder(ch2);
            string sql = "SELECT * FROM FLIGHTS";
            string FromCity, ArivCity;
            cout << "请输入出发/抵达城市(FromCity ArivCity):";
            cin >> FromCity >> ArivCity;
            if (!FromCity.empty() && !ArivCity.empty()) {
                sql = "SELECT * FROM FLIGHTS WHERE FromCity = '" + FromCity + "' AND ArivCity = '" + ArivCity + "';";
            }
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，没有您想要的航班\n" << std::endl;
            }
            else {
                cout << "所有航班信息如下:" << std::endl;
                //遍历结果集合:flight
                int num = mysql_num_fields(res);
                //得到所有列的名字, 并且输出
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                for (int i = 0; i < num; ++i)
                {
                    if (i > 1 && i < 4) cout << fields[i].name << "\t";
                    else cout << fields[i].name << "\t\t";
                }
                printf("\n");
                //遍历结果集中所有的行
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL)
                {
                    // 将当前行中的每一列信息读出
                    for (int i = 0; i < num; ++i)
                    {
                        cout << row[i] << "\t\t";
                    }
                    cout << endl;
                }
                //释放资源 - 结果集
                mysql_free_result(res);
                while (true) {
                    cout << "请选择航班编号(输入exit取消):\n";
                    string flightNum;
                    cin >> flightNum;
                    if (flightNum == "exit") {
                        break;
                    }
                    else {
                        string choice;
                        cout << "是否选择该航班:" + flightNum + "(Y/N)?\n";
                        cin >> choice;
                        if (choice == "Y" || choice == "y") {
                            //插入一条订单数据
                            insertReserv(ID, 1, flightNum);
                            cout << "预订成功！\n\n";
                            break;
                        }
                        else {
                            cout << "操作已取消！请重新选择\n";
                        }
                    }
                }
            }
        }
        else if (ch2 == 2) {
            showAllOrder(ch2);
            string sql = "SELECT * FROM BUS";
            string Location;
            cout << "请输入城市(Location):";
            cin >> Location;
            if (!Location.empty()) {
                sql = "SELECT * FROM BUS WHERE LOCATION = '" + Location + "';";
            }
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，没有您想要的大巴\n" << std::endl;
            }
            else {
                cout << "所有大巴信息如下:" << std::endl;
                //遍历结果集合:bus
                int num = mysql_num_fields(res);
                //得到所有列的名字, 并且输出
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                for (int i = 0; i < num; ++i)
                {
                    //printf("%s\t\t", fields[i].name);
                    if (i == 1 || i == 3) cout << fields[i].name << "\t";
                    else cout << fields[i].name << "\t\t";
                }
                printf("\n");
                //遍历结果集中所有的行
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL)
                {
                    // 将当前行中的每一列信息读出
                    for (int i = 0; i < num; ++i)
                    {
                        cout << row[i] << "\t\t";
                    }
                    printf("\n");
                }
                //释放资源 - 结果集
                mysql_free_result(res);

                while (true) {
                    cout << "请选择大巴编号(输入exit取消):\n";
                    string busNum;
                    cin >> busNum;
                    if (busNum == "exit") {
                        break;
                    }
                    else {
                        string choice;
                        cout << "是否选择该大巴: " + busNum + " (Y/N)? \n";
                        cin >> choice;
                        if (choice == "Y" || choice == "y") {
                            //插入一条订单数据
                            insertReserv(ID, 3, busNum);//注意编号
                            cout << "预订成功！\n\n";
                            break;
                        }
                        else {
                            cout << "操作已取消！请重新选择\n";
                        }
                    }
                }
            }
        }
        else if (ch2==3) {
            showAllOrder(ch2);
            string sql = "SELECT * FROM HOTELS";
            string Location;
            cout << "请输入城市(Location):";
            cin >> Location;
            if (!Location.empty()) {
                sql = "SELECT * FROM HOTELS WHERE LOCATION = '" + Location + "';";
            }
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，没有您想要的酒店\n" << std::endl;
            }
            else {
                cout << "所有酒店信息如下:" << std::endl;
                //遍历结果集合:bus
                int num = mysql_num_fields(res);
                //得到所有列的名字, 并且输出
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                for (int i = 0; i < num; ++i)
                {
                    //printf("%s\t\t", fields[i].name);
                    if (i == 3) cout << fields[i].name << "\t\t";
                    else cout << fields[i].name << "\t\t";
                }
                printf("\n");
                //遍历结果集中所有的行
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL)
                {
                    // 将当前行中的每一列信息读出
                    for (int i = 0; i < num; ++i)
                    {
                        cout << row[i] << "\t\t\t";
                    }
                    printf("\n");
                }
                //释放资源 - 结果集
                mysql_free_result(res);
                while (true) {
                    cout << "请选择酒店编号(输入exit取消):\n";
                    string hotelNum;
                    cin >> hotelNum;
                    if (hotelNum == "exit") {
                        break;
                    }
                    else {
                        string choice;
                        cout << "是否选择该酒店: " + hotelNum + " (Y/N)? \n";
                        cin >> choice;
                        if (choice == "Y" || choice == "y") {
                            //插入一条订单数据
                            insertReserv(ID, 2, hotelNum);//注意编号
                            cout << "预订成功！\n\n";
                            break;
                        }
                        else {
                            cout << "操作已取消！请重新选择\n";
                        }
                    }
                }
            }
        }
        else {
        cout << "已退出服务...\n\n";
        break;
        }
    }
}

void QueryService(string& ID) {//查询已有的预定信息
    while (true) {
        int ch2 = childMenu("查询预订");
        if (ch2 == 1) {//查询航班预订
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as flightNum,FromCity,ArivCity FROM reservations NATURAL JOIN customers,flights WHERE resvType=1 AND custID='" + ID + "' AND resvKey=flightNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，您还没有预订任何航班\n" << std::endl;
            }
            else {
                showQueriedFlights(res);
            }
        }
        else if (ch2 == 2) {//查询大巴信息
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as busNum,location FROM reservations NATURAL JOIN customers,bus WHERE resvType=3 AND custID='" + ID + "' AND resvKey=busNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，您没有预订任何大巴\n" << std::endl;
            }
            else {
                showQueriedBus(res);
            }
        }
        else if (ch2 == 3) {
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as hotelNum,location FROM reservations NATURAL JOIN customers,hotels WHERE resvType=2 AND custID='" + ID + "' AND resvKey=hotelNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，您没有预订任何酒店\n" << std::endl;
            }
            else {
                showQueriedHotels(res);
            }
        }
        else {
            cout << "已退出服务...\n\n";
            break;
        }
    }
}

void DeleteService(string& ID) {//删除已有的预订信息
    while (true) {
        int ch2 = childMenu("取消预订");
        string resvNum;
        if (ch2 != 0) {
            cout << "\n请输入您要取消的预订编号(六位预订编号 0.退出)：";
            cin >> resvNum;
            if (resvNum == "0") {
                cout << "已退出服务...\n\n";
                break;
            }
        }
        if (ch2 == 1) {//删除航班预订
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as flightNum,FromCity,ArivCity FROM reservations NATURAL JOIN customers,flights WHERE resvType=1 AND custID='" + ID + "' AND resvNum='" + resvNum + "' AND resvKey=flightNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，未查到该预定信息\n" << std::endl;
            }
            else {
                showQueriedFlights(res);
                cout << "是否取消这些预订？（Y/N）：";
                string choice;
                cin >> choice;
                if (choice == "Y" || choice == "y") {
                    sql = "DELETE FROM reservations WHERE resvNum='" + resvNum + "';";
                    exctDeleteSQL(sql);
                    cout << "已取消这些航班预订，感谢您的使用！\n\n";
                }
                else {
                    cout << "操作已取消\n\n";
                }
            }
        }
        else if (ch2 == 2) {//删除大巴预订
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as busNum,location FROM reservations NATURAL JOIN customers,bus WHERE resvType=3 AND custID='" + ID + "' AND resvNum='" + resvNum + "' AND resvKey=busNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，未查到该预订信息\n" << std::endl;
            }
            else {
                showQueriedBus(res);
                cout << "是否取消这些预订？（Y/N）：";
                string choice;
                cin >> choice;
                if (choice == "Y" || choice == "y") {
                    sql = "DELETE FROM reservations WHERE resvNum='" + resvNum + "';";
                    exctDeleteSQL(sql);
                    cout << "已取消这些大巴预订，感谢您的使用！\n\n";
                }
                else {
                    cout << "操作已取消\n\n";
                }
            }
        }
        else if (ch2 == 3) {
            string sql;
            sql = "SELECT resvNum,custID,custName,resvKey as hotelNum,location FROM reservations NATURAL JOIN customers,hotels WHERE resvType=2 AND custID='" + ID + "' AND resvNum='" + resvNum + "' AND resvKey=hotelNum;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            if (!mysql_num_rows(res)) {//没有查到结果
                cout << "很抱歉，未查到该预定信息\n" << std::endl;
            }
            else {
                showQueriedHotels(res);
                cout << "是否取消这些预订？（Y/N）：";
                string choice;
                cin >> choice;
                if (choice == "Y" || choice == "y") {
                    sql = "DELETE FROM reservations WHERE resvNum='" + resvNum + "';";
                    exctDeleteSQL(sql);
                    cout << "已取消这些酒店预订，感谢您的使用！\n\n";
                }
                else {
                    cout << "操作已取消\n\n";
                }
            }
        }
        else {
            cout << "已退出服务...\n\n";
            break;
        }
    }
}

void RouteService(string& ID) {//查询旅行路线
    string sql;
    sql = "select FromCity,ArivCity From reservations natural join customers,flights where resvKey=flightNum and resvType=1 and custID='" + ID + "';";
    MYSQL_RES* res = exctQuerySQL(sql);//进行查询
    if (!mysql_num_rows(res)) {//没有查到结果
        cout << "很抱歉，您没有预订任何航班\n" << std::endl;
    }
    else {
        cout << "\n您的旅游路线如下:" << std::endl;

        ////遍历结果集合:flight
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        cout << "<-----线路开始-----";
        while ((row = mysql_fetch_row(res)) != NULL)
        {   
            // 将当前行中的每一列信息读出
            for (int i = 0; i < num; ++i)
            {
                cout << row[i] << "-----";
            }
        }
        cout << "线路结束----->\n\n";
        //释放资源 - 结果集
        mysql_free_result(res);
    }
}

void CheckService(string& ID) {//检查路线合理性-----how????
    vector<string> passCity;//存储航班途径的城市信息
    //先查询航班信息
    string sql;
    sql = "select FromCity,ArivCity From reservations natural join customers,flights where resvKey=flightNum and resvType=1 and custID='" + ID + "';";
    MYSQL_RES* res = exctQuerySQL(sql);
    if (!mysql_num_rows(res)) {//没有查到结果
        cout << "很抱歉，您还没有预订任何航班\n" << std::endl;
        return;
    }
    else {
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // 将当前行中的每一列信息读出
            for (int i = 0; i < num; ++i)
            {
                passCity.push_back(row[i]);
            }
        }
    }

    //查询大巴信息
    sql = "SELECT location FROM reservations NATURAL JOIN customers,bus WHERE resvType=3 AND custID='" + ID + "' AND resvKey=busNum;";
    res = exctQuerySQL(sql);
    if (!mysql_num_rows(res)) {//没有查到结果
        cout << "\n很抱歉，您还没有预订任何大巴，旅游路线不合理！\n" << std::endl;
        return;
    }
    else {
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // 将当前行中的每一列信息读出
            for (int i = 0; i < num; ++i)
            {
                string temp = row[i];
                if (exist(temp, passCity) == 0) {
                    cout << "\n旅游路线不合理，您并不会途径" + temp + "，但却预订了大巴......\n" << endl;
                    return;
                }
            }
        }
    }

    //查询酒店信息
    sql = "SELECT location FROM reservations NATURAL JOIN customers,hotels WHERE resvType=2 AND custID='" + ID + "' AND resvKey=hotelNum;";
    res = exctQuerySQL(sql);
    if (!mysql_num_rows(res)) {//没有查到结果
        cout << "\n很抱歉，您还没有预订任何酒店，旅游路线不合理！\n" << std::endl;
        return;
    }
    else {
        int num = mysql_num_fields(res);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // 将当前行中的每一列信息读出
            for (int i = 0; i < num; ++i)
            {
                string temp = row[i];
                if (exist(temp, passCity) == 0) {
                    cout << "\n旅游路线不合理，您并不会途径" + temp + "，但却预订了酒店......\n" << endl;
                    return;
                }
            }
        }
    }
}

int insertReserv(string ID, int Type, string Key) {//插入一条预订信息
    int Num = getRandomNum();
    string sql = "insert into reservations values('" +to_string(Num)+"','" + ID + "', " + to_string(Type) + ",'" + Key + "');";
    //注意属性类型，Type为int类型
    int res = exctInsertSQL(sql);
    return res;//1执行成功 0执行失败
}

int getRandomNum() {
    random_device rd;  // 用于获取随机数种子
    mt19937 gen(rd()); // 使用种子初始化Mersenne Twister生成器
    uniform_int_distribution<> dis(100000, 999999); // 定义分布范围
    // 生成随机六位数
    int number = dis(gen);
    return number;
}

void showAllOrder(int Type) {
    if (Type == 1) {
        string sql = "select * from flights;";
        MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            cout << "\n全国航班信息如下:" << std::endl;

            //遍历结果集合:flight
            int num = mysql_num_fields(res);
            //得到所有列的名字, 并且输出
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            for (int i = 0; i < num; ++i)
            {
                //printf("%s\t\t", fields[i].name);
                if (i > 1 && i < 4) cout << fields[i].name << "\t";
                else cout << fields[i].name << "\t\t";
            }
            printf("\n");
            //遍历结果集中所有的行
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                // 将当前行中的每一列信息读出
                for (int i = 0; i < num; ++i)
                {
                    cout << row[i] << "\t\t";
                }
                printf("\n");
            }
            //释放资源 - 结果集
            mysql_free_result(res);
    }
    else if (Type == 2) {
            string sql = "select * from bus;";
            MYSQL_RES* res = exctQuerySQL(sql);//进行查询
            cout << "\n所有城市大巴信息如下:" << std::endl;

            //遍历结果集合:bus
            int num = mysql_num_fields(res);
            //得到所有列的名字, 并且输出
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            for (int i = 0; i < num; ++i)
            {
                //printf("%s\t\t", fields[i].name);
                if (i == 1 || i == 3) cout << fields[i].name << "\t";
                else cout << fields[i].name << "\t\t";
            }
            printf("\n");
            //遍历结果集中所有的行
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                // 将当前行中的每一列信息读出
                for (int i = 0; i < num; ++i)
                {
                    cout << row[i] << "\t\t";
                }
                printf("\n");
            }
            //释放资源 - 结果集
            mysql_free_result(res);
    }
    else {
        string sql = "select * from hotels;";
        MYSQL_RES* res = exctQuerySQL(sql);
        cout << "\n所有城市酒店信息如下:" << std::endl;

        //遍历结果集合:bus
        int num = mysql_num_fields(res);
        //得到所有列的名字, 并且输出
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        for (int i = 0; i < num; ++i)
        {
            //printf("%s\t\t", fields[i].name);
            if (i == 3) cout << fields[i].name << "\t\t";
            else cout << fields[i].name << "\t\t";
        }
        printf("\n");
        //遍历结果集中所有的行
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            // 将当前行中的每一列信息读出
            for (int i = 0; i < num; ++i)
            {
                cout << row[i] << "\t\t\t";
            }
            printf("\n");
        }
        //释放资源 - 结果集
        mysql_free_result(res);
    }
}

void showQueriedFlights(MYSQL_RES* res) {
    cout << "\n所有已经预定的航班信息如下:" << std::endl;

    //遍历结果集合:hotel
    int num = mysql_num_fields(res);
    //得到所有列的名字, 并且输出
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        //printf("%s\t\t", fields[i].name);
        if (i > 1 && i < 3) cout << fields[i].name << "\t";
        else cout << fields[i].name << "\t\t";
    }
    printf("\n");
    //遍历结果集中所有的行
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        // 将当前行中的每一列信息读出
        for (int i = 0; i < num; ++i)
        {
            if (i == 4) cout << row[i] << "\t\t\t";
            else cout << row[i] << "\t\t";
        }
        printf("\n\n");
    }
    //释放资源 - 结果集
    mysql_free_result(res);
}

void showQueriedBus(MYSQL_RES* res) {//展示预订大巴信息
    cout << "\n所有已经预定的大巴信息如下:" << std::endl;

    //遍历结果集合:bus
    int num = mysql_num_fields(res);
    //得到所有列的名字, 并且输出
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        //printf("%s\t\t", fields[i].name);
        if (i > 1 && i < 3) cout << fields[i].name << "\t";
        else cout << fields[i].name << "\t\t";
    }
    printf("\n");
    //遍历结果集中所有的行
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        // 将当前行中的每一列信息读出
        for (int i = 0; i < num; ++i)
        {
           cout << row[i] << "\t\t";
        }
        printf("\n\n");
    }
    //释放资源 - 结果集
    mysql_free_result(res);
}

void showQueriedHotels(MYSQL_RES* res) {
    cout << "\n所有已经预定的酒店信息如下:" << std::endl;

    //遍历结果集合:hotel
    int num = mysql_num_fields(res);
    //得到所有列的名字, 并且输出
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        //printf("%s\t\t", fields[i].name);
        if (i > 1 && i < 3) cout << fields[i].name << "\t";
        else cout << fields[i].name << "\t\t";
    }
    printf("\n");
    //遍历结果集中所有的行
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        // 将当前行中的每一列信息读出
        for (int i = 0; i < num; ++i)
        {
            if (i == 3) cout << row[i] << "\t\t\t";
            else cout << row[i] << "\t\t";
        }
        printf("\n\n");
    }
    //释放资源 - 结果集
    mysql_free_result(res);
}

int exist(string city, vector<string> passCity) {
    auto it = passCity.begin();
    while (it != passCity.end())
    {
        if (city == *it) return 1;//存在
        it++;
    }
    return 0;//不存在
}