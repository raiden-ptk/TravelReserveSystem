#pragma once
#define HOST "localhost"
#define USER "root"
#define PASSWORD "1234"
#define DATABASE "travelreserve"
#include<iostream>
#include<string>
#include<mysql.h>
using namespace std;

//���ݿ���ɾ�Ĳ�
int exctInsertSQL(string sql);
int exctDeleteSQL(string sql);
int exctQuerySQL_B(string sql);
MYSQL_RES* exctQuerySQL(string sql);
//ִ������sql
int exctSQL(string sql);