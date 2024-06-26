
// file: domainclient.c
// date: 2024/06/20
// receive string and send back the upper string.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>

#define SERV_ADDR "serv.socket"
#define CLIE_ADDR "clie.socket"

int main(void)
{
    int cfd, len;
    struct sockaddr_un servaddr, cliaddr;
    char buf[4096];

    cfd = socket(AF_UNIX, SOCK_STREAM, 0);

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, CLIE_ADDR);

    // get the actual length of client addr 
    len = offsetof(struct sockaddr_un, sun_path) + strlen(cliaddr.sun_path);
    unlink(CLIE_ADDR);
    bind(cfd, (struct sockaddr*)&cliaddr, len); // client needs bind, the default bind does not work

    ////////////////////// set the servr addr.
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, SERV_ADDR);

    connect(cfd, (struct sockaddr *)&servaddr, len);
    while(1)
    {
      if(fgets(buf, sizeof(buf), stdin) != NULL)
      {
        write(cfd, buf, strlen(buf));
        len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
      }
    }
    close(cfd);

    return 0;

}
