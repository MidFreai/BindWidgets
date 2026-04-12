#ifndef JSB_H
#define JSB_H

#include <mujs.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifndef JSBDEF
#define JSBDEF static
#endif

//
JSBDEF void jsB_initbindings(js_State *J);

//raylib typecast
JSBDEF Color js_tocolor(js_State *J, int i);

//raylib bindings

//JSBDEF void jsB_InitWindow(js_State *J);
JSBDEF void jsB_SetWindowSize(js_State *J);
JSBDEF void jsB_GetScreenWidth(js_State *J);
JSBDEF void jsB_GetScreenHeight(js_State *J);
JSBDEF void jsB_SetWindowTitle(js_State *J);
JSBDEF void jsB_IsWindowFullscreen(js_State *J);

JSBDEF void jsB_ClearBackground(js_State *J);
JSBDEF void jsB_DrawText(js_State *J);

//Timing-related functions
JSBDEF void jsB_SetTargetFPS(js_State *J);
JSBDEF void jsB_GetFrameTime(js_State *J);
JSBDEF void jsB_GetTime(js_State *J);
JSBDEF void jsB_GetFps(js_State *J);

JSBDEF void jsB_IsKeyPressed(js_State *J);
JSBDEF void jsB_IsKeyDown(js_State *J);
JSBDEF void jsB_GetKeyPressed(js_State *J);
JSBDEF void jsB_IsKeyReleased(js_State *J);
JSBDEF void jsB_SetExitKey(js_State *J);

//RSHAPES
JSBDEF void jsB_DrawLine(js_State *J);
JSBDEF void jsB_DrawRectangle(js_State *J); // Draw a color-filled rectangle

static const char *window =
  "var Window = {\n"
  "size: SetWindowSize, title: SetWindowTitle, \n"
  "width: GetScreenWidth, height: GetScreenHeight \n"
  "}\n"
;

static const char *keyboardKey; //list of keys

//TODO: Custom wait screen

//if setup is not defined this be replaced
static const char *setup =
  "function setup(){\n"
  "SetWindowTitle('BindWidgets')\n"
  "}\n"
;

//if loop is not defined this be replaced
static const char *loop =
  "function loop(){\n"
  "ClearBackground({a: 255 })\n"
  "DrawText('No file provided', 275, 200, 30)\n"
  "}\n"
;

//
static const char *end =
  "function end(){}"
;

//original bindings
JSBDEF void jsB_gc(js_State *J);
JSBDEF void jsB_load(js_State *J);
JSBDEF void jsB_compile(js_State *J);
JSBDEF void jsB_print(js_State *J);
JSBDEF void jsB_write(js_State *J);
JSBDEF void jsB_read(js_State *J);
JSBDEF void jsB_readline(js_State *J);
JSBDEF void jsB_quit(js_State *J);
JSBDEF void jsB_repr(js_State *J);

static const char *require_js;

static const char *stacktrace_js;

static const char *console_js;

#endif /* !JSB_H */

#ifdef JSB_IMPLEMENTATION

JSBDEF void js_newcsetglobal(js_State *J, void *func, char *nome, int args){
  js_newcfunction(J, func, nome, args);
  js_setglobal(J, nome);
}

JSBDEF void jsB_initbindings(js_State *J){
  //raylib bindings
  //js_newcsetglobal(J, jsB_InitWindow, "InitWindow", 3);
  js_newcsetglobal(J, jsB_SetWindowSize, "SetWindowSize", 2);
  js_newcsetglobal(J, jsB_GetScreenWidth, "GetScreenWidth", 0);
  js_newcsetglobal(J, jsB_GetScreenHeight, "GetScreenHeight", 0);
  js_newcsetglobal(J, jsB_SetWindowTitle, "SetWindowTitle", 1);
  js_newcsetglobal(J, jsB_IsWindowFullscreen, "IsWindowFullscreen", 0);

  js_newcsetglobal(J, jsB_ClearBackground, "ClearBackground", 1);

  js_newcsetglobal(J, jsB_DrawText, "DrawText", 4);

  js_newcsetglobal(J, jsB_SetTargetFPS, "SetTargetFPS", 1);
  js_newcsetglobal(J, jsB_GetFrameTime, "GetFrameTime", 0);
  js_newcsetglobal(J, jsB_GetTime, "GetTime", 0);
  js_newcsetglobal(J, jsB_GetFps, "GetFPS", 0);

  js_newcsetglobal(J, jsB_IsKeyPressed, "IsKeyPressed", 1);
  js_newcsetglobal(J, jsB_IsKeyDown, "IsKeyDown", 1);
  js_newcfunction(J, jsB_IsKeyReleased, "IsKeyReleased", 1);
  js_newcsetglobal(J, jsB_GetKeyPressed, "GetKeyPressed", 0);
  js_newcsetglobal(J, jsB_SetExitKey, "SetExitKey", 1);

  js_newcsetglobal(J, jsB_DrawLine, "DrawLine", 5);
  js_newcsetglobal(J, jsB_DrawRectangle, "DrawRectangle", 5);

  js_dostring(J, setup);
  js_dostring(J, loop);
  js_dostring(J, end);
  js_dostring(J, window);
  js_dostring(J, keyboardKey);

  //original bindings
  js_newcfunction(J, jsB_gc, "gc", 0);
	js_setglobal(J, "gc");

	js_newcfunction(J, jsB_load, "load", 1);
	js_setglobal(J, "load");

	js_newcfunction(J, jsB_compile, "compile", 2);
	js_setglobal(J, "compile");

	js_newcfunction(J, jsB_print, "print", 1);
	js_setglobal(J, "print");

	js_newcfunction(J, jsB_write, "write", 0);
	js_setglobal(J, "write");

	js_newcfunction(J, jsB_read, "read", 1);
	js_setglobal(J, "read");

	js_newcfunction(J, jsB_readline, "readline", 0);
	js_setglobal(J, "readline");

	js_newcfunction(J, jsB_repr, "repr", 0);
	js_setglobal(J, "repr");

	js_newcfunction(J, jsB_quit, "quit", 1);
	js_setglobal(J, "quit");

	js_dostring(J, require_js);
	js_dostring(J, stacktrace_js);
	js_dostring(J, console_js);
}

JSBDEF Color js_tocolor(js_State *J, int i){
  Color res = {0};
  int obj = i++;
  js_getproperty(J, obj, "r");
  res.r = js_tointeger(J, i++);
  js_getproperty(J, obj, "g");
  res.g = js_tointeger(J, i++);
  js_getproperty(J, obj, "b");
  res.b = js_tointeger(J, i++);
  js_getproperty(J, obj, "a");
  res.a = js_tointeger(J, i++);

  js_pop(J, 4);
  return res;
}

JSBDEF void jsB_InitWindow(js_State *J){
  int i = 1;
  int width = js_tointeger(J, i++);
  int height = js_tointeger(J, i++);
  const char *title = js_tostring(J, i++);
  InitWindow(width, height, title);
}

JSBDEF void jsB_ClearBackground(js_State *J){
  ClearBackground(js_tocolor(J, 1));
}

JSBDEF void jsB_SetWindowSize(js_State *J){
  SetWindowSize(js_tointeger(J, 1), js_tointeger(J, 2));
}

JSBDEF void jsB_GetScreenWidth(js_State *J){
  js_pushnumber(J, GetScreenWidth());
}
JSBDEF void jsB_GetScreenHeight(js_State *J){
  js_pushnumber(J, GetScreenHeight());
}

JSBDEF void jsB_SetWindowTitle(js_State *J){
  SetWindowTitle(js_tostring(J, 1));
}

JSBDEF void jsB_IsWindowFullscreen(js_State *J){
  js_pushboolean(J, IsWindowFullscreen());
}

//Timing-related functions
JSBDEF void jsB_SetTargetFPS(js_State *J){
  SetTargetFPS(js_tointeger(J, 1));
}

JSBDEF void jsB_GetFrameTime(js_State *J){
  js_pushnumber(J, GetFrameTime());
}

JSBDEF void jsB_GetTime(js_State *J){
  js_pushnumber(J, GetTime());
}

JSBDEF void jsB_GetFps(js_State *J){
  js_pushnumber(J, GetFPS());
}

// Input-related functions: keyboard

JSBDEF void jsB_IsKeyPressed(js_State *J){
  js_pushboolean(J, IsKeyPressed(js_tointeger(J, 1)));
}

JSBDEF void jsB_GetKeyPressed(js_State *J){
  js_pushnumber(J, GetKeyPressed());
}

JSBDEF void jsB_IsKeyDown(js_State *J){
  js_pushboolean(J, IsKeyDown(js_tointeger(J, 1)));                                // Check if a key is being pressed
}

JSBDEF void jsB_IsKeyReleased(js_State *J){
  js_pushboolean(J, IsKeyReleased(js_tointeger(J, 1)));                            // Check if a key has been released once
}

JSBDEF void jsB_SetExitKey(js_State *J){
  SetExitKey(js_tointeger(J, 1));                               // Set a custom key to exit program (default is ESC)
}


    //bool IsKeyPressedRepeat(int key);                       // Check if a key has been pressed again
    //bool IsKeyUp(int key);                                  // Check if a key is NOT being pressed
    //int GetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty

JSBDEF void jsB_DrawLine(js_State *J){
  int i = 1;
  Vector2 start = {x: js_tointeger(J, i++), y: js_tointeger(J, i++)};
  Vector2 end = {x: js_tointeger(J, i++), y: js_tointeger(J, i++)};
  Color color = js_tocolor(J, i++);

  DrawLine(start.x, start.y, end.x, end.y, color);                // Draw a line
}

JSBDEF void jsB_DrawRectangle(js_State *J){
  int i = 1;
  Vector2 pos = {x: js_tointeger(J, i++), y: js_tointeger(J, i++)};
  int width = js_tointeger(J, i++);
  int height = js_tointeger(J, i++);
  Color color = js_tocolor(J, i++);
  DrawRectangle(pos.x, pos.y, width, height, color);                        // Draw a color-filled rectangle
}

JSBDEF void jsB_DrawText(js_State *J){
  int i = 1;
  const char *text = js_tostring(J, i++);
  int posX = js_tointeger(J, i++);
  int posY = js_tointeger(J, i++);
  int fontSize = js_tointeger(J, i++);
  DrawText(text, posX, posY, fontSize, LIGHTGRAY);
}

static const char *keyboardKey =
  "var Keyboard = {\n"
  "KEY_NULL            : 0, \n"       // Key: NULL, used for no key pressed
  "KEY_APOSTROPHE      : 39,\n"       // Key: '
  "KEY_COMMA           : 44,\n"       // Key: ,
  "KEY_MINUS           : 45,\n"       // Key: -
  "KEY_PERIOD          : 46,\n"       // Key: .
  "KEY_SLASH           : 47,\n"       // Key: /
  "KEY_ZERO            : 48,\n"       // Key: 0
  "KEY_ONE             : 49,\n"       // Key: 1
  "KEY_TWO             : 50,\n"       // Key: 2
  "KEY_THREE           : 51,\n"       // Key: 3
  "KEY_FOUR            : 52,\n"       // Key: 4
  "KEY_FIVE            : 53,\n"       // Key: 5
  "KEY_SIX             : 54,\n"       // Key: 6
  "KEY_SEVEN           : 55,\n"       // Key: 7
  "KEY_EIGHT           : 56,\n"       // Key: 8
  "KEY_NINE            : 57,\n"       // Key: 9
  "KEY_SEMICOLON       : 59,\n"       // Key: ;
  "KEY_EQUAL           : 61,\n"       // Key: =
  "KEY_A               : 65,\n"       // Key: A | a
  "KEY_B               : 66,\n"       // Key: B | b
  "KEY_C               : 67,\n"       // Key: C | c
  "KEY_D               : 68,\n"       // Key: D | d
  "KEY_E               : 69,\n"       // Key: E | e
  "KEY_F               : 70,\n"       // Key: F | f
  "KEY_G               : 71,\n"       // Key: G | g
  "KEY_H               : 72,\n"       // Key: H | h
  "KEY_I               : 73,\n"       // Key: I | i
  "KEY_J               : 74,\n"       // Key: J | j
  "KEY_K               : 75,\n"       // Key: K | k
  "KEY_L               : 76,\n"       // Key: L | l
  "KEY_M               : 77,\n"       // Key: M | m
  "KEY_N               : 78,\n"       // Key: N | n
  "KEY_O               : 79,\n"       // Key: O | o
  "KEY_P               : 80,\n"       // Key: P | p
  "KEY_Q               : 81,\n"       // Key: Q | q
  "KEY_R               : 82,\n"       // Key: R | r
  "KEY_S               : 83,\n"       // Key: S | s
  "KEY_T               : 84,\n"       // Key: T | t
  "KEY_U               : 85,\n"       // Key: U | u
  "KEY_V               : 86,\n"       // Key: V | v
  "KEY_W               : 87,\n"       // Key: W | w
  "KEY_X               : 88,\n"       // Key: X | x
  "KEY_Y               : 89,\n"       // Key: Y | y
  "KEY_Z               : 90,\n"       // Key: Z | z
  "KEY_LEFT_BRACKET    : 91,\n"       // Key: [
  "KEY_BACKSLASH       : 92,\n"       // Key: '\'
  "KEY_RIGHT_BRACKET   : 93,\n"       // Key: ]
  "KEY_GRAVE           : 96,\n"       // Key: `
  "KEY_SPACE           : 32,\n"       // Key: Space
  "KEY_ESCAPE          : 256,\n"      // Key: Esc
  "KEY_ENTER           : 257,\n"      // Key: Enter
  "KEY_TAB             : 258,\n"      // Key: Tab
  "KEY_BACKSPACE       : 259,\n"      // Key: Backspace
  "KEY_INSERT          : 260,\n"      // Key: Ins
  "KEY_DELETE          : 261,\n"      // Key: Del
  "KEY_RIGHT           : 262,\n"      // Key: Cursor right
  "KEY_LEFT            : 263,\n"      // Key: Cursor left
  "KEY_DOWN            : 264,\n"      // Key: Cursor down
  "KEY_UP              : 265,\n"      // Key: Cursor up
  "KEY_PAGE_UP         : 266,\n"      // Key: Page up
  "KEY_PAGE_DOWN       : 267,\n"      // Key: Page down
  "KEY_HOME            : 268,\n"      // Key: Home
  "KEY_END             : 269,\n"      // Key: End
  "KEY_CAPS_LOCK       : 280,\n"      // Key: Caps lock
  "KEY_SCROLL_LOCK     : 281,\n"      // Key: Scroll down
  "KEY_NUM_LOCK        : 282,\n"      // Key: Num lock
  "KEY_PRINT_SCREEN    : 283,\n"      // Key: Print screen
  "KEY_PAUSE           : 284,\n"      // Key: Pause
  "KEY_F1              : 290,\n"      // Key: F1
  "KEY_F2              : 291,\n"      // Key: F2
  "KEY_F3              : 292,\n"      // Key: F3
  "KEY_F4              : 293,\n"      // Key: F4
  "KEY_F5              : 294,\n"      // Key: F5
  "KEY_F6              : 295,\n"      // Key: F6
  "KEY_F7              : 296,\n"      // Key: F7
  "KEY_F8              : 297,\n"      // Key: F8
  "KEY_F9              : 298,\n"      // Key: F9
  "KEY_F10             : 299,\n"      // Key: F10
  "KEY_F11             : 300,\n"      // Key: F11
  "KEY_F12             : 301,\n"      // Key: F12
  "KEY_LEFT_SHIFT      : 340,\n"      // Key: Shift left
  "KEY_LEFT_CONTROL    : 341,\n"      // Key: Control left
  "KEY_LEFT_ALT        : 342,\n"      // Key: Alt left
  "KEY_LEFT_SUPER      : 343,\n"      // Key: Super left
  "KEY_RIGHT_SHIFT     : 344,\n"      // Key: Shift right
  "KEY_RIGHT_CONTROL   : 345,\n"      // Key: Control right
  "KEY_RIGHT_ALT       : 346,\n"      // Key: Alt right
  "KEY_RIGHT_SUPER     : 347,\n"      // Key: Super right
  "KEY_KB_MENU         : 348,\n"      // Key: KB menu
  "KEY_KP_0            : 320,\n"      // Key: Keypad 0
  "KEY_KP_1            : 321,\n"      // Key: Keypad 1
  "KEY_KP_2            : 322,\n"      // Key: Keypad 2
  "KEY_KP_3            : 323,\n"      // Key: Keypad 3
  "KEY_KP_4            : 324,\n"      // Key: Keypad 4
  "KEY_KP_5            : 325,\n"      // Key: Keypad 5
  "KEY_KP_6            : 326,\n"      // Key: Keypad 6
  "KEY_KP_7            : 327,\n"      // Key: Keypad 7
  "KEY_KP_8            : 328,\n"      // Key: Keypad 8
  "KEY_KP_9            : 329,\n"      // Key: Keypad 9
  "KEY_KP_DECIMAL      : 330,\n"      // Key: Keypad .
  "KEY_KP_DIVIDE       : 331,\n"      // Key: Keypad /
  "KEY_KP_MULTIPLY     : 332,\n"      // Key: Keypad *
  "KEY_KP_SUBTRACT     : 333,\n"      // Key: Keypad -
  "KEY_KP_ADD          : 334,\n"      // Key: Keypad +
  "KEY_KP_ENTER        : 335,\n"      // Key: Keypad Enter
  "KEY_KP_EQUAL        : 336,\n"      // Key: Keypad =
  "}\n"
;

//original bindings
static const char *require_js =
	"function require(name) {\n"
	"var cache = require.cache;\n"
	"if (name in cache) return cache[name];\n"
	"var exports = {};\n"
	"cache[name] = exports;\n"
	"Function('exports', read(name+'.js'))(exports);\n"
	"return exports;\n"
	"}\n"
	"require.cache = Object.create(null);\n"
;

static const char *stacktrace_js =
	"Error.prototype.toString = function() { return this.stack }\n"
;

static const char *console_js =
	"var console = { log: print, debug: print, warn: print, error: print };"
;

JSBDEF void jsB_gc(js_State *J){
	int report = js_toboolean(J, 1);
	js_gc(J, report);
	js_pushundefined(J);
}

JSBDEF void jsB_load(js_State *J){
	int i, n = js_gettop(J);
	for (i = 1; i < n; ++i) {
		js_loadfile(J, js_tostring(J, i));
		js_pushundefined(J);
		js_call(J, 0);
		js_pop(J, 1);
	}
	js_pushundefined(J);
}

JSBDEF void jsB_compile(js_State *J){
	const char *source = js_tostring(J, 1);
	const char *filename = js_isdefined(J, 2) ? js_tostring(J, 2) : "[string]";
	js_loadstring(J, filename, source);
}

JSBDEF void jsB_print(js_State *J){
	int i, top = js_gettop(J);
	for (i = 1; i < top; ++i) {
		const char *s = js_tostring(J, i);
		if (i > 1) putchar(' ');
		fputs(s, stdout);
	}
	putchar('\n');
	js_pushundefined(J);
}

JSBDEF void jsB_write(js_State *J){
	int i, top = js_gettop(J);
	for (i = 1; i < top; ++i) {
		const char *s = js_tostring(J, i);
		if (i > 1) putchar(' ');
		fputs(s, stdout);
	}
	js_pushundefined(J);
}

JSBDEF void jsB_read(js_State *J){
	const char *filename = js_tostring(J, 1);
	FILE *f;
	char *s;
	int n, t;

	f = fopen(filename, "rb");
	if (!f) {
		js_error(J, "cannot open file '%s': %s", filename, strerror(errno));
	}

	if (fseek(f, 0, SEEK_END) < 0) {
		fclose(f);
		js_error(J, "cannot seek in file '%s': %s", filename, strerror(errno));
	}

	n = ftell(f);
	if (n < 0) {
		fclose(f);
		js_error(J, "cannot tell in file '%s': %s", filename, strerror(errno));
	}

	if (fseek(f, 0, SEEK_SET) < 0) {
		fclose(f);
		js_error(J, "cannot seek in file '%s': %s", filename, strerror(errno));
	}

	s = malloc(n + 1);
	if (!s) {
		fclose(f);
		js_error(J, "out of memory");
	}

	t = fread(s, 1, n, f);
	if (t != n) {
		free(s);
		fclose(f);
		js_error(J, "cannot read data from file '%s': %s", filename, strerror(errno));
	}
	s[n] = 0;

	js_pushstring(J, s);
	free(s);
	fclose(f);
}

char *__readline(const char *prompt){
	static char line[500], *p;
	int n;
	fputs(prompt, stdout);
	p = fgets(line, sizeof line, stdin);
	if (p) {
		n = strlen(line);
		if (n > 0 && line[n-1] == '\n')
			line[--n] = 0;
		p = malloc(n+1);
		memcpy(p, line, n+1);
		return p;
	}
	return NULL;
}

JSBDEF void jsB_readline(js_State *J){
	char *line = __readline("");
	if (!line) {
		js_pushnull(J);
		return;
	}
	js_pushstring(J, line);
	free(line);
}

JSBDEF void jsB_quit(js_State *J){
	exit(js_tonumber(J, 1));
}

JSBDEF void jsB_repr(js_State *J){
	js_repr(J, 1);
}

#endif
