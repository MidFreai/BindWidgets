#ifndef JSB_H
#define JSB_H

#include <mujs.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifndef JSBDEF
#define JSBDEF static inline
#endif

//raylib typecast
JSBDEF Color js_tocolor(js_State *J, int i);

//raylib bindings
JSBDEF void jsB_InitWindow(js_State *J);
JSBDEF void jsB_ClearBackground(js_State *J);
JSBDEF void jsB_DrawText(js_State *J);
JSBDEF void jsB_SetWindowSize(js_State *J);
JSBDEF void jsB_SetWindowTitle(js_State *J);
JSBDEF void jsB_SetTargetFPS(js_State *J);

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
  "ClearBackground({r: 0, g: 0, b: 0, a: 255 })\n"
  "DrawText('No file', 200, 100, 30)\n"
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
JSBDEF int eval_print(js_State *J, const char *source);

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

#endif /* !JSB_H */

#ifdef JSB_IMPLEMENTATION

JSBDEF void jsB_initbindings(js_State *J){
  //raylib bindings
  js_newcfunction(J, jsB_InitWindow, "InitWindow", 3);
  js_setglobal(J, "InitWindow");

  js_newcfunction(J, jsB_ClearBackground, "ClearBackground", 1);
  js_setglobal(J, "ClearBackground");

  js_newcfunction(J, jsB_SetWindowSize, "SetWindowSize", 2);
  js_setglobal(J, "SetWindowSize");

  js_newcfunction(J, jsB_SetWindowTitle, "SetWindowTitle", 1);
  js_setglobal(J, "SetWindowTitle");

  js_newcfunction(J, jsB_SetTargetFPS, "SetTargetFPS", 1);
  js_setglobal(J, "SetTargetFPS");

  js_newcfunction(J, jsB_DrawText, "DrawText", 4);
  js_setglobal(J, "DrawText");

  js_dostring(J, setup);
  js_dostring(J, loop);
  js_dostring(J, end);

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

JSBDEF void jsB_SetWindowTitle(js_State *J){
  SetWindowTitle(js_tostring(J, 1));
}

JSBDEF void jsB_SetTargetFPS(js_State *J){
  SetTargetFPS(js_tointeger(J, 1));
}

JSBDEF void jsB_DrawText(js_State *J){
  int i = 1;
  const char *text = js_tostring(J, i++);
  int posX = js_tointeger(J, i++);
  int posY = js_tointeger(J, i++);
  int fontSize = js_tointeger(J, i++);
  DrawText(text, posX, posY, fontSize, LIGHTGRAY);
}

//original bindings
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

JSBDEF int eval_print(js_State *J, const char *source) {
	if (js_ploadstring(J, "[stdin]", source)) {
		fprintf(stderr, "%s\n", js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return 1;
	}
	js_pushundefined(J);
	if (js_pcall(J, 0)) {
		fprintf(stderr, "%s\n", js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return 1;
	}
	if (js_isdefined(J, -1)) {
		printf("%s\n", js_tryrepr(J, -1, "can't convert to string"));
	}
	js_pop(J, 1);
	return 0;
}
#endif
