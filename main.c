#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

char *read_line(void) {
    char *line = NULL;
    size_t buffersize = 0;
    getline(&line, &buffersize, stdin);
    return line;
}

char **parse_line(char *line) {
    int token_buffer_size = MAX_ARGS;
    char **tokens = malloc(token_buffer_size * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "shimsh allocation error \n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    int index = 0;
    while (token != NULL) {
        tokens[index++] = token;

        if (index >= token_buffer_size) {
            token_buffer_size += MAX_ARGS;
            tokens = realloc(tokens, token_buffer_size * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "shimsh allocation error \n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n");
    }

    tokens[index] = NULL;
    return tokens;
}

int execute(char **args) {
    pid_t pid, wpid;
    int status;

    if (args[0] == NULL) {
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("shimsh");
        }
    } else if (pid < 0) {
        perror("shimsh");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = read_line();
        args = parse_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

int main() {
    loop();
    return 0;
}
