/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* p75 ch5.3 */


#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


/*
fcntl()：获取/修改文件访问权限和状态falgs
*/
int
main(int argc, char *argv[])
{
    int fd;
    
    fd = open(argv[1], O_RDONLY);
    int flags,accessMode;
    flags=fcntl(fd,F_GETFL);
    if(flags==-1)
        errExit("fcntl");

    if(flags & O_SYNC)
        printf("fd are sync !\n");
    else
        printf("fd are not sync\n");


    accessMode=flags&O_ACCMODE;
    if(accessMode==O_WRONLY || accessMode==O_RDWR)
        printf("file is writable \n");
    else printf("file can not write\n");

    close(fd);



    //---------//
    printf("\n");
    int  fd2 = open(argv[1], O_RDONLY|O_SYNC|O_RDWR);
    flags=fcntl(fd2,F_GETFL);
    if(flags & O_SYNC)
        printf("fd2 are sync !\n");
    else
        printf("fd2 are not sync\n");


    accessMode=flags&O_ACCMODE;
    if(accessMode==O_WRONLY || accessMode==O_RDWR)
        printf("file is writable \n");
    else printf("file can not write\n");
    close(fd2);

    //--------//
    printf("\n");
    int  fd3 = open(argv[1], O_RDONLY|O_SYNC|O_RDWR);

    flags=fcntl(fd3,F_GETFL);
    flags|=O_APPEND;
    if(fcntl(fd3,F_SETFL,flags)==-1)
        errExit("fcntl");


    if(flags&O_APPEND)
        printf("file can O_APPEND \n");
    else printf("file can not O_APPEND\n");

    
    exit(EXIT_SUCCESS);
}
