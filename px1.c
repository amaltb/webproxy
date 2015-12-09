//too long can be made short

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include<pthread.h>
#include<time.h>

void* doprocessing (int sock);
void* timesp(double t1);
struct node
{
	int count;
	char* a;
	
};
int flag;
int itr=0;
int cnt=0;

struct node v[1000];
char ind[5][100];
char data[5][100000];
int i;

int main(int argc,char* argv[])
{
//	strcpy(ind[0],"amalsalkjdddddddddddddddddddddddddddddddddddddffffffffffffffffffffffffffffffff;;;;;;;;;;;;;;;;  ;dfd");
/*	ind[0][1]='q';
	ind[0][0]='p';
	ind[0][2]=' ';
	ind[0][3]='r';
*/

ind[0][1]='t';
ind[1][1]='t';
ind[2][1]='t';
ind[3][1]='t';
ind[4][1]='t';

	

	clock_t t1;
	t1=clock();	
	pthread_t tim;
	pthread_create(&tim,NULL,&timesp,t1);


	puts(ind[0]);	
	pid_t pid;
	struct sockaddr_in cli_addr,serv_addr;
	
	int newsockfd,sockfd;
	char buff[1000];
	
	if(argc<2)
	{
		perror("./proxy <port_no>");
		exit(0);
	}
	
	printf("RUNNING PROXY SERVER.....\n");
	
	int portno=atoi(argv[1]);
	
	printf("%d\n",portno);

	bzero((char*)&serv_addr,sizeof(serv_addr));
	bzero((char*)&cli_addr,sizeof(cli_addr));
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(portno);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	
	sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd<0)
	{
		perror("Socket initilization failed..");
		exit(0);
	}
	int n=bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	if(n<0)
	{
		perror("Binding error...");
		exit(0);
	}
	
	listen(sockfd,50);
	
	int clilen=sizeof(cli_addr);
	
//	acceptence:
	
	struct node y;
	y.a=malloc(1);
	strcpy(y.a,"amal");
	v[0]=y;
	
	while (1) 
    {
	
		newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	
		if(newsockfd<0)
		{
			error("Problem in accepting connection");
		}
		
		pthread_t pts;
        pthread_create(&pts,NULL,&doprocessing,newsockfd);
        pthread_join(pts,NULL);
    }
    return 0;
}

void* doprocessing (int sock)
{
		printf("%s\n",v[0].a);
		char tmp[1000000];
//		tmp=malloc(1);
		int fg=0;
//		printf("\nInside 1...\n");
		struct sockaddr_in host_addr;
		struct hostent* host;
		int flag=0,newsockfd1,sockfd1,n,port=0,i,p;
		char buffer[1000],t1[400],t2[400],t3[10],temp1[400],buff[1000],bu[300000];
		char* temp=NULL;
		bzero(buffer,1000);
		puts(ind[0]);
//		puts(data[0]);
		n=read(sock,buffer,1000);
		
		puts(buffer);
		
		sscanf(buffer,"%s %s %s",t1,t2,t3);
		if(strncmp(t1,"GET",3)==0 && strncmp(t3,"HTTP/1.1",8)==0 && strncmp(t2,"http://",7)==0)
		{
			strcpy(t1,t2);
			
			int flag=0;	
			for(i=7;i<strlen(t2);i++)
			{
				if(t2[i]==':')
				{
					flag=1;
					break;
				}
			}
			
			temp=strtok(t2,"//");
				
			if(flag==0)
			{
				port=80;
				temp=strtok(NULL,"/");
			}
			else
			{
				temp=strtok(NULL,":");
			}
			sprintf(t2,"%s",temp);
			printf("host = %s\n",t2);
			
		
		
			printf("%d",strcmp(ind[0],t2));
			i=0;
			for(i=0;i<5;i++)
			{
				printf("\ninside\n");
				if(ind[i]==NULL)
				{
					break;
				}
				else if(strcmp(ind[i],t2)==0)
				{
//					printf("\ninside if\n");
					fg=1;
					bzero(bu,300000);
					int k;
					
					printf("%d\n",strlen(data[i]));
					
					for(k=0;k<strlen(data[i]);k++)
					{
//						strcpy(buffer,data[i]);
						bu[k]=data[i][k];
					}
					n=write(sock,bu,300000);
					break;
				}
			}
//			printf("%d",i);
			if(fg==1)
			{
				close(sock);
				return NULL;	
			}
			
			
			
											
			host=gethostbyname(t2);
			
			strcat(t1,"^]");
			temp=strtok(t1,"//");
			temp=strtok(NULL,"/");
			if(temp!=NULL)
			temp=strtok(NULL,"^]");
			printf("\npath = %s\nPort = %d\n",temp,port);
//			printf("\nOver\n");
			
			bzero((char*)&host_addr,sizeof(host_addr));
			host_addr.sin_port=htons(port);
			host_addr.sin_family=AF_INET;
			bcopy((char*)host->h_addr,(char*)&host_addr.sin_addr.s_addr,host->h_length);
			   
			sockfd1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			newsockfd1=connect(sockfd1,(struct sockaddr*)&host_addr,sizeof(struct sockaddr));
			sprintf(buffer,"\nConnected to %s  IP - %s\n",t2,inet_ntoa(host_addr.sin_addr));
			if(newsockfd1<0)
			error("Error in connecting to remote server");
			   
			printf("\n%s\n",buffer);
			bzero((char*)buffer,sizeof(buffer));
			if(temp!=NULL)
				sprintf(buffer,"GET /%s %s\r\nHost: %s\r\nConnection: close\r\n\r\n",temp,t3,t2);
			else
				sprintf(buffer,"GET / %s\r\nHost: %s\r\nConnection: close\r\n\r\n",t3,t2);
			 			 
			n=send(sockfd1,buffer,strlen(buffer),0);
			printf("\n%s\n",buffer);
			if(n<0)
				error("Error writing to socket");
			else
			{
				do
				{
					bzero((char*)buffer,500);
					n=recv(sockfd1,buffer,500,0);
					if(!(n<=0))
					{
						strcat(tmp,buffer);
						send(sock,buffer,n,0);			
					}
				}while(n>0);
				
				strcat(tmp,"\0");
				int z;

				
				
				
				int j;
	
				printf("%s,%s,%d\n",t2,v[0].a,strcmp(t2,v[0].a));	
	
				for(j=0;j<1000;j++)
				{
//					printf("%d\n",j);
					if(v[j].a!=NULL)
					{
						if(strcmp(v[j].a,t2)==0)
						{
							if(v[j].count==1)
							{
								if(itr<4)
								{
//									printf("inside,%d,%s\n",itr,v[j].a);
									strcpy(ind[itr],v[j].a);				
//									printf("%d %s\n",itr,ind[itr]);
//									printf("%d\n",strlen(tmp));
									
									for(z=0;z<strlen(tmp);z++)
									{
//										printf("inside z");
										data[0][z]=tmp[z];
									}
				
									itr++;
									break;
								}
								else
								{
									itr=0;
									strcpy(ind[itr],v[j].a);
									for(z=0;z<strlen(tmp);z++)
									{
//										printf("inside z");
										data[0][z]=tmp[z];
									}
									itr++;
									break;
								}
							}
							else
							{
								v[j].count=v[j].count+1;
		//						printf("%d\n",v[j].count);
								break;		
							}
						}
					}
				}
				
				if(i==5 && j==1000)
				{
//					printf("inside1000\n");
					struct node x;
					x.a=malloc(1);	
					strcpy(x.a,t2);
					x.count=1;
//					puts(x.a);
					v[cnt]=x;
//					printf("\n%d %s\n",cnt,v[cnt].a);
					cnt++;
				}				
			}
		}
		else
		{
			send(sock,"400 : BAD REQUEST\nONLY HTTP REQUESTS ALLOWED",18,0);
		}
			
//		free(tmp);
		close(sockfd1);
		close(sock);	
	return NULL;
}
	
	
void* timesp(double t1)
{
	
//	printf("%c\n",ind[1][1]);
	clock_t t2;
	int k,z;
//	int Port=80;
	
	char h[100];
	
	while(1)
	{
		t2=clock();
		double timespent=(double)(t2-t1)/CLOCKS_PER_SEC;
		if(timespent==10)
		{
			for(k=0;k<5;k++)
			{
				printf("%d\n",k);
				if(ind[k][1]=='t')
				continue;
				else
				{
					bzero(h,100);
					strcpy(h,ind[k]);
					char tmp[1000000];
					puts(h);
			//		tmp=malloc(1);
					int fg=0;
			//		printf("\nInside 1...\n");
					struct sockaddr_in host_addr;
					struct hostent* host;
					int flag=0,newsockfd1,sockfd1,n,port=80,i,p;
					char buffer[1000],t1[400],t2[400],t3[10],temp1[400],buff[1000],bu[300000];
					char* temp=NULL;
					
					host=gethostbyname(h);
					
					strcat(t1,"^]");
					temp=strtok(t1,"//");
					temp=strtok(NULL,"/");
					if(temp!=NULL)
					temp=strtok(NULL,"^]");
					printf("\npath = %s\nPort = %d\n",temp,port);
		//			printf("\nOver\n");
					
					bzero((char*)&host_addr,sizeof(host_addr));
					host_addr.sin_port=htons(port);
					host_addr.sin_family=AF_INET;
					bcopy((char*)host->h_addr,(char*)&host_addr.sin_addr.s_addr,host->h_length);
					   
					sockfd1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
					newsockfd1=connect(sockfd1,(struct sockaddr*)&host_addr,sizeof(struct sockaddr));
					sprintf(buffer,"\nConnected to %s  IP - %s\n",t2,inet_ntoa(host_addr.sin_addr));
					if(newsockfd1<0)
					error("Error in connecting to remote server");
					   
	//				printf("\n%s\n",buffer);
					bzero((char*)buffer,sizeof(buffer));
					if(temp!=NULL)
						sprintf(buffer,"GET /%s %s\r\nHost: %s\r\nConnection: close\r\n\r\n",temp,t3,t2);
					else
						sprintf(buffer,"GET / %s\r\nHost: %s\r\nConnection: close\r\n\r\n",t3,t2);
								 
					n=send(sockfd1,buffer,strlen(buffer),0);
	//				printf("\n%s\n",buffer);
					if(n<0)
						error("Error writing to socket");
					else
					{
						do
						{
							bzero((char*)buffer,500);
							n=recv(sockfd1,buffer,500,0);
							if(!(n<=0))
							{
								strcat(tmp,buffer);			
							}
						}while(n>0);	
					}
					
					for(z=0;z<strlen(tmp);z++)
					{
	//					printf("inside z");
						data[k][z]=tmp[z];
					}
					close(sockfd1);
				}
			}
			t1=t2;
		}
	}
	pthread_exit(NULL);
}
	
		
	
	
	
	
	



