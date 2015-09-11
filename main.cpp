#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "mysql.h"
#include "string.h"

using namespace std;

char end_en_symbol[] = {' ',',',':',';','!','?','\r','\n'};
char normal_en_symbol[] = {' ','!','\\','*','(',')','-','_','+','=','{','}','[',']',':',';','\'','\"',',','<','>','?','/','|','@','#','$','%','^','&'};

string end_zh_symbol[] = {"。","，","：","；","！","？"};
string normal_zh_symbol[] = {"　","。","，","：","；","！","？","～","@","#","￥","%","……","（","）","——","｛","｝","【","】","“","”","‘","’","《","》","、","`"};

template <class T>
int get_array_len(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}

bool is_end_en_symbol(char a_char)
{
	int i = 0;
	for (i = 0; i < get_array_len(end_en_symbol); i ++)
		if (a_char == end_en_symbol[i])
			return true;
	return false;
}

bool is_normal_en_symbol(char a_char)
{
	int i = 0;
	for (i = 0; i < get_array_len(normal_en_symbol); i ++)
		if (a_char == normal_en_symbol[i])
			return true;
	return false;
}

bool is_end_zh_symbol(char a_char_0, char a_char_1, char a_char_2)
{
	string a_char({a_char_0, a_char_1, a_char_2});
	int i = 0;
	for (i = 0; i < get_array_len(end_zh_symbol); i ++)
		if (a_char == end_zh_symbol[i])
			return true;
	return false;
}

bool is_normal_zh_symbol(char a_char_0, char a_char_1, char a_char_2)
{
	string a_char({a_char_0, a_char_1, a_char_2});
	int i = 0;
	for (i = 0; i < get_array_len(normal_zh_symbol); i ++) {
		if (a_char == normal_zh_symbol[i])
			return true;
	}
	return false;
}

void split_word(string sentence)
{
	printf("%s\n", sentence.c_str());
}

int main()
{
	printf("== AI Begin ==\n");
	const char *host = "robot.mokfc.com";
	const char *user = "root";
	const char *pwd = "513939";
	const char *db = "mysql";
	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, pwd, db, 0, NULL, 0)) {
		printf("%s\n", mysql_error(&mysql));
	} else {
		printf("DB Connect successed\n");
	}
	mysql_close(&mysql);

	char path[80];
	char data_path[100];
	getcwd(path, sizeof(path));
	sprintf(data_path, "%s%s", path, "/data.txt");
	printf("Reading file : %s\n", data_path);

	char *read_buffer = (char *)malloc(409600);
	int fd;
	fd = open(data_path, O_RDWR | O_CREAT, S_IRWXU);
	read(fd, read_buffer, 409600);
//	printf("Readed : %s\n", read_buffer);

	int i = 0;
	string sentence = "";
	while (read_buffer[i] != 0x00) {
		if (read_buffer[i] < 0) {
			char zh_char[3] = {read_buffer[i++], read_buffer[i++], read_buffer[i++]};
			if (is_end_zh_symbol(zh_char[0], zh_char[1], zh_char[2])) {
//				printf("ze - %c%c%c\n", zh_char[0], zh_char[1], zh_char[2]);
				if (!sentence.empty()) {
					split_word(sentence);
				}
				sentence = "";
			} else if (is_normal_zh_symbol(zh_char[0], zh_char[1], zh_char[2])) {
//				printf("zn - %c%c%c\n", zh_char[0], zh_char[1], zh_char[2]);
			} else {
//				printf("z  - %c%c%c\n", zh_char[0], zh_char[1], zh_char[2]);
				sentence = sentence.append({zh_char[0], zh_char[1], zh_char[2]});
			}
		} else {
			char en_char = read_buffer[i++];
			if (is_end_en_symbol(en_char)) {
//				printf("ce - %c\n", en_char);
				if (!sentence.empty()) {
					split_word(sentence);
				}
			} else if (is_normal_en_symbol(en_char)) {
//				printf("cn - %c\n", en_char);
			} else {
//				printf("c  - %c\n", en_char);
				sentence = sentence.append({en_char});
			}
		}
	}

	printf("=== AI End ===\n");
	return 0;
}
