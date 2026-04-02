#ifndef JSB_H
#define JSB_H

#include <mujs.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifndef JSBDEF
#define JSBDEF static inline
#endif


//original fn
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
