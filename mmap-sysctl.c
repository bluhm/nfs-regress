#include <sys/mman.h>
#include <sys/queue.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/tcp_seq.h>
#include <netinet/tcp_timer.h>
#include <netinet/tcp_var.h>

#include <err.h>
#include <fcntl.h>
#include <stdlib.h>

int
main(void)
{
	char file[] = "/nfs/nfsfile";
	int fd;
	char *p;

	if ((fd = open(file, O_WRONLY|O_CREAT, 0777)) == -1)
		err(1, "open '%s'", file);

	p = mmap(NULL, sizeof(struct tcpstat), PROT_READ, MAP_SHARED, fd, 0);
	if (p == NULL)
		err(1, "mmap '%s'", file);

	return (0);
}
