#pragma once
//author���ﱦ��
//date��2020.7.22
//function��ʵ����user

#ifndef TRY_USER_H
#define TRY_USER_H
#include <string>
using namespace std;

class User
{
public:
	string id;   // �ǳ�
	string password;  // ����
	string email;   //����
	int win;   //Ӯ�˼���
	int lose;   //���˼���
	float winrate;   ///ʤ��
	User()
	{

    }
};


#endif