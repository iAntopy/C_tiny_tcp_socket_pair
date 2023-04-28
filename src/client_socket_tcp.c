/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_socket_tcp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:40:01 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/28 04:40:38 by iamongeo         ###   ########.fr       */
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

typedef struct	s_tcp_client
{
	int	sockfd;
	SA_in	sa;
}	t_tcpc;


int	client_clear(char *e, t_tcpc *cl, int exit_code)
{
	if (e)
		fprintf(stderr, "TCP Client : %s\n", e);
	if (cl)
	{
		if (cl->sockfd)
			close(cl->sockfd);
	}
	return (exit_code);
}

int	main(int argc, char **argv)
{
	t_tcpc	cl;
	char	*sendline = "GET / HTTP/1.1\r\n\r\n";
	ssize_t	sentbytes;
	ssize_t	recvbytes;
	char	recvline[4092];
//	char	*line;
	
	(void)argv;
	memset(&cl, 0, sizeof(cl));

	cl.sa.sin_family = AF_INET;
	cl.sa.sin_port = htons(SERVER_PORT);

	if (argc < 2)
		return (client_clear("Missing ip address", &cl, EXIT_FAILURE));
	if (argc > 2)
		sendline = argv[2];

	printf("Try open socket \n");
	if ( (cl.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (client_clear("socket creation failed", &cl, EXIT_FAILURE));
	printf("Try open socket successful\n");

	if (inet_pton(AF_INET, argv[1], &cl.sa.sin_addr) <= 0)
		return (client_clear("string ip to bin convertion failed", &cl, EXIT_FAILURE));
	printf("ip address convertion SUCCESS\n");

	if (connect(cl.sockfd, (SA *)&cl.sa, sizeof(SA)) < 0)
		return (client_clear("connection to ip failed", &cl, EXIT_FAILURE));
	printf("Connection SUCCESS\n");

	printf("Sending msg : %s\n", sendline);
	if ((sentbytes = write(cl.sockfd, sendline, strlen(sendline))) < 0)
		return (client_clear("msg transmission FAILED", &cl, EXIT_FAILURE));
	printf("msg transmission SUCCESS\n");


	if ((recvbytes = read(cl.sockfd, recvline, 4090)) < 0)
		return (client_clear("socket read FAILED", &cl, EXIT_FAILURE));
	printf("socket read SUCCESS. Read %zd bytes.\n", recvbytes);
	recvline[recvbytes] = '\0';
	printf("received msg : %s\n", recvline);

	printf("Closing connection and quiting. Good bye.");
	return (client_clear(NULL, &cl, EXIT_SUCCESS));
}
