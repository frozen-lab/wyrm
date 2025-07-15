#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
    printf(
        "Usage: wyrm -l <path>\n\n"
        "Options:\n"
        "  -l <path>   Print specified directory structure\n"
        "  -h          Show this help message\n"   
    );
}

void print_dir_struct(const char *dir_path) {
    printf("[Path]: %s\n", dir_path);
}

int main(int argc, char *argv[]) {
    char *dir_path = NULL;

    if (argc < 2) {
        fprintf(
           stderr,
          "[Err] Missing arguments\n"
          "Use `-h` for help\n"
        );
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-l") == 0) {
        if (argc < 3) {
            fprintf(stderr, "[Err] Missing path argument for -l\n");
            return EXIT_FAILURE;
        }

        dir_path = argv[2];
        print_dir_struct(dir_path);
        return EXIT_SUCCESS;
    }
    else if (strcmp(argv[1], "-h") == 0) {
        print_usage();
        return EXIT_SUCCESS;
    }
    else {
        fprintf(
          stderr,
          "[Err] Unknown argument: %s\n"
          "Use `-h` for help\n",
          argv[1]
        );
        return EXIT_FAILURE;
    }
}

