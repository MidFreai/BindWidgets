#include <mujs.h>
#include <stdio.h>
#include <mujs.h>
#include <raylib.h>

#define JSB_IMPLEMENTATION
#include "jsbindings.c"

void jsB_SetWindowSize(js_State *J){
  SetWindowSize(js_tointeger(J, 1), js_tointeger(J, 2));
}

void jsB_DrawText(js_State *J){
  DrawText(js_tostring(J, 1), 190, 200, 20, LIGHTGRAY);
}

int main(int argc, char **argv){
  js_State *J = js_newstate(NULL, NULL, 0);
  jsB_initbindings(J);

  js_newcfunction(J, jsB_SetWindowSize, "SetWindowSize", 2);
  js_setglobal(J, "SetWindowSize");
  js_newcfunction(J, jsB_DrawText, "DrawText", 1);
  js_setglobal(J, "DrawText");


  InitWindow(800, 450, "raylib [core]");

  if (argc > 1){
    js_dofile(J, argv[1]);
  }

  while (!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    js_dostring(J, "loop()");
    //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  js_gc(J, 0);
	js_freestate(J);
  return 0;
}
