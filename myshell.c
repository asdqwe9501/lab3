#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARG_COUNT 128

void handle_signal(int signo) {
    if (signo == SIGINT) {
        printf("\nReceived SIGINT (Ctrl-C). Use 'exit' to quit.\n");
    } else if (signo == SIGQUIT) {
        printf("\nReceived SIGQUIT (Ctrl-\\). Use 'exit' to quit.\n");
    }
}

void execute_command(char *cmd) {
    char *args[MAX_ARG_COUNT];
    char *token = strtok(cmd, " ");
    int arg_count = 0, background = 0;
    int in_redirect = 0, out_redirect = 0;
    char *input_file = NULL, *output_file = NULL;

    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            in_redirect = 1;
            token = strtok(NULL, " ");
            input_file = token;
        } else if (strcmp(token, ">") == 0) {
            out_redirect = 1;
            token = strtok(NULL, " ");
            output_file = token;
        } else if (strcmp(token, "&") == 0) {
            background = 1;
        } else {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }

    args[arg_count] = NULL;

    if (arg_count == 0) return;

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();

    if (pid == 0) {  // Child process
        if (in_redirect && input_file) {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("Input file error");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        if (out_redirect && output_file) {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("Output file error");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        execvp(args[0], args);
        perror("Command execution failed");
        exit(1);
    } else if (pid > 0) {  // Parent process
        if (!background) {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("Fork failed");
    }
}

void parse_and_execute(char *line) {
    char *commands[MAX_ARG_COUNT];
    char *token = strtok(line, "|");
    int command_count = 0;

    while (token != NULL) {
        commands[command_count++] = token;
        token = strtok(NULL, "|");
    }

    int pipefds[2 * (command_count - 1)];
    for (int i = 0; i < command_count - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("Pipe error");
            exit(1);
        }
    }

    for (int i = 0; i < command_count; i++) {
        pid_t pid = fork();

        if (pid == 0) {  // Child process
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            if (i < command_count - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * (command_count - 1); j++) {
                close(pipefds[j]);
            }

            execute_command(commands[i]);
            exit(0);
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
    }

    for (int i = 0; i < 2 * (command_count - 1); i++) {
        close(pipefds[i]);
    }

    for (int i = 0; i < command_count; i++) {
        wait(NULL);
    }
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);

    char line[MAX_COMMAND_LENGTH];

    while (1) {
        printf("myshell> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';  // Remove newline character

        if (strlen(line) == 0) {
            continue;
        }

        parse_and_execute(line);
    }

    return 0;
}
