#pragma once
//author：孙宝臻
//date：2020.7.22
//function：数据操作

#ifndef TRY_DB_H
#define TRY_DB_H

#include <winsock.h> //注意顺序，要放在mysql.h前
#include <mysql.h>//控制台项目中要在mysql.h之前include <winsock.h>
#include <string.h>
#include "User.h"
#pragma warning(disable:4996)
using namespace std;

class dbuser {
public:
	MYSQL* pConn;

	//数据库连接
	dbuser() {
		pConn = mysql_init(NULL);
		//第2、3、4、5参数的意思分别是：服务器地址、用户名、密码、数据库名，第6个为mysql端口号（0为默认值3306）
		if (!mysql_real_connect(pConn, "39.98.48.34", "root", "2296026", "chess", 0, NULL, 0)) {
			printf("无法连接数据库:%s", mysql_error(pConn));
			return;
		}
		mysql_query(pConn, "set names gbk");//防止乱码。设置和数据库的编码一致就不会乱码
	};

	//测试用
	void test() {
		if (mysql_query(pConn, "select * from user")) {
			printf("查询失败:%s", mysql_error(pConn));
			return;
		}
		//mysql_store_result是把查询结果一次性取到客户端的离线数据集，当结果比较大时耗内存。
		//mysql_use_result则是查询结果放在服务器上，客户端通过指针逐行读取，节省客户端内存。但是一个MYSQL*连接同时只能有一个未关闭的mysql_use_result查询
		MYSQL_RES* result = mysql_store_result(pConn);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(result)) {
			printf("%s %s\n", row[0], row[1]);
		}

		mysql_free_result(result);
		mysql_close(pConn);
	};
	//新建用户  所有信息都需要
	void makeAccount(User user) {
		string temp = "INSERT INTO user VALUES ('" + user.id + "','" + user.password 
			+ "','" + user.sex + "'," + to_string(user.age) + ",'" + user.birth
			+ "','" + user.email + "',"  + to_string(user.level) + ",0,0,0)";
		char sql[100];
		strcpy(sql, temp.c_str());
		mysql_query(pConn, sql);
		mysql_close(pConn);
	};

	//获取某个用户信息,根据用户id获取
	User getUser(User user) {
		string temp = "select * from user where id='" + user.id + "'";
		char sql[100];
		strcpy(sql, temp.c_str());
		mysql_query(pConn, sql);
		MYSQL_RES* result = mysql_store_result(pConn);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(result)) {
			user.password = row[1];
			user.sex = row[2];
			user.age = atoi(row[3]);
			user.birth = row[4];
			user.email = row[5];
			user.level = atoi(row[6]);
			user.win = atof(row[7]);
			user.lose = atof(row[8]);
			user.winrate = atof(row[9]);
		}
		mysql_free_result(result);
		mysql_close(pConn);
		return user;
	};

	//登录校验   无此账号返回-1，密码错误返回0，正确返回1
	int login(string id, string password)
	{
		string temp = "select password from user where id='" + id + "'";
		char sql[100];
		strcpy(sql, temp.c_str());
		string temppassword = " ";

		mysql_query(pConn, sql);

		MYSQL_RES* result = mysql_store_result(pConn);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(result)) {
			temppassword = row[0];
		}
		mysql_free_result(result);
		mysql_close(pConn);
		if (temppassword == " ")
			return -1;
		else if (temppassword == password)
			return 1;
		return 0;
	}

	//修改对局  赢了i为1，输了i为-1  需要传入一个健全的user，信息要都有
	void change(User user, int i)
	{
		
		string temp = "";
		if (i == 1)
			temp = "update user set win=" + to_string(user.win + 1) + ",winrate=" + to_string((float)(user.win + 1) / (user.win + 1 + user.lose)) + " where id ='" + user.id + "'";
		else if (i == -1)
			temp = "update user set lose=" + to_string(user.lose + 1) + ",winrate=" + to_string((float)(user.lose + 1) / (user.lose + 1 + user.win)) + " where id ='" + user.id + "'";
		else
			cout << "wrong input!";
		char sql[100];
		strcpy(sql, temp.c_str());

		mysql_query(pConn, sql);
		mysql_close(pConn);
	}

};


#endif //TRY_DB_H



