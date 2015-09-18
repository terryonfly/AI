#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "mysql.h"
#include <vector>
#include <cstring>
#include <map>
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

map<string, word *> word_map;

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

word *check_word(utfstring *utfword, const char *table)
{
	printf(".");
	fflush(stdout);
	mysql_select_db(&mysql, "corpus");
	sprintf(query_buf,"select * from %s where %s.word = '%s' limit 1;", table, table, utfword->c_str());
	if(mysql_query(&mysql,query_buf)) {
		fprintf(stderr, "check_word : Query failed (%s)\n", mysql_error(&mysql));
		return NULL;
	}
	MYSQL_RES *res;
	if (!(res = mysql_store_result(&mysql))) {
		fprintf(stderr, "check_word : Couldn't get result from %s\n", mysql_error(&mysql));
		return NULL;
	}
	MYSQL_ROW row;
	word *sword = NULL;
	if ((row = mysql_fetch_row(res))) {
		char *szOrbits = row[4];
		char* pEnd;
		double probability;
		probability = strtold(szOrbits, &pEnd);
		char *word_type = row[2];
//		printf("row[1][2] = %s, %s, %g\n", row[1], row[2], probability);
		sword = new word(utfword, word_type, probability);
	}
	if (res)
		mysql_free_result(res);
	return sword;
}

void add_word_to_cache(utfstring *utfword, word *a_word)
{
	string key(utfword->c_str());
	if (a_word) {
		word *value = new word(a_word->utfword, a_word->word_type, a_word->word_probability);
		word_map[key] = value;
	} else {
		word_map[key] = NULL;
	}
}

word *search_word_from_cache(utfstring *utfword, bool *found)
{
	map<string, word *>::iterator word_map_iter;
	string key(utfword->c_str());
	word_map_iter = word_map.find(key);
	if (word_map_iter != word_map.end()) {
		word *reuse_word = NULL;
		if (word_map_iter->second)
			reuse_word = new word(word_map_iter->second->utfword, word_map_iter->second->word_type, word_map_iter->second->word_probability);
		printf("o");
		fflush(stdout);
		*found = true;
		return reuse_word;
	}
	*found = false;
	return NULL;
}

vector<sentence *> split_word(utfstring *utfstr)
{
	vector<sentence *> sentences;
	bool is_single_word = true;
	for (int len = (utfstr->length() > 10) ? 10 : utfstr->length(); len > 0; len--) {
		printf("\n%d", len);
		utfstring *first_utfword = utfstr->substring(0, len);
		word *first_word = NULL;
		bool found;
		printf("-%s", first_utfword->c_str());
		fflush(stdout);
		if (!first_word)
			first_word = search_word_from_cache(first_utfword, &found);
	if (!found) {
		if (!first_word)
			first_word = check_word(first_utfword, "word_sogou");
		if (!first_word) {
			first_word = check_word(first_utfword, "word_cn");
		} else {
			if (strcmp(first_word->word_type,"|") == 0) {
//				printf("finding word type in word_cn -- ");
				word *pre_first_word = check_word(first_utfword, "word_cn");
				if (pre_first_word) {
//					printf("found\n");
					delete(first_word);
					first_word = pre_first_word;
				} else {
//					printf("not found\n");
				}
			}
		}
		if (!first_word && len == 1)
			first_word = check_word(first_utfword, "char_cn");
		if (!first_word && len == 1) {
			first_word = new word(first_utfword);
			first_word->is_new_char = true;
		}
		printf("+");
		fflush(stdout);
		add_word_to_cache(first_utfword, first_word);
	}
		if (first_word) {
			printf("S");
			fflush(stdout);
			if (utfstr->length() == len) {// Im the last wordi
				printf("Y");
				fflush(stdout);
				sentence *parent_sentence = new sentence();
				parent_sentence->add_word(first_word);
				delete(first_word);
				first_word = NULL;
				sentences.push_back(parent_sentence);
			} else {
				printf("N");
				fflush(stdout);
				utfstring *substr = utfstr->substring(len, utfstr->length() - len);
				vector<sentence *> sub_sentences = split_word(substr);
				printf("\nC");
				fflush(stdout);
				printf(" = %d ", (int)sub_sentences.size());
				fflush(stdout);
				for (int i = 0; i < sub_sentences.size(); i++) {
					printf("X1 ");
					fflush(stdout);
					sentence *parent_sentence = new sentence();
					printf("X2 ");
					fflush(stdout);
					parent_sentence->add_word(first_word);
					printf("X3 ");
					fflush(stdout);
					parent_sentence->add_sentence(sub_sentences[i]);
					printf("X4 ");
					fflush(stdout);
					//delete(sub_sentences[i]);
					printf("X5 ");
					fflush(stdout);
					//sub_sentences[i] = NULL;
					printf("X6 ");
					fflush(stdout);
					sentences.push_back(parent_sentence);
					printf("X7 ");
					fflush(stdout);
				}
				printf("R ");
				fflush(stdout);
				//delete(first_word);
				//first_word = NULL;
				sub_sentences.clear();
			}
		}
		printf("E%d ", len);
		fflush(stdout);
	}
	printf("T ");
	fflush(stdout);
	return sentences;
}

sentence *get_best_sentence(utfstring *utfstr)
{
	for (map<string, word *>::iterator word_map_it = word_map.begin(); word_map_it != word_map.end(); word_map_it ++) {
		if (word_map_it->second)
			delete(word_map_it->second);
	}
	word_map.clear();
	printf("==============================\n\n");
	printf("Checking : %s\n", utfstr->c_str());
	printf("Proccessing");
	fflush(stdout);
	vector<sentence *> sentences = split_word(utfstr);
	printf("d\n");
//	printf("has split %d sentences\n", (int) sentences.size());
	sentence *best_sentence = new sentence();
	for (int i = 0; i < sentences.size(); i++) {
//		printf("- %s\n", sentences[i]->c_str());
		if (best_sentence->length() == 0) {
			delete(best_sentence);
			best_sentence = sentences[i]->copy();
		} else {
			if (best_sentence->get_score() < sentences[i]->get_score()) {
				delete(best_sentence);
				best_sentence = sentences[i]->copy();
			}
		}
		delete(sentences[i]);
	}
	sentences.clear();
	return best_sentence;
}

void feedback_word(word *update_word)
{
//	printf("fb = %s\n", update_word->utfword->c_str());
	mysql_select_db(&mysql, "corpus");
	sprintf(query_buf,"select * from word where word.word = '%s' limit 1;", update_word->utfword->c_str());
	if(mysql_query(&mysql,query_buf)) {
		fprintf(stderr, "feedback_word : Query failed (%s)\n", mysql_error(&mysql));
		return;
	}
	MYSQL_RES *res;
	if (!(res = mysql_store_result(&mysql))) {
		fprintf(stderr, "feedback_word : Couldn't get result from %s\n", mysql_error(&mysql));
		return;
	}
	MYSQL_ROW row;
	if ((row = mysql_fetch_row(res))) {// update
		sprintf(query_buf,"update `corpus`.`word` set `word`.`repeat` = (`word`.`repeat`+1) where `word`='%s' limit 1;", update_word->utfword->c_str());
		if(mysql_query(&mysql,query_buf)) {
			fprintf(stderr, "Update failed (%s)\n",mysql_error(&mysql));
			return;
		}
		printf("Updated \"%s\" %s\n", update_word->utfword->c_str(), update_word->word_type);
//		printf("Update %d rows\n",(int)mysql_affected_rows(&mysql));
	} else {// insert
		sprintf(query_buf,"insert into `corpus`.`word` values(NULL, '%s', '%s', 1, 0.00000000000000000000001);", update_word->utfword->c_str(), update_word->word_type);
		if(mysql_query(&mysql,query_buf)) {
			fprintf(stderr, "Update failed (%s)\n",mysql_error(&mysql));
			return;
		}
		printf("Inserted \"%s\" %s\n", update_word->utfword->c_str(), update_word->word_type);
//		printf("Insert %d rows\n",(int)mysql_affected_rows(&mysql));
	}
	mysql_free_result(res);
}

void feedback_sentence(sentence *best_sentence)
{
	word *update_word;
	for (int i = 0; i < best_sentence->length(); i++) {
		update_word = best_sentence->at(i);
		feedback_word(update_word);
	}
}

void use_sentence(utfstring *utfstr)
{
	sentence *best_sentence = get_best_sentence(utfstr);
//	printf("======== sentence ========\n");
	printf("%s\n", best_sentence->c_str());
//	printf("======== feedback ========\n");
	feedback_sentence(best_sentence);
	delete(best_sentence);
//	printf("==========================\n");
	printf("\n");
	printf("Word Map Size : %d\n", (int)word_map.size());
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

	char *read_buffer = (char *)malloc(11664761);
	int fd;
	fd = open(data_path, O_RDWR | O_CREAT, S_IRWXU);
	read(fd, read_buffer, 11664761);
//	printf("Readed : %s\n", read_buffer);

	int i = 0;
	string sentence = "";
	while (read_buffer[i] != 0x00) {
		if ((signed char)read_buffer[i] < 0) {
			char zh_char[3] = {read_buffer[i++], read_buffer[i++], read_buffer[i++]};
			if (is_end_zh_symbol(zh_char[0], zh_char[1], zh_char[2])) {
				if (!sentence.empty() && i > 2729) {
					utfstring *utfstr = new utfstring(sentence.c_str());
					use_sentence(utfstr);
					printf("done byte : %d/%d %f%%\n", i, 11664761, i * 100.00000000 / 11664761.00000000);
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
				if (!sentence.empty() && i > 2729) {
					utfstring *utfstr = new utfstring(sentence.c_str());
					use_sentence(utfstr);
					printf("done byte : %d/%d %f%%\n", i, 11664761, i * 100.00000000 / 11664761.00000000);
					delete(utfstr);
				}
				sentence = "";
			} else if (is_normal_en_symbol(en_char)) {
				// do nothing
			} else {
//				sentence = sentence.append({en_char});
			}
		}
	}
	if (!sentence.empty()) {
		utfstring *utfstr = new utfstring(sentence.c_str());
		use_sentence(utfstr);
		delete(utfstr);
	}

	mysql_close(&mysql);

	printf("\n=== AI End ===\n");
	return 0;
}
