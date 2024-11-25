#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define portRx "/dev/ttyS3"

struct termios optionsRx;

int rx() {
	int fdrx;
	fdrx = open(portRx, O_RDWR | O_NOCTTY | O_SYNC);
	if (fdrx == -1) {
		perror("Błąd podczas otwierania portu");
		return 1;
	}

	tcgetattr(fdrx, &optionsRx);
	cfsetispeed(&optionsRx, B9600);
	cfsetospeed(&optionsRx, B9600);
	optionsRx.c_cflag |= (CLOCAL | CREAD);
	optionsRx.c_cflag &= ~PARENB;
	optionsRx.c_cflag &= ~CSTOPB;
	optionsRx.c_cflag &= ~CSIZE;
	optionsRx.c_cflag |= CS8;
	tcsetattr(fdrx, TCSANOW, &optionsRx);

	char recv_data[100];
	int n;
	while ((n = read(fdrx, recv_data, sizeof(recv_data) - 1)) < 0) {
		if (errno == EAGAIN) {
			usleep(100000);
			continue;
		} else {
			perror("Błąd odczytu");
			close(fdrx);
			return 1;
		}
	}

	recv_data[n] = '\0';
	printf("Received data: %s\n", recv_data);

	close(fdrx);
	return 0;
}

int main() {
	int error = 0;
	while(!error) {
		error = rx();
	}

	return 0;
}
