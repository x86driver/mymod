#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

static char *sysfs_pid[4] = {"/sys/devices/platform/tccxxx-camera.0/rear_out_pid",
				"/sys/devices/platform/tccxxx-camera.0/rear_in_pid",
				"/sys/devices/platform/tccxxx-camera.0/drm_out_pid",
				"/sys/devices/platform/tccxxx-camera.0/drm_in_pid"};

static char *cmd[4] = {"/system/bin/rearcameraout.sh",
			"/system/bin/rearcamerain.sh",
			"/system/bin/drout.sh",
			"/system/bin/drin.sh"};

void create_signal(int status, void (*handler)(int))
{
        char buffer[32];
        struct sigaction act;
        pid_t pid = getpid();
        FILE *fp = fopen(sysfs_pid[status], "w");
        if (!fp) {
                perror("fopen");
                exit(1);
        }
        snprintf(buffer, sizeof(buffer), "%d", pid);
        fwrite(buffer, strlen(buffer), 1, fp);
        fclose(fp);

        act.sa_handler = handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        if (sigaction(SIGUSR1, &act, NULL) < 0) {
                perror("sigaction");
        }
}

void start_rear_out(int signo)
{
	system(cmd[0]);
}

void start_rear_in(int signo)
{
        system(cmd[1]);
}

void start_drm_out(int signo)
{
        system(cmd[2]);
}

void start_drm_in(int signo)
{
        system(cmd[3]);
}

void rear_out(int n)
{
	printf("rear out\n");
	create_signal(n, start_rear_out);
	while (1)
		pause();
}

void rear_in(int n)
{
	create_signal(n, start_rear_in);
	while (1)
		pause();
}

void drm_out(int n)
{
	create_signal(n, start_drm_out);
	while (1)
		pause();
}

void drm_in(int n)
{
	create_signal(n, start_drm_in);
	while (1)
		pause();
}

int main()
{
	pid_t pid[4];
	int status;
	void (*func[4])(int) = {rear_out, rear_in, drm_out, drm_in};
	int i;
	for (i = 0; i < 4; ++i) {
		pid[i] = fork();
		if (pid[i] == 0) {
			func[i](i);
			exit(1);
		}
	}

	for (i = 0; i < 4; ++i) {
		waitpid(pid[i], &status, 0);
		printf("pid: %u exit code: %d\n", pid[i], WEXITSTATUS(status));
	}
	printf("end\n");
	return 0;
}
