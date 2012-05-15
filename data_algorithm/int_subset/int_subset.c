/**
 * input two file with ints;output the same;
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BITSPERWORD 32
#define SHIFT 5
#define MASK 0x1F
#define N 0xFFFFFFFF
int a[1 + N/BITSPERWORD];
 
void set (int i) {        a[i>>SHIFT] |=  (1<<(i & MASK)); }
void clr (int i) {        a[i>>SHIFT] &= ~(1<<(i & MASK)); }
int  test(int i) { return a[i>>SHIFT] &   (1<<(i & MASK)); }

int main(int argc, char *argv[])
{
	if (argc != 3) {
		exit(1);
	}
	char *file1 = argv[1];
	char *file2 = argv[2];

	FILE *rfp = fopen(file1, "r");
	if (rfp == NULL) {
		exit(2);
	}
	char sint[sizeof("4294967295") + 1] = {0};
	while (fgets(sint, sizeof(sint), rfp) != NULL) {
		int tmp = atoi(sint);
		set(tmp);
	}
	if (fclose(rfp) != 0) {
		exit(3);
	}

	rfp = fopen(file2, "r");
	FILE *wfp = fopen("out.txt", "w");
	if (rfp == NULL || wfp == NULL) {
		exit(4);
	}
	while (fgets(sint, sizeof(sint), rfp) != NULL) {
		int tmp = atoi(sint);
		if (test(tmp)) {
			snprintf(sint, sizeof(sint), "%d\n", tmp);
			fwrite(sint, strlen(sint), 1, wfp);
		}
	}
	if (fclose(rfp) != 0 || fclose(wfp) != 0) {
		exit(5);
	}

	return 0;
}
