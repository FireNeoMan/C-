#pragma once
//author：孙宝臻
//date：2020.7.22
//function：实体类user

#ifndef TRY_USER_H
#define TRY_USER_H
#include <string>
using namespace std;

class User
{
public:
	string id;   // 昵称
	string password;  // 密码
	string sex;   //性别
	int age;   //年龄
	string birth;   //生日
	string email;   //邮箱
	int level;   //级别
	int win;   //赢了几局
	int lose;   //输了几局
	float winrate;   ///胜率
	User()
	{

    }
};


#endif