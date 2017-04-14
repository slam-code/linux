#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{


	
/*	int *p = (int *) malloc(10*sizeof(int));
	p++;
	free(p);*/

	int *p = (int *) malloc(10*sizeof(int));
	int *q = p;
	p++;
	free(q);// 这样写没有错.

	p = (int *) malloc(10*sizeof(int));
	p++;//free 一个不是 malloc 来的指针，都是非常危险的举动 
	free(p);// 这样写编译连接没有错! 一运行就出现 Debug Assertion Failed!
	return 0;
	//malloc/free线程安全意味着他要加锁
	//https://www.zhihu.com/question/29161424
/*
当调用malloc(size)时，实际分配的内存大小大于size字节，这是因为在分配的内存区域头部有类似于
struct control_block { 
   unsigned size; 
   int used;};
 这样的一个结构，如果malloc函数内部得到的内存区域的首地址为void *p,那么它返回给你的就是
 p + sizeof(control_block)，而调用free(p)的时候，该函数把p减去sizeof(control_block)，
 然后就可以根据((control_blcok*)p)->size得到要释放的内存区域的大小。
 这也就是为什么free只能用来释放malloc分配的内存，如果用于释放其他的内存，会发生未知的错误。
*/
}