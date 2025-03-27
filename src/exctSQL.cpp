#pragma once
#include "exctSQL.h"
//���ݿ���ɾ�Ĳ�

int exctInsertSQL(string sql) {//�����ݿ���в������ �ֶ��ύģʽ
    //�������ݿ�
    MYSQL* mysql = mysql_init(NULL);
    if (mysql == NULL) {
        cout << "mysql_init() error" << endl;
        return 0;
    }
    mysql = mysql_real_connect(mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
    if (mysql == NULL) {
        cout << "mysql connect error,pls retry\n";
        return 0;
    }
    //�������
    mysql_autocommit(mysql, 0);
    //ִ��sql���
    int ret = mysql_query(mysql, sql.c_str());//ע������ת��string->const char *
    if (ret != 0) {
        cout << "mysql_queryʧ���ˣ�ԭ���ǣ�" << mysql_error(mysql) << endl;//ִ��
        mysql_rollback(mysql);//ִ��ʧ�ܣ��ع�
        return 0;//ʧ��
    }
    else {
        mysql_commit(mysql);//�����ύ
    }
    // �ͷ����ݿ���Դ
    mysql_close(mysql);

	return 1;//�ɹ�
}

int exctDeleteSQL(string sql) {//�����ݿ����ɾ������ �ֶ��ύģʽ
    //�������ݿ�
    MYSQL* mysql = mysql_init(NULL);
    if (mysql == NULL) {
        cout << "mysql_init() error" << endl;
        return 0;
    }
    mysql = mysql_real_connect(mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
    if (mysql == NULL) {
        cout << "mysql connect error,pls retry\n";
        return 0;
    }
    //�������
    mysql_autocommit(mysql, 0);
    int ret = mysql_query(mysql, sql.c_str());
    if (ret!=0) {
        cout << "mysql_queryʧ���ˣ�ԭ���ǣ�" << mysql_error(mysql) << endl;//ִ��
        mysql_rollback(mysql);
        return 0;
    }
    else {
        mysql_commit(mysql);
    }
    int res = mysql_affected_rows(mysql);
    mysql_close(mysql);
	return res;//�ɹ�,����Ӱ������
}

int exctQuerySQL_B(string sql) {//�����ݿ���в�ѯ���� ��0�鵽 0δ�鵽
    //�������ݿ�
    MYSQL* mysql = mysql_init(NULL);
    if (mysql == NULL) {
        cout << "mysql_init() error" << endl;
        return 0;
    }
    mysql = mysql_real_connect(mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
    if (mysql == NULL) {
        cout << "mysql connect error,pls retry\n";
        return 0;
    }
    //�������
    //cout << sql.c_str();
    int ret = mysql_query(mysql, sql.c_str());
    if (ret != 0) {
        cout << "mysql_queryʧ���ˣ�ԭ���ǣ�" << mysql_error(mysql) << endl;//ִ��
        return 0;
    }

    //��ȡ��ѯ�����
    MYSQL_RES* res = mysql_store_result(mysql);
    if (res == NULL) {
        cout << "mysql_store_result() ʧ��: " << mysql_error(mysql) << std::endl;
        mysql_close(mysql);
        return 0;
    }

    //��ȡ����
    int rows = mysql_num_rows(res);

    // �ͷ����ݿ���Դ
    mysql_free_result(res);
    mysql_close(mysql);
	return rows;
}

MYSQL_RES* exctQuerySQL(string sql) {//�����ݿ���в�ѯ ���ز�ѯ�����
    //�������ݿ�
    MYSQL* mysql = mysql_init(NULL);
    if (mysql == NULL) {
        cout << "mysql_init() error" << endl;
        return 0;
    }
    mysql = mysql_real_connect(mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
    if (mysql == NULL) {
        cout << "mysql connect error,pls retry\n";
        return 0;
    }
    //�������

    //��ѯ
    //cout << sql.c_str();
    int ret = mysql_query(mysql, sql.c_str());
    if (ret != 0) {
        cout << "mysql_queryʧ���ˣ�ԭ���ǣ�" << mysql_error(mysql) << endl;//ִ��
        return 0;
    }

    //��ȡ��ѯ�����
    MYSQL_RES* res = mysql_store_result(mysql);
    if (res == NULL) {
        cout << "mysql_store_result() ʧ��: " << mysql_error(mysql) << std::endl;
        mysql_close(mysql);
        return 0;
    }

    return res;

}

//ִ������sql
int exctSQL(string sql) {
	return false;
}