# TravelReserveSystem
2024年秋季数据库4、5次上机内容

## 实验内容
见：上机45要求.doc

## 该课设说明
该课设的界面和思路参考了ZYCat学长的TourBookingSystem项目：https://github.com/izcat/TourBookingSystem 是基于其的c++实现版本，使用mysql库进行数据库链接操作。

## c++中数据库操作示例
如何在c++中操作mysql参考：https://blog.csdn.net/weixin_43450564/article/details/115537977 具体示例如下：
```
#include <iostream>
#include <mysql.h>
using namespace std;

int main()
{
            //1. 初始化连接环境
                MYSQL * mysql = mysql_init(NULL);
            if (mysql == NULL)\
            {
                printf("mysql_init() error\n");
                return -1;
                    }
        
            // 2. 连接数据库服务器
            mysql = mysql_real_connect(mysql, "localhost", "root", "1234",//主机，用户名，密码，数据库名
                "bank946", 0, NULL, 0);
            if (mysql == NULL)
            {
                printf("mysql_real_connect() error\n");
                return -1;
            }
        
            //printf("mysql api使用的默认编码: %s\n", mysql_character_set_name(mysql));
        
            // 设置编码为utf8
            mysql_set_character_set(mysql, "utf8");
        
            //printf("mysql api使用的修改之后的编码: %s\n", mysql_character_set_name(mysql));
        
            printf("恭喜, 连接数据库服务器成功了...\n");

            // 3. 执行一个sql语句
            // 查询scott数据库下的dept部门表
            const char* sql = "select * from account946";
            // 执行这个sql语句
            int ret = mysql_query(mysql, sql);
            if (ret != 0)
            {
                printf("mysql_query() a失败了, 原因: %s\n", mysql_error(mysql));
                return -1;
            }
        
            // 4. 取出结果集
            MYSQL_RES* res = mysql_store_result(mysql);
            if (res == NULL)
            {
                printf("mysql_store_result() 失败了, 原因: %s\n", mysql_error(mysql));
                return -1;
            }
        
            // 5. 得到结果集中的列数
            int num = mysql_num_fields(res);
        
            // 6. 得到所有列的名字, 并且输出
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            for (int i = 0; i < num; ++i)
            {
                printf("%s\t\t", fields[i].name);
            }
            printf("\n");
        
            // 7. 遍历结果集中所有的行
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                // 将当前行中的每一列信息读出
                for (int i = 0; i < num; ++i)
                {
                    printf("%s\t\t", row[i]);
                }
                printf("\n");
            }
        
            // 8. 释放资源 - 结果集
            mysql_free_result(res);
        
            // 9. 写数据库
            // 以下三条是一个完整的操作, 对应的是一个事务
            // 设置事务为手动提交
            mysql_autocommit(mysql, 0);
            int ret1 = mysql_query(mysql, "insert into dept values(61, '海军', '圣地玛丽乔亚')");
            int ret2 = mysql_query(mysql, "insert into dept values(62, '七武海', '世界各地')");
            int ret3 = mysql_query(mysql, "insert into dept values(63, '四皇', '新世界')");
            printf("ret1 = %d, ret2 = %d, ret3 = %d\n", ret1, ret2, ret3);
        
            if (ret1 == 0 && ret2 == 0 && ret3 == 0)
            {
                // 提交事务
                mysql_commit(mysql);
            }
            else
            {
                mysql_rollback(mysql);
            }
            // 释放数据库资源
            mysql_close(mysql);
        
            return 0;
}        
```