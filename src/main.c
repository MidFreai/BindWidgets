#include <stdio.h>
#include <mujs.h>
#include <raylib.h>

#define JSB_IMPLEMENTATION
#include "jsbindings.c"



int main(int argc, char **argv){
  js_State *J = js_newstate(NULL, NULL, 0);
  jsB_initbindings(J);

  InitWindow(800, 450, "Raylib - JS");

  if(!IsWindowReady){
    printf("[ERROR] Window not been initialized successfully\n");
    return 1;
  }

  if (argc > 1){
    js_dofile(J, argv[1]);
  }else{
    printf("[INFO] No file provided\n");
  }

  js_dostring(J, "setup();");

  while (!WindowShouldClose()){
    BeginDrawing();
    ClearBackground((Color) {r:0, g:0, b:0, a:255});

    js_dostring(J, "loop();");

    EndDrawing();
  }

  js_dostring(J, "end();");

  CloseWindow();

  js_gc(J, 0);
	js_freestate(J);
  return 0;
}
