#pragma once
#include <string>
#include<fstream>
int k_max_v = 1;
double keyHeight = 41;
double keyWidth = 48;
bool autoTray = false;
double keyPadingY = 0;
double keyPadingX = 2;
bool firstRun = true;
static std::string logs = "";
static std::string filename = "KeyBoardHook.conf";
static void appLog(std::string text) {
	logs.append(text + "\r\n");
}

static std::string getLog() {
	printf("%s", logs.c_str());
	return logs;
}

static void clearLog() {
	logs = "";
}

static void saveConfig(std::string data) {
	std::ofstream stream(filename.c_str(), (std::ios::trunc | std::ios::binary));
	if (!stream.fail()) {
		stream.write(data.c_str(), data.length());
		stream.close();
	}
	else {
		appLog("[#] ����ʧ�ܣ�");

	}
}
static void importConfig() {
	std::ifstream stream(filename.c_str(), (std::ios::in));
	if (!stream.fail()) {
		std::string temp = "";
		char buffer[65535];
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
			if (key == "keyHeight") keyHeight = std::stoi(value.c_str());
			else if (key == "keyWidth") keyWidth = std::stoi(value.c_str());
			else if (key == "autoTray") autoTray = std::stoi(value.c_str()) == 1 ? true: false;
			temp = temp.substr(pos + 1, temp.length() - pos); // �ظ�ֵ��ȡ�㵽��β����
		}
	}

}