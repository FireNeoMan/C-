#include <stdio.h>
//author：孙宝臻
//date：2020.7.22
//function：发布rest服务
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include <windows.h>
//数据库操作相关
#include <iostream>
#include "User.h"
#include <winsock.h>
#include <mysql.h>
#include "db.h"
//向量，rest层面数据处理相关
#include<vector>;
#include <string.h>
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "httpapi.lib")
//必要准备
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

using namespace pplx;
using namespace std;
//转码工具
//string 转 wstring(string_t)
wstring StringToWString(const string& s)
{
	std::wstring wszStr;

	int nLength = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, NULL);
	wszStr.resize(nLength);
	LPWSTR lpwszStr = new wchar_t[nLength];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength);
	wszStr = lpwszStr;
	delete[] lpwszStr;

	return wszStr;
}
//wstring(string_t) 转 string
string WStringToString(wstring& wstr)
{
	int nLen = (int)wstr.length();
	string str = " ";
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen,

		(LPSTR)str.c_str(), nLen, NULL, NULL);

	return str;
}
//s为待分割串，按照splitchar中的各种字符分割，结果保存在vec中
void StringSplit(string s, char splitchar, vector<string>& vec)
{
	if (vec.size() > 0)//保证vec是空的
		vec.clear();
	int length = s.length();
	int start = 0;
	for (int i = 0;i < length;i++)
	{
		if (s[i] == splitchar && i == 0)//第一个就遇到分割符
		{
			start += 1;
		}
		else if (s[i] == splitchar)
		{
			vec.push_back(s.substr(start, i - start));
			start = i + 1;
		}
		else if (i == length - 1)//到达尾部
		{
			vec.push_back(s.substr(start, i + 1 - start));
		}
	}
}
//获取指定字符后面的字符串
string FindPreStrOf(string str, const char& ch)
{
	int nPos = (int)str.find(ch);
	if (str.length()> nPos)
	{
		return str.substr(nPos+1);
	}
	return "";
}



class CommandHandler
{
public:
	CommandHandler() {}
	CommandHandler(utility::string_t url);
	//提供服务器开启关闭方法
	task<void> open() { return m_listener.open(); }
	task<void> close() { return m_listener.close(); }
private:
	void handle_get_or_post(http_request message);
	//内部变量
	http_listener m_listener;
};
//构造函数
//string_t（wstring 或字符串）
CommandHandler::CommandHandler(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, bind(&CommandHandler::handle_get_or_post, this, placeholders::_1));
	m_listener.support(methods::POST, bind(&CommandHandler::handle_get_or_post, this, placeholders::_1));
	m_listener.support(methods::OPTIONS, std::bind(&CommandHandler::handle_get_or_post, this, std::placeholders::_1));

}
//处理请求
void CommandHandler::handle_get_or_post(http_request message)
{
	//处理跨域请求
	http_response rep;
	rep.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	rep.headers().add(U("Access-Control-Request-Method"), U("GET,POST,OPTIONS"));
	rep.headers().add(U("Access-Control-Allow-Credentials"), U("true"));
	rep.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type,Access-Token,x-requested-with,Authorization"));
	rep.set_status_code(status_codes::OK);
	//自己瞎写的
	//请求调用的方法
	ucout << "Method: " << message.method() << endl;
	//请求调用的url
	ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << endl;
	//请求调用的查询
	ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << endl << endl;
	string_t urltemp = http::uri::decode(message.relative_uri().path());
	string url = WStringToString(urltemp);
	string_t querytemp = http::uri::decode(message.relative_uri().query());
	string query = WStringToString(querytemp);
	//存储分割后的字符串
	vector<string> a(6);
	//单刀多置开关
	if (url == "/test")
	{
		cout << query;
		rep.set_body("0");
		message.reply(rep);
	}
	//新建用户  所有信息都需要
	else if (url == "/makeAccount")
	{
		User user;
		//处理url
		StringSplit(query,'&',a);
		//赋值
		user.id = FindPreStrOf(a[0],'=');
		user.password = FindPreStrOf(a[1], '=');
		user.sex = FindPreStrOf(a[2], '=');
		user.age = stoi(FindPreStrOf(a[3], '='));
		user.birth = FindPreStrOf(a[4], '=');
		user.email = FindPreStrOf(a[5], '=');
		user.level = 0;//string转int

		dbuser db1;
		db1.makeAccount(user);
		rep.set_body("0");
		message.reply(rep);
	}
	//获取某个用户信息,根据用户id获取,返回一个用户
	else if (url == "/getUser")
	{
		User user;
		//赋值
		user.id = FindPreStrOf(query, '=');


		dbuser db1;
		user=db1.getUser(user);
		//cout<<"\""
		string result = "{\"id\":\""+user.id+"\", \"password\" :\""+user.password
			+ "\", \"sex\" :\""+ user.sex + "\", \"age\" :\"" + to_string(user.age) + "\", \"birth\" :\"" + user.birth
			+ "\", \"email\" :\"" + user.email + "\", \"level\" :\"" + to_string(user.level)
			+ "\", \"win\" :\"" + to_string(user.win)+ "\", \"lose\" :\"" + to_string(user.lose)+ "\", \"winrate\" :\"" + to_string(user.winrate) + "\" }";
		rep.set_body(result);
		message.reply(rep);
	}
	//登录校验   无此账号返回-1，密码错误返回0，正确返回1
	else if (url == "/login")
	{
		User user;
		//处理url
		StringSplit(query, '&', a);
		//赋值
		user.id = FindPreStrOf(a[0], '=');
		user.password = FindPreStrOf(a[1], '=');


		dbuser db1;
		int result = db1.login(user.id, user.password);
		rep.set_body(to_string(result));
		message.reply(rep);
	}
	//修改对局  赢了i为1，输了i为-1  需要传入一个健全的user，信息要都有
	else if (url == "/change")
	{
		User user;
		//处理url
		StringSplit(query, '&', a);
		//赋值
		user.id = FindPreStrOf(a[0], '=');
		int i = stoi(FindPreStrOf(a[1], '='));


		//
		dbuser db1;
		user = db1.getUser(user);
		dbuser db2;
		db2.change(user, i);
		rep.set_body("0");
		message.reply(rep);
	}
	else
	{
		rep.set_body("Not Real url");
		message.reply(rep);
	}
};


//发布服务位
class FirstIn
{
public:
	void chose();
};
void FirstIn::chose()
{
	try
	{
		utility::string_t address = U("http://*:8080");
		uri_builder uri(address);
		//声明变量时根据初始化表达式自动推断该变量的类型
		//auto 存储类
		auto addr = uri.to_uri().to_string();
		CommandHandler handler(addr);
		//开始监听
		handler.open().wait();
		ucout << utility::string_t(U("Listening for requests at: ")) << addr << endl;
		ucout << U("Press ENTER key to quit...") << endl;
		string line;
		getline(cin, line);
		handler.close().wait();
	}
	catch (exception& ex)
	{
		//意外处理
		ucout << U("Exception: ") << ex.what() << endl;
		ucout << U("Press ENTER key to quit...") << endl;
		string line;
		getline(cin, line);
	}
}



int main(int argc, char argv[])
{
	FirstIn temp1;
	temp1.chose();
	//vector<string> a(3);
	//StringSplit("id=Alex&i=1", '&', a);
	//cout <<a[0]<< endl;
	//cout << a[1]<< endl;
	//cout << FindPreStrOf(a[0], '=') << endl;
	//cout << stoi(FindPreStrOf(a[1], '=')) << endl;
}




//int main(int argc, char argv[])
//{
//	try
//	{
//		utility::string_t address = U("http://*:8080/test");
//		uri_builder uri(address);
//		//声明变量时根据初始化表达式自动推断该变量的类型
//		//auto 存储类
//		auto addr = uri.to_uri().to_string();
//		CommandHandler handler(addr);
//		//开始监听
//		handler.open().wait();
//		ucout << utility::string_t(U("Listening for requests at: ")) << addr << endl;
//		ucout << U("Press ENTER key to quit...") << endl;
//		string line;
//		getline(cin, line);
//		handler.close().wait();
//	}
//	catch (exception& ex)
//	{
//		//意外处理
//		ucout << U("Exception: ") << ex.what() << endl;
//		ucout << U("Press ENTER key to quit...") << endl;
//		string line;
//		getline(cin, line);
//	}
//	return 0;
//}