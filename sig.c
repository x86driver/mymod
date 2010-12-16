#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define CAMERA_DAEMON_SYSFS "/sys/devices/platform/tccxxx-camera.0/pid"

void start_app(int signo)
{
	system("/system/bin/startcamera.sh");
}

int main()
{
	char buffer[32];
	struct sigaction act;
	pid_t pid = getpid();
	FILE *fp = fopen(CAMERA_DAEMON_SYSFS, "w");
	if (!fp) {
		perror("fopen");
		exit(1);
	}
	snprintf(buffer, sizeof(buffer), "%d", pid);
	fwrite(buffer, strlen(buffer), 1, fp);
	fclose(fp);

	act.sa_handler = start_app;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGUSR1, &act, NULL) < 0) {
		perror("sigaction");
	}
	while (1)
		pause();

	return 0;
}
