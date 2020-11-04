// 2020-11-04
// 使用C/C++实现一个函数，int * match （const char* substr, const char* target）, 返回substr在target中出现的所有位置


#include <stdio.h>

/**
 * Loop Match
 * substr:bb, target:bbbbbbb,return:0,1,2,3,4,5
 */
int * match(const char * substr, const char * target) {

    int reset_match = 0;
    const char * substr_ptr = substr;
    const char * target_match_prefix = target, * target_match_tmp = target;
    
    if (substr == NULL || target == NULL) {
        return NULL;
    }
    
    while (*target_match_tmp != '\0') {
        
        printf("%d\n", target_match_prefix - target);
        
        if (*substr_ptr == *target_match_tmp) {
            target_match_tmp++;
            substr_ptr++;

            // match succ
            if (*substr_ptr == '\0') {
                printf("match:%d\n", target_match_prefix - target);

                reset_match = 1;
            }
            
        } else {
            reset_match = 1;
        }

        if (reset_match == 1) {
            reset_match = 0;

            // substr return to begin
            substr_ptr = substr;

            // target move 1,and start anther match
            target_match_prefix++;
            target_match_tmp = target_match_prefix;
        }
    }
}

int main(void) {

    const char * target = "ababbbabbb", * substr = "bb";

    match(substr, target);
    
    return 0;
}
