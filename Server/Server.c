//Server.c
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <unistd.h>  
#include <netinet/in.h>  
#include <errno.h> 
#include "Pattern.h" 
#include <fcntl.h>
#define PORT 44444  
  
#define MAX_BYTE 250000

int main(int argc,char **argv)  
{  
    //char temp = 0xff;
    //printf("%x \n",temp);
    int ser_sockfd,cli_sockfd;  
    int err;  
    int addlen;  
    struct sockaddr_in ser_addr;  
    struct sockaddr_in cli_addr;  
    char recvData[MAX_BYTE];
	memset(recvData, 0, sizeof(recvData)); 
      
    ser_sockfd = socket(AF_INET,SOCK_STREAM,0);          //创建套接字  
    if(ser_sockfd == -1)  
    {  
        printf("socket error:%s\n",strerror(errno));  
        return -1;  
    }  
      
	int on = 1;
    int ret = setsockopt(ser_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  
    memset(&ser_addr, 0, sizeof(ser_addr));
    /*在待捆绑到该TCP套接口(sockfd)的网际套接口地址结构中填入通配地址(INADDR_ANY) 
    和服务器的众所周知端口(PORT,这里为6666)，这里捆绑通配地址是在告知系统：要是系统是 
    多宿主机(具有多个网络连接的主机)，我们将接受宿地址为任何本地接口的地址*/       
    ser_addr.sin_family = AF_INET;  
    ser_addr.sin_addr.s_addr = INADDR_ANY; 
    ser_addr.sin_port = htons(PORT);  
      
    //将网际套接口地址结构捆绑到该套接口  
    err = bind(ser_sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));    
    if(err == -1)  
    {  
        printf("bind error:%s\n",strerror(errno));  
        return -1;  
    } 



 
    //将套接口转换为一个监听套接口，监听等待来自客户端的连接请求  
    err = listen(ser_sockfd,5);                                        
    if(err == -1)  
    {  
        printf("listen error\n");  
        return -1;  
    } 


      
    printf("listen the port:\n");  
    int start;
	int length;
	int errorCount = 0;
	unsigned int time = 0;
	float percent;
	char result;  
	int size = 0;
	int moreData = 0;
	int count = 0;
	
	char* temp_recvData = recvData + 1500;
    char* pStart = NULL;
    char* pCopy = NULL;

        addlen = sizeof(cli_addr);  
        //等待阻塞，等待客户端申请，并接受客户端的连接请求  
        //accept成功，将创建一个新的套接字，并为这个新的套接字分配一个套接字描述符  
        cli_sockfd = accept(ser_sockfd,(struct sockaddr *)&cli_addr,&addlen);     
        if(cli_sockfd == -1)  
        {  
            printf("accept error\n");
            return -1;
        }  
		

		int nRecvBuf=32 * 1024 * 1024;
	 	setsockopt(cli_sockfd, SOL_SOCKET, SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	 	
	 	int flags = fcntl(cli_sockfd, F_GETFL, 0);
	 	//fcntl(cli_sockfd, F_SETFL, flags|O_NONBLOCK);

         printf("waiting for client...\n");
    
    int readMore = 1;
    int totalByte = 0;
    
    int exitCount = 0;
    
    //debug 
    unsigned int ipCount = 0;
    int totalLengt = 0; 
    int eagainCount = 0;
    
    while(1)  
    {   
		size = 0;
		readMore = 1;

		while(readMore){
            int n = recv(cli_sockfd,temp_recvData,80000,0);
            if(n < 0)  
            {  
                if(errno == EAGAIN){
                    eagainCount = eagainCount + 1;
                    if(size == 0)                      
                        break; // a litte bit small data use
                }
                else{
                    //printf("n < 0 return \n");
                    return -1;
                }
            }
            else if(n == 0){ //disconnect
                //printf("\nipCount = %u , totalLengt = %d \n", ipCount, totalLengt);
                exitCount = exitCount + 1;
                if(exitCount > 100)
                    return 0;
                else
                    break;
            }
//            else if(n > 0)
//            {
//                if(n > 20000)
//                    printf("n > 20000 , n = %d \n", n);
//                
//                size = size + n;
//                if(size < 160000)
//                    readMore = 1;
//                else
//                    readMore = 0;
//            }
		//}
		
//		if(size == 0) // if(errno == EAGAIN){ associate
//		{
//		    continue;
//		}
		
//		if(errno == EAGAIN)
//		    printf("Error size , size = %d , eagainCount = %d \n" , 
//		            size , eagainCount);
		
        //printf("after read while size = %d\n",size);
        size = n;
		pStart = temp_recvData - moreData;
		totalByte = size + moreData;
		
		while(1){
			if(totalByte <=12)
			{
				//printf("111\n");
				moreData = totalByte;
				pCopy = pStart;
				if(moreData > 0)
				{
				    memcpy(temp_recvData - moreData, pCopy, moreData);
				}
				break;
			}
			if(totalByte > 12)
			{
				//printf("222\n");
				length= *((int *)(pStart + 8));
				
				//if(length > 1500){
				 //   printf("!!!!!! %x , %x , %x , %x \n", *(pStart) , *(pStart + 1), *(pStart+2), *(pStart + 3));
				//    printf("start = %d , length = %d , moreData = %d , totalByte = %d \n", 
				  //          *((int *)(pStart + 4)), *((int *)(pStart + 8)), moreData, totalByte);
				 //   for(int i = -20 ; i < 30; i++){
				  //      printf("%c , " , *(pStart + i));
				  //  }
				   // printf("\nipCount = %u , totalLengt = %d \n", ipCount, totalLengt);
				    //return -1;
				}

				if(totalByte < (length + 12) )
				{
					moreData = totalByte;
					//printf("101010 --- length = %d , moreData = %d , totalByte = %d \n" , 
					//        length, moreData, totalByte);
					pCopy = pStart;
					if(moreData > 0){
					    memcpy(temp_recvData - moreData, pCopy, moreData);
					    //printf("333memcpy\n");
					}
					break;
				} 
				else 
				{
					//printf("bitcompare\n");
					start = *((int *)(pStart+4));
					length = *((int *)(pStart+8));
					
					//totalLengt = totalLengt + length + 12;
					//ipCount = ipCount + 1;
					for(int i=0;i<length;i++)
					{
						result  = (*(pStart+i + 12)) ^ (*(srcData+start + i));
                        if ((result & 0x80) == 0x80)   
                            ++errorCount;
                        if ((result & 0x40) == 0x40)   
                            ++errorCount;
                        if ((result & 0x20) == 0x20)   
                            ++errorCount;
                        if ((result & 0x10) == 0x10)   
                            ++errorCount;
                        if ((result & 0x08) == 0x08)   
                            ++errorCount;
                        if ((result & 0x04) == 0x04)   
                            ++errorCount;
                        if ((result & 0x02) == 0x02)   
                            ++errorCount;
                        if ((result & 0x01) == 0x01)   
                            ++errorCount;
                        
					
					}
					
					time++;
					if(time % 300 == 0)
					{	
					printf("the packet length is %d byte,the error bit is %d \n",length,errorCount);
					}
					pStart = pStart + length + 12;
					totalByte = totalByte - length - 12;
					if(totalByte == 0){
					    moreData = 0;
					    break;
					}
	
				}				
			}
		} 
   }
return 0; 
}
  
