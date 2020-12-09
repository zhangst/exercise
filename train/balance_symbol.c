#include "stack_list.h"
#include <stdio.h>
#include <assert.h>

/**
 * DSSA P52
 * 平衡符号
 */


enum bs_ret {
	BS_ERR,
	BS_NOT_BALANCE,
	BS_BALANCE
};
static enum bs_ret balance_symbol(const char *filename)
{
	struct stack *S = NULL;
	FILE *fp = NULL;
	int c = 0, c1 = 0;
	enum bs_ret ret = BS_ERR;

	assert(filename != NULL);

	if ((S = stack_create(100)) == NULL) {
		ret = BS_ERR;
		goto out;
	}
	stack_dump(S);
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "fopen error\n");
		ret = BS_ERR;
		goto freestack_out;
	}

	while ((c = fgetc(fp)) != EOF) {
		if (c == '(' || c == '[' || c == '{') {
			stack_dump(S);
			if (stack_isfull(S)) {
				ret = BS_ERR;
				goto closefile_out;
			} else {
				stack_push(S, c);
			}
		} else if (c == ')' || c == ']' || c == '}') {
			if (stack_isempty(S)) {
				ret = BS_NOT_BALANCE;
				goto closefile_out;
			} else {
				c1 = stack_topandpop(S);
				if ((c == ')' && c1 != '(') ||
				    (c == ']' && c1 != '[') ||
				    (c == '}' && c1 != '{')) {
					ret = BS_NOT_BALANCE;
					goto closefile_out;
				}
			}
		}
	}

	if (!stack_isempty(S)) {
		ret = BS_NOT_BALANCE;
		goto closefile_out;
	}

	ret = BS_BALANCE;

closefile_out:
	fclose(fp);
freestack_out:
	stack_dispose(S);
out:
	return ret;
}


int main(void)
{
	enum bs_ret ret = BS_ERR;

	ret = balance_symbol("./list.c");
	if (BS_BALANCE == ret) {
		printf("BS_BALANCE\n");
	} else if (BS_NOT_BALANCE == ret) {
		printf("BS_NOT_BALANCE\n");
	} else if (BS_ERR == ret) {
		printf("BS_ERR\n");
	}
	
	return 0;
}
