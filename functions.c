#include "SysEng.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>	
#include <openssl/ssl.h>
int parseCL(int argc, char **argv, char**url,int *port, int *profile){
    int hflag = 0;
    int c;
    int option_index = 0;
    struct option long_options[] = {{"url",required_argument,NULL,'u'},{"help",no_argument,NULL,'h'},{"ssl",no_argument,NULL,'s'},{"profile",required_argument,NULL,'p'}};
    while((c= getopt_long(argc,argv,"shu:p:",long_options,&option_index)) != -1){
        switch(c){
            case 'u':
                *url = optarg;
                break;
            case 'h':
                hflag = 1;
                break;
            case 's':
                *port = 443;
                break;
            case 'p':
                *profile = atoi(optarg);
                break;
            default:
                break;
        }
    }
    if(hflag){
        printf("usage: ./syseng [options]\n\toptions:\n\t\t-h,--help\t\tDisplays flags and usage for the program\n\t\t-u,--url argument\tThese flags are required.Using the -u or --url flag requires an additional argument for the URL\n\t\t-p,--profile argument\tUsing the -p or -profile flag requires and additional numerical argument.If no option is given or an invalid is given, it will default to 0 which will not runn the program in profile mode\n\t\t-s,--ssl\t\tThis flag sets the port to the SSL port if specified. Some pages will require that you use SSL\n");
        return 0;    
      }else{
        if (*url == NULL){
            return 0;
        }
    }
    return 1;
}

int makeReq(char*host,char*path, int port,int profile){
    //Make a socket and connect to the address 
    int on = 1;
    int sockfd;
    struct hostent *h = gethostbyname(host);
    struct sockaddr_in addr; 
    if(h == NULL){
        return -1;
    }
    char *hAddr = h->h_addr;
    int hLen = h->h_length;
    memmove(&addr.sin_addr,hAddr,hLen);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0){
		printf("\nInvalid Address. Could not connect.\n");
		return -1;
	}
    if(sockfd < 0){
        printf("Socket Error");
        return -1;
    }
    char req[4096 + 1]; 
    char resp[4096 + 1];
    int resp_size = 0;
    int total = 0;
    struct timeval begin,now;
    double time;
    int timeout_window = 4;
    snprintf(req,4096,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",path,host);
    if(port == 443){
        SSL_load_error_strings ();
        SSL_library_init ();
        OpenSSL_add_all_algorithms();
        SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method ());
        SSL *conn = SSL_new(ssl_ctx);
        SSL_set_fd(conn, sockfd);
        if(SSL_connect(conn) != 1){
            return -1;
        }
        if(SSL_write(conn,req,strlen(req))>= 0){
            fcntl(sockfd,F_SETFL,O_NONBLOCK);
            gettimeofday(&begin , NULL);
            while(1){
                gettimeofday(&now , NULL);
                time = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
                if(time > timeout_window && total != 0){
			        break;
                }else if(time > timeout_window*2){
                    break;
                    }
                memset(resp ,0 , 4096);
                if((resp_size = SSL_read(conn,resp,4096))<0){
                    usleep(100000);
                }else{
                    total +=resp_size;
                    if (!profile){
                        printf("%s" , resp);
                    }
                    gettimeofday(&begin , NULL);
                }
            }
        }
    }else{
        if(send(sockfd,req,strlen(req),0) >= 0){
            fcntl(sockfd,F_SETFL,O_NONBLOCK);
            gettimeofday(&begin , NULL);
            while(1){
                gettimeofday(&now , NULL);
                time = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
                if(time > timeout_window && total != 0){
                    break;
                }else if(time > timeout_window*2){
                    break;
                }
                memset(resp ,0 , 4096);
                if((resp_size = recv(sockfd,resp,4096,0))<0){
                    sleep(0.1);
                }else{
                    total +=resp_size;
                    if (!profile){
                        printf("%s" , resp);
                    }
                    gettimeofday(&begin , NULL);
                }
            }
        }
    }
    return total;
}