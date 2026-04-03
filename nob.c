#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define BUILD_FOLDER "build/"
#define MUJS_FOLDER "./mujs/"
#define RAYLIB_FOLDER "./raylib-5.5_linux_amd64/"
#define SRC_FOLDER "./src/"

int main(int argc, char **argv){
  NOB_GO_REBUILD_URSELF(argc, argv);
  Nob_Cmd cmd = {0};

  if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

  nob_cmd_append(&cmd, "cc");
  nob_cmd_append(&cmd, "-I"MUJS_FOLDER, "-I"RAYLIB_FOLDER"include/");
  nob_cmd_append(&cmd, "-o", BUILD_FOLDER"main");
  nob_cmd_append(&cmd, MUJS_FOLDER"one.c", SRC_FOLDER"main.c");
  nob_cmd_append(&cmd, "-L"RAYLIB_FOLDER"lib/");
  nob_cmd_append(&cmd, "-l:libraylib.a");
  nob_cmd_append(&cmd, "-lm");
  if (!nob_cmd_run(&cmd)) return 1;

  return 0;
}
