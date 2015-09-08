#include <iostream>
#include "mysql.h"

using namespace std;

int main()
{
	cout << "== AI Begin ==" << endl;
	const char *host = "robot.mokfc.com";
	const char *user = "root";
	const char *pwd = "513939";
	const char *db = "mysql";
	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, pwd, db, 0, NULL, 0)) {
		printf("%s\n", mysql_error(&mysql));
	} else {
		printf("Connect successed\n");
	}
	mysql_close(&mysql);
	cout << "=== AI End ===" << endl;
	return 0;
}
