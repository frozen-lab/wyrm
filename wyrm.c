// gcc -std=c99 -Wall -Wextra -o wyrm wyrm.c

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void cpuid(uint32_t regs[4], uint32_t leaf, uint32_t sub) {
  __asm__ volatile("cpuid"
                   : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
                   : "a"(leaf), "c"(sub));
}

static inline uint64_t xgetbv(uint32_t index) {
  uint32_t eax, edx;
  __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
  return ((uint64_t)edx << 32) | eax;
}

bool is_avx(void) {
  uint32_t regs[4];

  // get basic CPU features
  cpuid(regs, 1, 0);

  int hasAVX = (regs[2] & (1 << 28)) != 0;   // CPUID.1: ECX bit 28 = AVX
  int hasXSAVE = (regs[2] & (1 << 26)) != 0; // ECX bit 26 = XSAVE/XGETBV
  int avxOsEnabled = 0;

  if (hasXSAVE) {
    uint64_t xcr0 = xgetbv(0);
    avxOsEnabled = ((xcr0 & 0x6) == 0x6); // XMM (bit 1) + YMM (bit 2)
  }

  return (hasAVX && avxOsEnabled);
}

bool is_avx2(void) {
  uint32_t regs[4];

  // get basic CPU features
  cpuid(regs, 1, 0);

  if (is_avx()) {
    // Extended features in CPUID leaf 7, subleaf 0
    cpuid(regs, 7, 0);

    // EBX bit 5 = AVX2
    return (regs[1] & (1 << 5)) != 0;
  }

  return false;
}

void print_usage() {
  printf("Usage: wyrm -l <path>\n\n"
         "Options:\n"
         "  -l <path>   Print specified directory structure\n"
         "  -h          Show this help message\n");
}

void print_dir_struct(const char *dir_path) {
  printf("[Path]: %s\n", dir_path);

  printf("AVX: %s\n", is_avx() ? "yes" : "no");
  printf("AVX2: %s\n", is_avx2() ? "yes" : "no");
}

int main(int argc, char *argv[]) {
  char *dir_path = NULL;

  if (argc < 2) {
    fprintf(stderr, "[Err] Missing arguments\n"
                    "Use `-h` for help\n");

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
  } else if (strcmp(argv[1], "-h") == 0) {
    print_usage();

    return EXIT_SUCCESS;
  } else {
    fprintf(stderr,
            "[Err] Unknown argument: %s\n"
            "Use `-h` for help\n",
            argv[1]);

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
