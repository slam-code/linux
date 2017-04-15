/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 24-3 */

/* footprint.c

   Using fork() + wait() to control the memory footprint of an application.

   This program contains a function that (artificially) consumes a large
   amount of memory. To avoid changing the process's memory footprint, the
   program creates a child process that calls the function. When the child
   terminates, all of its memory is freed, and the memory consumption of
   the parent is left unaffected.
*/
#define _BSD_SOURCE     /* To get sbrk() declaration from <unistd.h> in case
                           _XOPEN_SOURCE >= 600; defining _SVID_SOURCE or
                           _GNU_SOURCE also suffices */
#include <sys/wait.h>
#include "tlpi_hdr.h"

static int
func(int arg)
{
    int j;
// 0x100是一个十六进制数,换算十进制的方法如下:0x100=1*16*16+0*16+0*1=256 
    for (j = 0; j < 0x100; j++) 
        if (malloc(0x8000) == NULL)
            errExit("malloc");
    printf("Program break in child:  %10p\n", sbrk(0));

    return arg;
}

//temp_buff[1]*0x100 是什么意思:就是对temp_buff[1]左移8位，为了和其他字节拼接成一个16字节的数据.
//*0x100就是剩以256相当于左移8位
int
main(int argc, char *argv[])
{
    int arg = (argc > 1) ? getInt(argv[1], 0, "arg") : 0;
    pid_t childPid;
    int status;

    setbuf(stdout, NULL);           /* Disable buffering of stdout */

    printf("Program break in parent: %10p\n", sbrk(0));

    childPid = fork();
    if (childPid == -1)
        errExit("fork");

    if (childPid == 0)              /* Child calls func() and */
        exit(func(arg));            /* uses return value as exit status */

    /* Parent waits for child to terminate. It can determine the
       result of func() by inspecting 'status' */

    if (wait(&status) == -1) //status 子进程退出时的状态,信息存放一个整数的不同二进制位.
        errExit("wait");

    printf("Program break in parent: %10p\n", sbrk(0));

    printf("Status = %d %d\n", status, WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
}

/*
WEXITSTATUS:

可以获取子进程调传送给exit或用 exit 函数参数的低 8位。

1、WEXITSTATUS 是返回子进程的退出码，用来判断子进程的退出值。当WIFEXITED(status) 这个宏用来指出子进程是否为正常退出的，
如果是，它会返回一个非零值。

2、WEXITSTATUS(status)取得子进程exit()返回的结束代码，一般会先用WIFEXITED 来判断是否正常结束才能使用此宏。

*/



/*
wait的函数原型是：
pid_t wait(int *status)
进程一旦调用了wait，就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸
的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到
有一个出现为止。参数status用来保存被收集进程退出时的一些状态，它是一个指向int类型的指针。但如果我们对这个子进程是如何死
1/ 掉的毫不在意，只想把这个僵尸进程消灭掉，（事实上绝大多数情况下，我们都会这样想），我们就可以设定这个参数为NULL，就象下面这样：
pid = wait(NULL);
如果成功，wait会返回被收集的子进程的进程ID，如果调用进程没有子进程，调用就会失败，此时wait返回-1，同时errno被置为ECHILD。

2/ 如果参数status的值不是NULL，wait就会把子进程退出时的状态取出并存入其中，这是一个整数值（int），指出了子进程是正常退出还是
被非正常结束的（一个进程也可以被其他进程用信号结束，我们将在以后的文章中介绍），以及正常结束时的返回值，或被哪一个信号结束的
等信息。由于这些信息被存放在一个整数的不同二进制位中，所以用常规的方法读取会非常麻烦，人们就设计了一套专门的宏（macro）来完
成这项工作，下面我们来学习一下其中最常用的两个：
1，WIFEXITED(status) 这个宏用来指出子进程是否为正常退出的，如果是，它会返回一个非零值。
（请注意，虽然名字一样，这里的参数status并不同于wait唯一的参数--指向整数的指针status，而是那个指针所指向的整数，切记不要搞混了。）

2，WEXITSTATUS(status) 当WIFEXITED返回非零值时，我们可以用这个宏来提取子进程的返回值，如果子进程调用exit(5)退出，
WEXITSTATUS(status) 就会返回5；如果子进程调用exit(7)，WEXITSTATUS(status)就会返回7。请注意，如果进程不是正常退出的，
也就是 说，WIFEXITED返回0，这个值就毫无意义。

3,WIFSIGNALED(status)若为异常结束子进程返回的状态,则为真
当然，处理进程退出状态的宏并不止这两个，但它们当中的绝大部分在平时的编程中很少用到。

*/