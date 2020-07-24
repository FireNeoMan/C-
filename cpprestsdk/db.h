#pragma once
//author���ﱦ��
//date��2020.7.22
//function�����ݲ���

#ifndef TRY_DB_H
#define TRY_DB_H

#include <winsock.h> //ע��˳��Ҫ����mysql.hǰ
#include <mysql.h>//����̨��Ŀ��Ҫ��mysql.h֮ǰinclude <winsock.h>
#include <string.h>
#include "User.h"
#pragma warning(disable:4996)
using namespace std;

class dbuser {
public:
	MYSQL* pConn;

	//���ݿ�����
	dbuser() {
		pConn = mysql_init(NULL);
		//��2��3��4��5��������˼�ֱ��ǣ���������ַ���û��������롢���ݿ�������6��Ϊmysql�˿ںţ�0ΪĬ��ֵ3306��
		if (!mysql_real_connect(pConn, "39.98.48.34", "root", "2296026", "chess", 0, NULL, 0)) {
			printf("�޷��������ݿ�:%s", mysql_error(pConn));
			return;
		}
		mysql_query(pConn, "set names gbk");//��ֹ���롣���ú����ݿ�ı���һ�¾Ͳ�������
	};

	//������
	void test() {
		if (mysql_query(pConn, "select * from user")) {
			printf("��ѯʧ��:%s", mysql_error(pConn));
			return;
		}
		//mysql_store_result�ǰѲ�ѯ���һ����ȡ���ͻ��˵��������ݼ���������Ƚϴ�ʱ���ڴ档
		//mysql_use_result���ǲ�ѯ������ڷ������ϣ��ͻ���ͨ��ָ�����ж�ȡ����ʡ�ͻ����ڴ档����һ��MYSQL*����ͬʱֻ����һ��δ�رյ�mysql_use_result��ѯ
		MYSQL_RES* result = mysql_store_result(pConn);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(result)) {
			printf("%s %s\n", row[0], row[1]);
		}

		mysql_free_result(result);
		mysql_close(pConn);
	};
	//�½��û�  ������Ϣ����Ҫ
	void makeAccount(User user) {
		string temp = "INSERT INTO user VALUES ('" + user.id + "','" + user.password 
			+ "','" + user.sex + "'," + to_string(user.age) + ",'" + user.birth
			+ "','" + user.email + "',"  + to_string(user.level) + ",0,0,0)";
		char sql[100];
		strcpy(sql, temp.c_str());
		mysql_query(pConn, sql);
		mysql_close(pConn);
	};

	//��ȡĳ���û���Ϣ,�����û�id��ȡ
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

	//��¼У��   �޴��˺ŷ���-1��������󷵻�0����ȷ����1
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

	//�޸ĶԾ�  Ӯ��iΪ1������iΪ-1  ��Ҫ����һ����ȫ��user����ϢҪ����
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



