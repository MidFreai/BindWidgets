#include <mujs.h>
#include <stdio.h>
#include <mujs.h>
#include <raylib.h>

#define JSB_IMPLEMENTATION
#include "jsbindings.c"

int main(int argc, char **argv){
  js_State *J = js_newstate(NULL, NULL, 0);
  jsB_initbindings(J);
  js_dofile(J, argv[1]);

  InitWindow(800, 450, "raylib [core] example - basic window");

  while (!WindowShouldClose()){
      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
      EndDrawing();
  }

  CloseWindow();

  js_gc(J, 0);
	js_freestate(J);
  return 0;
}
