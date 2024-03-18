#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

using namespace std;

bool exit_flag=false;
thread t_send, t_recv;
int client_socket;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void recv_message(int client_socket);

int main()
{
	if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=htons(10000); // Port no. of server
	client.sin_addr.s_addr=INADDR_ANY;
	//client.sin_addr.s_addr=inet_addr("127.0.0.1"); // Provide IP address of server
	bzero(&client.sin_zero,0);

	if((connect(client_socket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	signal(SIGINT, catch_ctrl_c);
	char name[MAX_LEN];
	cout<<"Enter your name : ";
	cin.getline(name,MAX_LEN);
	send(client_socket,name,sizeof(name),0);

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chat-room ======   "<<endl<<def_col;

	thread t1(send_message, client_socket);
	thread t2(recv_message, client_socket);

	t_send=move(t1);
	t_recv=move(t2);

	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
			
	return 0;
}
// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="#exit";
	send(client_socket,str,sizeof(str),0);
	exit_flag=true;
	t_send.detach();
	t_recv.detach();
	close(client_socket);
	exit(signal);
}
