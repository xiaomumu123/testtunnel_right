//Client.c
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <netinet/in.h> 
#include "Pattern.h"
#include <time.h>
#define PORT 44444  

#define MAX_BYTE 5000
 
const char* ip = "192.168.1.129"; //"192.168.1.121"
 void delay(){
	 int i = 0;
	for(i=0;i<1000;i++){
		for(int j= 0; j< 1000; j++){}
	}
}

void makeHeader(char * SendBuf, int start, int length)
 {	 
    char temp[4] = { 0xff , 0xff , 0xff , 0xff};
    memcpy(SendBuf ,temp,4);
    
    
	int  data_temp1 = start;
	char data_start[4];
	*((int *)data_start) = data_temp1;
	memcpy(SendBuf + 4 ,data_start,4);
	
	int  data_temp2 = length;
	char data_length[4];
	*((int *)data_length) = data_temp2;
	memcpy(SendBuf + 8,data_length,4);
}
 
 
int main(int argc,char **argv)  
{  
    int sockfd;  
    int err,n;  
	int count = 0;
    struct sockaddr_in addr_ser;  
      
    sockfd = socket(AF_INET,SOCK_STREAM,0);       //创建套接字  
    if(sockfd == -1)  
    {  
        printf("socket error\n");  
        return -1;  
    }  
      
    bzero(&addr_ser,sizeof(addr_ser));       
      
    /*用通配地址和指定端口号装填一个网际接口地址结构*/   
    addr_ser.sin_family = AF_INET;  
    //addr_ser.sin_addr.s_addr = inet_addr(ip);     
    addr_ser.sin_port = htons(PORT); 
	inet_pton(AF_INET, ip, &addr_ser.sin_addr.s_addr);                 
      
    //TCP:客户(sockfd)向服务器(套接口地址结构)发起连接，主动请求  

    //服务器的IP地址和端口号有参数addr_ser指定  
	int sendbuflen=32 * 1024 * 1024;
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,(const char*)&sendbuflen,sizeof(int));

    err = connect(sockfd,(struct sockaddr *)&addr_ser,sizeof(addr_ser));     
    if(err == -1)  
    {  
        printf("connect error\n");  
        return -1;  
    }  
      
	
    printf("connect with server...\n");  

      
    //数据传输  
	unsigned int sendTime = 1000001;
	unsigned int seed = time(0);
	srand(seed);					//按时间随机
	
	int   BufLen = MAX_BYTE; // 
	char  SendBuf[MAX_BYTE];
    memset(SendBuf, 0 , sizeof(SendBuf));
	int start;
	int length;
    char *srcData_start;
	
    while(--sendTime)  
    {     
		/////////////////////
		int start  = rand() % 1520;
		int length = rand() % 1400 + 64;
		count = count +length + 12;
		
		
		makeHeader(SendBuf,start,length);
		
		srcData_start = srcData + start;
		
		memcpy(SendBuf + 12, srcData_start, length);
		
		
        int size = send(sockfd,SendBuf,length + 12,0);
        
//        for(int i = 0 ; i < length + 12 ; i++){
//            printf("%x, " , *(SendBuf + i));
//        }
//        for(int i = 0 ; i < length + 12 ; i++){
//            printf("%c, " , *(SendBuf + i));
//        }
        
        
        if(sendTime == 1)
            printf("length = %d, start = %d ,size = %d\n",length , start , size);	
		//delay();
    }
	printf("count = %d\n",count);
    return 0;  
} 
