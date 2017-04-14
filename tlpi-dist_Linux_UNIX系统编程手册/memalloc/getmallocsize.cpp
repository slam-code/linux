#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

template <class T>
int GetNewBufferSize8(T *p)
{   //获取一个指针所指向内存空间的长度
	if (p)
		return *(int*)((char *)p - 8) / sizeof(*p);
	else
		return 0;
}

template <class T>
int GetNewBufferSize16(T *p)
{   //获取一个指针所指向内存空间的长度
	if (p)
		return *(int*)((char *)p - 16) / sizeof(*p);
	else
		return 0;
}
int main(int argc, char const *argv[])
{
	float **temPix;
	temPix = (float**)malloc(3* sizeof(float*));//例如获取一个动态二维数组的长度
	for (int i = 0; i < 3; i++)
		temPix[i] = (float*)malloc(4 * sizeof(float));

	if(GetNewBufferSize8(temPix)!=3)
		perror("tempix != 3");
	if(GetNewBufferSize8(temPix[0])!=4)
		perror("tempix[0] != 4");

	printf("\n\n");

	if(GetNewBufferSize16(temPix)!=3)
		perror("tempix != 3");
	if(GetNewBufferSize16(temPix[0])!=4)
		perror("tempix[0] != 4");
	return 0;
}