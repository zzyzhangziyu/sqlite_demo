// tt.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

	}
	printf("\n");
	return 0;
}
void PreUpdate(
	void *pCtx,                   /* Copy of third arg to preupdate_hook() */
	sqlite3 *db,                  /* Database handle */
	int op,                       /* SQLITE_UPDATE, DELETE or INSERT */
	char const *zDb,              /* Database name */
	char const *zName,            /* Table name */
	sqlite3_int64 iKey1,          /* Rowid of row about to be deleted/updated */
	sqlite3_int64 iKey2           /* New rowid value (for a rowid UPDATE) */
) {
	printf("TableName:%s\n", zName);
	sqlite3_value * pVal;
	sqlite3_preupdate_new(db, 0, &pVal);
	printf("%d\n",sqlite3_value_int(pVal));
	sqlite3_preupdate_new(db, 1, &pVal);
	printf("%d\n", sqlite3_value_int(pVal));
}

char create[] = "create table person (id int,val int)";
char insert[] = "insert into person values (%d,%d)";
char select[] = "select * from person";

int main(int argc, char **argv) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[1024];

	srand(time(NULL));

	rc = sqlite3_open("a.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}

	sqlite3_preupdate_hook(db, PreUpdate, NULL);

	sprintf(sql, select);
	//sprintf(sql, insert, rand(), rand());
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

	}
	sqlite3_close(db);
	system("pause");
	return 0;

}