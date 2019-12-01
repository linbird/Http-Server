#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>

#ifndef CONFIG_H_
#define CONFIG_H_

class CONF_INFO{
public:
	int port;
	std::string private_key;
	std::string certificate;
	std::string source_root;
	std::string data_root;
	std::string default_page;
	std::string index_page;
	std::string root;
	std::string file;
	std::string login;
	std::string test;
	std::string states;
	CONF_INFO();
};

#endif