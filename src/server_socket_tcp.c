/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_socket_tcp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:40:01 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/28 04:40:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 3738

typedef struct sockaddr_in	SA_in;
typedef struct sockaddr		SA;

typedef struct	s_tcp_server
{
	SA_in	sa;
	int	listenfd;
	int	connfd;
}	t_tcps;


int	server_clear(char *e, t_tcps *sl, int exit_code)
{
	if (e)
	{
		fprintf(stderr, "TCP server : %s\n", e);
		perror("What happend : ?");
	}
	if (sl)
	{
		if (sl->listenfd)
			close(sl->listenfd);
		if (sl->connfd)
			close(sl->connfd);
	}
	return (exit_code);
}

void	set_sentline(char *sendline, char *newline)
{
	const char	*header = "HTTP/1.0 200 OK\r\n\r\n";
	const size_t	headerlen = strlen(header);

	strncpy(sendline, header, headerlen);
	strncpy(sendline + headerlen, newline, 4090);
	printf("SETTING UP sendline to : %s\n", sendline);
}

int	main(int argc, char **argv)//int argc, char **argv)
{
	t_tcps	sl;
	char	sendline[4092];
//	char	*default_line = "HTTP/1.0 200 OK\r\n\r\nHELLO !";
	ssize_t	sentbytes;
	char	recvdata[4092];
	ssize_t	recvbytes;
	
	memset(&sl, 0, sizeof(sl));

	if (argc >= 2)
		set_sentline(sendline, argv[1]);
	else
		set_sentline(sendline, "HELLO !\n");
	sl.sa.sin_family = AF_INET;
	sl.sa.sin_addr.s_addr = htons(INADDR_ANY);
	sl.sa.sin_port = htons(SERVER_PORT);

//	if (argc != 2)
//		return (server_clear("Missing ip address", &sl, EXIT_FAILURE));

	printf("Try open socket \n");
	if ( (sl.listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (server_clear("socket creation failed", &sl, EXIT_FAILURE));
	printf("Try open socket successful\n");

	if (bind(sl.listenfd, (SA *)&sl.sa, sizeof(SA)) < 0)
		return (server_clear("socket binding failed", &sl, EXIT_FAILURE));
	printf("bind SUCCESS\n");

	if (listen(sl.listenfd, 10) < 0)
		return (server_clear("listening call failed", &sl, EXIT_FAILURE));
	printf("listen SUCCESS\n");

	printf("SERVER STARTED ! LISTENING TO PORT %d\n", SERVER_PORT);
	while (1)
	{
		printf("Waiting for connection on port %d\n", SERVER_PORT);
		sl.connfd = accept(sl.listenfd, NULL, NULL);

		if ((recvbytes = read(sl.connfd, recvdata, 4090)) < 0)
			return (server_clear("reading connfd failed", &sl, EXIT_FAILURE));
		
		recvdata[recvbytes] = '\0';
		printf("received from client : %s\n", recvdata);
		if (strncmp(recvdata, "POST nogoodbum", 10) == 0)
		{
			close(sl.connfd);
			break ;			
		}
		else if (strncmp(recvdata, "POST msg:", 9) == 0)
			set_sentline(sendline, recvdata + 9);
		if ((sentbytes = write(sl.connfd, sendline, strlen(sendline))) < 0)
			return (server_clear("writing to connected client failed", &sl, EXIT_FAILURE));
		printf("send response (%zd bytes) to client SUCCESS ! Closing connection.\n", sentbytes);
		close(sl.connfd);
	}
/*
	printf("Sending msg : %s\n", sendline);
	if ((sentbytes = write(sl.sockfd, sendline, strlen(sendline))) < 0)
		return (server_clear("msg transmission FAILED", &sl, EXIT_FAILURE));
	printf("msg transmission SUCCESS\n");

	line = get_next_line(sl.sockfd);
	while (line)
	{
		printf("o- : %s", line);
		line = get_next_line(sl.sockfd);
	}
*/
	
	printf("Closing connection and quiting. Good bye.");
	return (server_clear(NULL, &sl, EXIT_SUCCESS));
}
