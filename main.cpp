#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "mysql.h"
#include <vector>
#include "Modules/utfstring.h"
#include "Modules/sentence.h"

using namespace std;

MYSQL mysql;
const char *host = "robot.mokfc.com";
const char *user = "root";
const char *pwd = "513939";
char query_buf[1024];

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
	for (i = 0; i < get_array_len(end_zh_symbol); i ++) {
		if (a_char == end_zh_symbol[i]) {
			return true;
		}
	}
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

word *check_word(utfstring *utfword, )
{
	mysql_select_db(&mysql, "corpus");
	sprintf(query_buf,"select * from word_sogou where word_sogou.word = '%s' limit 1;",utfword->c_str());
	if(mysql_query(&mysql,query_buf)) {
		fprintf(stderr, "Query failed (%s)\n",mysql_error(&mysql));
		return NULL;
	}
	MYSQL_RES *res;
	if (!(res = mysql_store_result(&mysql))) {
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(&mysql));
		return NULL;
	}
	MYSQL_ROW row;
	word *sword = NULL;
	while ((row = mysql_fetch_row(res))) {
		printf("row[1][2] = %s, %s\n", row[1], row[2]);
		sword = new word(utfword, row[2]);
	}
	mysql_free_result(res);
	return sword;
}

vector<sentence *> split_word(utfstring *utfstr)
{
	vector<sentence *> sentences;
	bool is_single_word = true;
	for (int len = (utfstr->length() > 8) ? 8 : utfstr->length(); len > 0; len--) {
		utfstring *first_utfword = utfstr->substring(0, len);
		word *first_word = NULL;
		if (len == 1) {
			if (is_single_word) {
//				printf("single - %s\n", first_utfword->c_str());
				first_word = new word(first_utfword);
			}
		} else {
			first_word = check_word(first_utfword);
			if (first_word) {
//				printf("mutil - %s\n", first_utfword->c_str());
				is_single_word = false;
			}
		}
		if (first_word) {
			if (utfstr->length() == len) {// Im the last word
				sentence *parent_sentence = new sentence();
				parent_sentence->add_word(first_word);
				sentences.push_back(parent_sentence);
			} else {
				utfstring *substr = utfstr->substring(len, utfstr->length() - len);
				vector<sentence *> sub_sentences = split_word(substr);
				for (int i = 0; i < sub_sentences.size(); i++) {
					sentence *parent_sentence = new sentence();
					parent_sentence->add_word(first_word);
					parent_sentence->add_sentence(sub_sentences[i]);
					sentences.push_back(parent_sentence);
				}
			}
		}
	}
	return sentences;
}

sentence *get_best_sentence(utfstring *utfstr)
{
	vector<sentence *> sentences = split_word(utfstr);
	printf("has split %d sentences\n", (int) sentences.size());
	for (int i = 0; i < sentences.size(); i++) {
		printf("%s\n", sentences[i]->c_str());
	}
	return NULL;
}

int main()
{
	printf("\n== AI Begin ==\n\n");
	mysql_init(&mysql);

	int ret = mysql_set_character_set(&mysql, "utf8");
	if (!mysql_real_connect(&mysql, host, user, pwd, "corpus", 0, NULL, 0)) {
		printf("DB connect err : %s\n", mysql_error(&mysql));
	} else {
		printf("DB connect successed!\n");
	}

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
		if ((signed char)read_buffer[i] < 0) {
			char zh_char[3] = {read_buffer[i++], read_buffer[i++], read_buffer[i++]};
			if (is_end_zh_symbol(zh_char[0], zh_char[1], zh_char[2])) {
				if (!sentence.empty()) {
					utfstring *utfstr = new utfstring(sentence.c_str());
					get_best_sentence(utfstr);
					delete(utfstr);
				}
				sentence = "";
			} else if (is_normal_zh_symbol(zh_char[0], zh_char[1], zh_char[2])) {
				// do nothing
			} else {
				sentence = sentence.append({zh_char[0], zh_char[1], zh_char[2]});
			}
		} else {
			char en_char = read_buffer[i++];
			if (is_end_en_symbol(en_char)) {
				if (!sentence.empty()) {
					utfstring *utfstr = new utfstring(sentence.c_str());
					get_best_sentence(utfstr);
					delete(utfstr);
				}
				sentence = "";
			} else if (is_normal_en_symbol(en_char)) {
				// do nothing
			} else {
				sentence = sentence.append({en_char});
			}
		}
	}
	if (!sentence.empty()) {
		utfstring *utfstr = new utfstring(sentence.c_str());
		get_best_sentence(utfstr);
		delete(utfstr);
	}

	mysql_close(&mysql);

	printf("\n=== AI End ===\n");
	return 0;
}
