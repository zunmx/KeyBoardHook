#pragma once

#pragma  comment(lib, "user32")
#pragma  comment(lib, "advapi32")
#include <windows.h>
#include <map>
#include <string>
#include "commons.h"
#include<fstream>
std::map<std::string, std::int64_t> keyRecord;
int number = 0;
std::string logFile = "records.ini";

void toWrite(std::string data) {
	std::ofstream stream(logFile.c_str(), (std::ios::trunc | std::ios::binary));
	if (!stream.fail()) {
		stream.write(data.c_str(), data.length());
		stream.close();
	}
	else {
		appLog("[#] 保存失败！");
	}
}


void import() {
	std::ifstream stream(logFile.c_str(), (std::ios::in));
	if (!stream.fail()) {
		std::string temp = "";
		char buffer[65535] ;
		memset(buffer, '\0', sizeof(buffer)); // 初始化buffer为空
		while (!stream.eof()) {
			stream.read(buffer, 65535); // 读
			temp += buffer; // 累加到缓冲区
		}
		size_t pos; // 换行符索引
		while ((pos = temp.find("\n")) != -1) { // 查找\n
			std::string item = temp.substr(0, pos); // 截取
			size_t pos2 = item.find(std::string("->")); // 截取点
			std::string key = item.substr(0, pos2);
			std::string value = item.substr(pos2 + 2, item.length() - pos2 - 2);
			keyRecord[key.c_str()] = std::stoi(value.c_str());
			temp = temp.substr(pos + 1, temp.length() - pos); // 重赋值截取点到结尾部分
		}
	}

}
void save() {
	std::map<std::string, std::int64_t>::iterator iter;
	iter = keyRecord.begin();
	std::string data = "";
	while (iter != keyRecord.end()) {
		data += iter->first + "->" + std::to_string(iter->second) + "\n";
		iter++;
	}
	toWrite(data);
	number = 0;
	appLog("[#] 持久化成功!");
}
void KRecord(std::string data) {
	if (keyRecord[data] == NULL) {
		keyRecord[data] = 1;
	}
	else {
		keyRecord[data] ++;
	}
	number++;
	if (number >= 10) {
		save();
	}
}

void print() {
	save();
	std::map<std::string, std::int64_t>::iterator iter;
	iter = keyRecord.begin();
	while (iter != keyRecord.end()) {
		std::string line = iter->first.c_str() + std::string("->" + std::string(std::to_string(iter->second).c_str()) + "\t\t");
		appLog(std::string("[输出]") + line.c_str());
		iter++;
	}
}