#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <codecvt>
#include "mysql.h"
#include "string.h"
#include <iomanip>
#include <type_traits>

using namespace std;

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

	char *read_buffer = (char *)malloc(4096);
	int fd;
	fd = open(data_path, O_RDWR | O_CREAT, S_IRWXU);
	read(fd, read_buffer, 4096);
	printf("Readed : %s\n", read_buffer);

	int i = 0;
	while (read_buffer[i] != 0x00) {
		if (read_buffer[i] < 0) {
			printf("z - %c%c%c\n", read_buffer[i++], read_buffer[i++], read_buffer[i++]);
		} else {
			printf("c - %c\n", read_buffer[i++]);
		}
	}



	printf("=== AI End ===\n");
	return 0;
}
