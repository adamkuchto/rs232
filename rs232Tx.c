#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define portTx "/dev/ttyS2"

struct termios optionsTx;

int tx() {
	int fdtx;
	fdtx = open(portTx, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fdtx == -1) {
		perror("Błąd podczas otwierania portu");
		return 1;
	}

	tcgetattr(fdtx, &optionsTx);
	cfsetispeed(&optionsTx, B9600);
	cfsetospeed(&optionsTx, B9600);
	optionsTx.c_cflag |= (CLOCAL | CREAD);
	optionsTx.c_cflag &= ~PARENB;
	optionsTx.c_cflag &= ~CSTOPB;
	optionsTx.c_cflag &= ~CSIZE;
	optionsTx.c_cflag |= CS8;
	tcsetattr(fdtx, TCSANOW, &optionsTx);

	char send_data[] = "dane przesłane prze rs232\n\r";
	int n = write(fdtx, send_data, strlen(send_data));
	if (n < 0) {
		close(fdtx);
		perror("bład podczas wysyłania");
		return 1;
	}
	close(fdtx);
	return 0;
}

int main() {
	int error = 0;
		error = tx();
		if (error)
			printf("BŁĄD");

	return 0;
}
