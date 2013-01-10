#include "so.h"

#include <stdio.h>

/**
 * 包含构造和析构的动态库
 * 函数属性也可以用在main.c文件中，调用main()函数之前会调用构造函数
 */
__attribute__ ((constructor)) void so_init(void) {
    printf("so_init\n");
}

__attribute__ ((destructor)) void so_fini(void) {
    printf("so_fini\n");
}

void main_print(void);          /* 未声明无警告，-Wall -Werror有必要 */
void so_print(void) {
    printf("so_print\n");
    main_print();
    /**
     * main_print()是 main.c中定义的函数
     * 调用execute file中发布出来的函数；编译时使用-rdynamic属性
     */
}
