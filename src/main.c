#include <mujs.h>
#include <stdio.h>
#include <mujs.h>
#define JSB_IMPLEMENTATION
#include "jsbindings.c"

int main(int argc, char **argv){
  js_State *J = js_newstate(NULL, NULL, 0);
  jsB_initbindings(J);
  js_dofile(J, argv[1]);

  js_gc(J, 0);
	js_freestate(J);
  return 0;
}
