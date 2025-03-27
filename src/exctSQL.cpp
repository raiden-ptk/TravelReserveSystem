#pragma once
#include "exctSQL.h"
//数据库增删改查

int exctInsertSQL(string sql) {//对数据库进行插入操作 手动提交模式
    //连接数据库
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
    //连接完成
    mysql_autocommit(mysql, 0);
    //执行sql语句
    int ret = mysql_query(mysql, sql.c_str());//注意类型转换string->const char *
    if (ret != 0) {
        cout << "mysql_query失败了，原因是：" << mysql_error(mysql) << endl;//执行
        mysql_rollback(mysql);//执行失败，回滚
        return 0;//失败
    }
    else {
        mysql_commit(mysql);//正常提交
    }
    // 释放数据库资源
    mysql_close(mysql);

	return 1;//成功
}

int exctDeleteSQL(string sql) {//对数据库进行删除操作 手动提交模式
    //连接数据库
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
    //连接完成
    mysql_autocommit(mysql, 0);
    int ret = mysql_query(mysql, sql.c_str());
    if (ret!=0) {
        cout << "mysql_query失败了，原因是：" << mysql_error(mysql) << endl;//执行
        mysql_rollback(mysql);
        return 0;
    }
    else {
        mysql_commit(mysql);
    }
    int res = mysql_affected_rows(mysql);
    mysql_close(mysql);
	return res;//成功,返回影响行数
}

int exctQuerySQL_B(string sql) {//对数据库进行查询操作 非0查到 0未查到
    //连接数据库
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
    //连接完成
    //cout << sql.c_str();
    int ret = mysql_query(mysql, sql.c_str());
    if (ret != 0) {
        cout << "mysql_query失败了，原因是：" << mysql_error(mysql) << endl;//执行
        return 0;
    }

    //获取查询结果集
    MYSQL_RES* res = mysql_store_result(mysql);
    if (res == NULL) {
        cout << "mysql_store_result() 失败: " << mysql_error(mysql) << std::endl;
        mysql_close(mysql);
        return 0;
    }

    //获取行数
    int rows = mysql_num_rows(res);

    // 释放数据库资源
    mysql_free_result(res);
    mysql_close(mysql);
	return rows;
}

MYSQL_RES* exctQuerySQL(string sql) {//对数据库进行查询 返回查询结果集
    //连接数据库
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
    //连接完成

    //查询
    //cout << sql.c_str();
    int ret = mysql_query(mysql, sql.c_str());
    if (ret != 0) {
        cout << "mysql_query失败了，原因是：" << mysql_error(mysql) << endl;//执行
        return 0;
    }

    //获取查询结果集
    MYSQL_RES* res = mysql_store_result(mysql);
    if (res == NULL) {
        cout << "mysql_store_result() 失败: " << mysql_error(mysql) << std::endl;
        mysql_close(mysql);
        return 0;
    }

    return res;

}

//执行任意sql
int exctSQL(string sql) {
	return false;
}