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
		appLog("[#] ����ʧ�ܣ�");
	}
}


void import() {
	std::ifstream stream(logFile.c_str(), (std::ios::in));
	if (!stream.fail()) {
		std::string temp = "";
		char buffer[65535] ;
		memset(buffer, '\0', sizeof(buffer)); // ��ʼ��bufferΪ��
		while (!stream.eof()) {
			stream.read(buffer, 65535); // ��
			temp += buffer; // �ۼӵ�������
		}
		size_t pos; // ���з�����
		while ((pos = temp.find("\n")) != -1) { // ����\n
			std::string item = temp.substr(0, pos); // ��ȡ
			size_t pos2 = item.find(std::string("->")); // ��ȡ��
			std::string key = item.substr(0, pos2);
			std::string value = item.substr(pos2 + 2, item.length() - pos2 - 2);
			keyRecord[key.c_str()] = std::stoi(value.c_str());
			temp = temp.substr(pos + 1, temp.length() - pos); // �ظ�ֵ��ȡ�㵽��β����
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
	appLog("[#] �־û��ɹ�!");
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
		appLog(std::string("[���]") + line.c_str());
		iter++;
	}
}