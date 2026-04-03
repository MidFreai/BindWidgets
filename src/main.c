#include <mujs.h>
#include <stdio.h>
#include <mujs.h>
#include <raylib.h>

#define JSB_IMPLEMENTATION
#include "jsbindings.c"



int main(int argc, char **argv){
  js_State *J = js_newstate(NULL, NULL, 0);
  jsB_initbindings(J);

  InitWindow(800, 450, "Raylib - JS");

  if (argc > 1){
    js_dofile(J, argv[1]);
  }

  js_dostring(J, "setup();");

  while (!WindowShouldClose()){
    BeginDrawing();

    js_dostring(J, "loop();");

    EndDrawing();
  }

  js_dostring(J, "end();");

  CloseWindow();

  js_gc(J, 0);
	js_freestate(J);
  return 0;
}
