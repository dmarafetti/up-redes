/*
 * fork_execv_lsof.c
 *
 * Tests the fork + execv pattern by forking and in the child invoking
 * lsof to list all TCPv4 ports in LISTEN state.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

/** Path to lsof (Linux often has it in /usr/bin; macOS in /usr/sbin). */
#define LSOF_PATH "/usr/bin/lsof"

int main(void)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /*
         * Child: replace this process image with lsof.
         * Arguments: lsof -a -i 4 -s TCP:LISTEN
         *   -a         AND conditions (IPv4 and LISTEN)
         *   -i 4       IPv4 only
         *   -s TCP:LISTEN  only sockets in LISTEN state
         */
        char *argv[] = {
            "lsof",
            "-a",
            "-i", "4",
            "-s", "TCP:LISTEN",
            NULL
        };

        (void)execv(LSOF_PATH, argv);

        /* execv returns only on failure */
        perror("execv " LSOF_PATH);
        _exit(EXIT_FAILURE);
    }

    /* Parent: wait for child and report exit status */
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        printf("Child exited with status %d\n", code);
        return code;
    }
    if (WIFSIGNALED(status)) {
        printf("Child killed by signal %d\n", WTERMSIG(status));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
