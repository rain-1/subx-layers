
// Includes
#include <assert.h>
#include <stdlib.h>

#include<iostream>
using std::cerr;

#include <map>
using std::map;
#define SIZE(X) (assert((X).size() < (1LL<<(sizeof(int)*8-2))), static_cast<int>((X).size()))

#include <signal.h>

#include <fenv.h>
#include <assert.h>

#include <iostream>
using std::istream;
using std::ostream;
using std::iostream;
using std::cin;
using std::cout;
using std::cerr;
#include <iomanip>

#include <string.h>
#include <string>
using std::string;

#include <algorithm>
using std::min;
using std::max;

// End Includes

// Types
// End Types

// Function prototypes are auto-generated in the 'build' script; define your
// functions in any order. Just be sure to declare each function header all on
// one line, ending with the '{'. Our auto-generation scripts are too minimal
// and simple-minded to handle anything else.
#include "function_list"  // by convention, files ending with '_list' are auto-generated

// from http://stackoverflow.com/questions/152643/idiomatic-c-for-reading-from-a-const-map
template<typename T> typename T::mapped_type& get(T& map, typename T::key_type const& key) {
  typename T::iterator iter(map.find(key));
  if (iter == map.end()) {
    cerr << "get couldn't find key '" << key << "'\n";
    assert(iter != map.end());
  }
  return iter->second;
}
template<typename T> typename T::mapped_type const& get(const T& map, typename T::key_type const& key) {
  typename T::const_iterator iter(map.find(key));
  if (iter == map.end()) {
    cerr << "get couldn't find key '" << key << "'\n";
    assert(iter != map.end());
  }
  return iter->second;
}
template<typename T> typename T::mapped_type const& put(T& map, typename T::key_type const& key, typename T::mapped_type const& value) {
  map[key] = value;
  return map[key];
}
template<typename T> bool contains_key(T& map, typename T::key_type const& key) {
  return map.find(key) != map.end();
}
template<typename T> typename T::mapped_type& get_or_insert(T& map, typename T::key_type const& key) {
  return map[key];
}
template<typename T> typename T::mapped_type const& put_new(T& map, typename T::key_type const& key, typename T::mapped_type const& value) {
  assert(map.find(key) == map.end());
  map[key] = value;
  return map[key];
}

// Globals
//
// All statements in this section should always define a single variable on a
// single line. The 'build' script will simple-mindedly auto-generate extern
// declarations for them. Remember to define (not just declare) constants with
// extern linkage in this section, since C++ global constants have internal
// linkage by default.
//
map<string, string> Help;
// End Globals

int main(int argc, char* argv[]) {
  initialize_signal_handlers();  // not always necessary, but doesn't hurt
  //? cerr << INT_MAX+1 << '\n';  // test overflow
  //? assert(false);  // test SIGABRT
  feenableexcept(FE_OVERFLOW | FE_UNDERFLOW);
  //? assert(sizeof(int) == 4 && sizeof(float) == 4);
  //? //                          | exp   |  mantissa
  //? int smallest_subnormal = 0b00000000000000000000000000000001;
  //? float smallest_subnormal_f = *reinterpret_cast<float*>(&smallest_subnormal);
  //? cerr << "ε: " << smallest_subnormal_f << '\n';
  //? cerr << "ε/2: " << smallest_subnormal_f/2 << " (underflow)\n";  // test SIGFPE
  atexit(reset);
  // we require a 32-bit little-endian system
  assert(sizeof(int) == 4);
  assert(sizeof(float) == 4);
  assert_little_endian();

  init_help();
  // End One-time Setup

  // Commandline Parsing
  if (argc <= 1 || is_equal(argv[1], "--help")) {
    // currently no automated tests for commandline arg parsing
    cerr << get(Help, "usage");
    return 0;
  }

  char** arg = &argv[1];
  while (argc > 1 && starts_with(*arg, "--")) {
    if (false)
      ;  // no-op branch just so any further additions can consistently always start with 'else'
    // End Commandline Options(*arg)
    else
      cerr << "skipping unknown option " << *arg << '\n';
    --argc;  ++argv;  ++arg;
  }

  if (is_equal(argv[1], "help")) {
    if (argc == 2) {
      cerr << "help on what?\n";
      help_contents();
      return 0;
    }
    string key(argv[2]);
    // End Help Special-cases(key)
    if (contains_key(Help, key)) {
      cerr << get(Help, key);
      return 0;
    }
    else {
      cerr << "No help found for '" << key << "'\n";
      help_contents();
      cerr << "Please check your command for typos.\n";
      return 1;
    }
  }

  // End Commandline Parsing

  // End Main

  return 0;
}

// Unit Tests
// End Unit Tests

void reset() {
  // End Reset
}

void assert_little_endian() {
  const int x = 1;
  const char* y = reinterpret_cast<const char*>(&x);
  if (*y != 1) {
    cerr << "SubX requires a little-endian processor. Do you have Intel (or AMD or Atom) inside?\n";
    exit(1);
  }
}

void help_contents() {
  cerr << "Available top-level topics:\n";
  cerr << "  usage\n";
  // End Help Contents
}

void init_help() {
  put(Help, "usage",
    "Welcome to SubX, a better way to program in machine code.\n"
    "SubX uses a subset of the x86 instruction set. SubX programs will run\n"
    "without modification on Linux computers.\n"
    "It provides a better experience and better error messages than\n"
    "programming directly in machine code, but you have to stick to the\n"
    "instructions it supports.\n"
    "\n"
    "== Ways to invoke subx\n"
    "- Run tests:\n"
    "    subx test\n"
    "- See this message:\n"
    "    subx --help\n"
    "- Convert a textual SubX program into a standard ELF binary that you can\n"
    "  run on your computer:\n"
    "    subx translate input1.subx intput2.subx ... -o <output ELF binary>\n"
    "- Run a SubX binary using SubX itself (for better error messages):\n"
    "    subx run <ELF binary>\n"
    "\n"
    "== Debugging aids\n"
    "- Add '--trace' to any of these commands to print a trace to stderr\n"
    "  for debugging purposes.\n"
    "- Add '--map' to add information to traces. 'subx --map translate' will save\n"
    "  (to a file called 'map') the mapping from labels to addresses that it computes\n"
    "  during translation. This file is then available to 'subx --map --trace run'\n"
    "  which prints out label names in the trace as it encounters them.\n"
    "\n"
    "Options starting with '--' must always come before any other arguments.\n"
    "\n"
    "To start learning how to write SubX programs, see Readme.md (particularly\n"
    "the section on the x86 instruction set) and then run:\n"
    "  subx help\n"
  );
  // End Help Texts
}

bool is_equal(char* s, const char* lit) {
  return strncmp(s, lit, strlen(lit)) == 0;
}

bool starts_with(const string& s, const string& pat) {
  string::const_iterator a=s.begin(), b=pat.begin();
  for (/*nada*/;  a!=s.end() && b!=pat.end();  ++a, ++b)
    if (*a != *b) return false;
  return b == pat.end();
}

// based on https://spin.atomicobject.com/2013/01/13/exceptions-stack-traces-c
void initialize_signal_handlers() {
  struct sigaction action;
  bzero(&action, sizeof(action));
  action.sa_sigaction = dump_and_exit;
  sigemptyset(&action.sa_mask);
  sigaction(SIGABRT, &action, NULL);  // assert() failure or integer overflow on linux (with -ftrapv)
  sigaction(SIGILL,  &action, NULL);  // integer overflow on OS X (with -ftrapv)
}
void dump_and_exit(int sig, siginfo_t* /*unused*/, void* /*unused*/) {
  switch (sig) {
    case SIGABRT:
      #ifndef __APPLE__
        cerr << "SIGABRT: might be an integer overflow if it wasn't an assert() failure\n";
        _Exit(1);
      #endif
      break;
    case SIGILL:
      #ifdef __APPLE__
        cerr << "SIGILL: most likely caused by integer overflow\n";
        _Exit(1);
      #endif
      break;
    default:
      break;
  }
}
#ifdef __APPLE__
// Public domain polyfill for feenableexcept on OS X
// http://www-personal.umich.edu/~williams/archive/computation/fe-handling-example.c
int feenableexcept(unsigned int excepts) {
  static fenv_t fenv;
  unsigned int new_excepts = excepts & FE_ALL_EXCEPT;
  unsigned int old_excepts;
  if (fegetenv(&fenv)) return -1;
  old_excepts = fenv.__control & FE_ALL_EXCEPT;
  fenv.__control &= ~new_excepts;
  fenv.__mxcsr &= ~(new_excepts << 7);
  return fesetenv(&fenv) ? -1 : old_excepts;
}
#endif

bool has_data(istream& in) {
  return in && !in.eof();
}

