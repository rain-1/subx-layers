
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

#define CHECK(X) \
  if (Passed && !(X)) { \
    cerr << "\nF - " << __FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << "): " << #X << '\n'; \
    Passed = false; \
    return;  /* Currently we stop at the very first failure. */ \
  }

#define CHECK_EQ(X, Y) \
  if (Passed && (X) != (Y)) { \
    cerr << "\nF - " << __FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << "): " << #X << " == " << #Y << '\n'; \
    cerr << "  got " << (X) << '\n';  /* BEWARE: multiple eval */ \
    Passed = false; \
    return;  /* Currently we stop at the very first failure. */ \
  }

#include <stdlib.h>

// No brackets around the expansion so that it prints nothing if Trace_stream
// isn't initialized.
#define trace(...)  !Trace_stream ? cerr : Trace_stream->stream(__VA_ARGS__)

#define START_TRACING_UNTIL_END_OF_SCOPE  lease_tracer leased_tracer;
#define raise  (!Trace_stream ? (++Trace_errors,cerr) /*do print*/ : Trace_stream->stream(Error_depth, "error"))
#define warn (!Trace_stream ? (++Trace_errors,cerr) /*do print*/ : Trace_stream->stream(Warn_depth, "warn"))

// If we aren't yet sure how to deal with some corner case, use assert_for_now
// to indicate that it isn't an inviolable invariant.
#define assert_for_now assert
#define raise_for_now raise


#define CHECK_TRACE_CONTENTS(...)  check_trace_contents(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)

#define CHECK_TRACE_DOESNT_CONTAIN(...)  CHECK(trace_doesnt_contain(__VA_ARGS__))

#define CHECK_TRACE_COUNT(label, count) \
  if (Passed && trace_count(label) != (count)) { \
    cerr << "\nF - " << __FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << "): trace_count of " << label << " should be " << count << '\n'; \
    cerr << "  got " << trace_count(label) << '\n';  /* multiple eval */ \
    DUMP(label); \
    Passed = false; \
    return;  /* Currently we stop at the very first failure. */ \
  }

#define CHECK_TRACE_CONTAINS_ERRORS()  CHECK(trace_contains_errors())
#define CHECK_TRACE_DOESNT_CONTAIN_ERRORS() \
  if (Passed && trace_contains_errors()) { \
    cerr << "\nF - " << __FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << "): unexpected errors\n"; \
    DUMP("error"); \
    Passed = false; \
    return; \
  }

// Allow tests to ignore trace lines generated during setup.
#define CLEAR_TRACE  delete Trace_stream, Trace_stream = new trace_stream

// To debug why a test is failing, dump its trace using '?'.
#define DUMP(label)  if (Trace_stream) cerr << Trace_stream->readable_contents(label);

// To add temporary prints to the trace, use 'dbg'.
// `git log` should never show any calls to 'dbg'.
#define dbg trace(0, "a")


#include <vector>
using std::vector;
#include <list>
using std::list;
#include <set>
using std::set;

#include <sstream>
using std::istringstream;
using std::ostringstream;

#include <fstream>
using std::ifstream;
using std::ofstream;

// End Includes

// Types
typedef void (*test_fn)(void);
const int Max_depth = 9999;
struct trace_line {
  string contents;
  string label;
  int depth;  // 0 is 'sea level'; positive integers are progressively 'deeper' and lower level
  trace_line(string c, string l) {
    contents = c;
    label = l;
    depth = 0;
  }
  trace_line(string c, string l, int d) {
    contents = c;
    label = l;
    depth = d;
  }
};

const int Error_depth = 0;
const int Warn_depth = 1;
struct trace_stream {
  vector<trace_line> past_lines;
  // accumulator for current trace_line
  ostringstream* curr_stream;
  string curr_label;
  int curr_depth;
  // other stuff
  int collect_depth;  // avoid tracing lower levels for speed
  ofstream null_stream;  // never opened, so writes to it silently fail

  // End trace_stream Fields

  trace_stream() {
    curr_stream = NULL;
    curr_depth = Max_depth;
    collect_depth = Max_depth;

    // End trace_stream Constructor
  }
  ~trace_stream() {
    // End trace_stream Destructor
  }
  ostream& stream(string label) {
    return stream(Max_depth, label);
  }

  ostream& stream(int depth, string label) {
    if (depth > collect_depth) return null_stream;
    curr_stream = new ostringstream;
    curr_label = label;
    curr_depth = depth;
    (*curr_stream) << std::hex;  // printing addresses is the common case
    return *curr_stream;
  }

  void newline();
  bool should_incrementally_print_trace();

  string readable_contents(string label) {
    string trim(const string& s);  // prototype
    ostringstream output;
    label = trim(label);
    for (vector<trace_line>::iterator p = past_lines.begin();  p != past_lines.end();  ++p)
      if (label.empty() || label == p->label)
        output << std::setw(4) << p->depth << ' ' << p->label << ": " << p->contents << '\n';
    return output.str();
  }

  // End trace_stream Methods
};


struct end {};
struct lease_tracer {
  lease_tracer();
  ~lease_tracer();
};
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

// move a global ahead into types that we can't generate an extern declaration for
const test_fn Tests[] = {
  #include "test_list"  // auto-generated; see 'build*' scripts
};

// Names for each element of the 'Tests' global, respectively.
const string Test_names[] = {
  #include "test_name_list"  // auto-generated; see 'build*' scripts
};
// Globals
//
// All statements in this section should always define a single variable on a
// single line. The 'build' script will simple-mindedly auto-generate extern
// declarations for them. Remember to define (not just declare) constants with
// extern linkage in this section, since C++ global constants have internal
// linkage by default.
//
map<string, string> Help;
bool Run_tests = false;
bool Passed = true;  // set this to false inside any test to indicate failure

trace_stream* Trace_stream = NULL;

int Hide_errors = false;  // if set, don't print errors or warnings to screen
int Hide_warnings = false;  // if set, don't print warnings to screen
int Trace_errors = 0;  // used only when Trace_stream is NULL

// Fail tests that displayed (unexpected) errors.
// Expected errors should always be hidden and silently checked for.
ofstream Trace_file;
bool Dump_trace = false;
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
  atexit(cleanup_main);
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
    else if (is_equal(*arg, "--trace")) {
      cerr << "saving trace to 'last_run'\n";
      Trace_file.open("last_run");
      // Add a dummy line up top; otherwise the `browse_trace` tool currently has
      // no way to expand any lines above an error.
      Trace_file << "   0 dummy: start\n";
    }
    else if (is_equal(*arg, "--dump")) {
      Dump_trace = true;
    }
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

  if (argc > 1 && is_equal(argv[1], "test")) {
    Run_tests = true;  --argc;  ++argv;  // shift 'test' out of commandline args
  }

  // End Commandline Parsing

  if (Run_tests) {
    // Test Runs
    string maybe_single_test_to_run = argv[argc-1];
    if (!starts_with(maybe_single_test_to_run, "test_"))
      maybe_single_test_to_run.insert(0, "test_");
    for (size_t i=0;  i < sizeof(Tests)/sizeof(Tests[0]);  ++i) {
      if (Test_names[i] == maybe_single_test_to_run) {
        run_test(i);
        if (Passed) cerr << ".\n";
        return 0;
      }
    }

    // we run some tests and then exit; assume no state need be maintained afterward

    long num_failures = 0;
    // End Test Run Initialization
    time_t t;  time(&t);
    cerr << "C tests: " << ctime(&t);
    for (size_t i=0;  i < sizeof(Tests)/sizeof(Tests[0]);  ++i) {
  //?     cerr << "running " << Test_names[i] << '\n';
      run_test(i);
      if (Passed) cerr << '.';
      else ++num_failures;
    }
    cerr << '\n';
    // End Tests
    if (num_failures > 0) {
      cerr << num_failures << " failure"
           << (num_failures > 1 ? "s" : "")
           << '\n';
      return 1;
    }
    return 0;
  }

  // End Main
  cerr << "nothing to do\n";
  return 1;


  return 0;
}

// Unit Tests
// End Unit Tests

void reset() {
  Passed = true;

  Hide_errors = false;
  Hide_warnings = false;
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


void run_test(size_t i) {
  if (i >= sizeof(Tests)/sizeof(Tests[0])) {
    cerr << "no test " << i << '\n';
    return;
  }
  reset();
  START_TRACING_UNTIL_END_OF_SCOPE

  Hide_warnings = true;
  // End Test Setup
  (*Tests[i])();
  if (Passed && !Hide_errors && trace_contains_errors()) {
    Passed = false;
  }
  // End Test Teardown
}



ostream& operator<<(ostream& os, end /*unused*/) {
  if (Trace_stream) Trace_stream->newline();
  return os;
}

void trace_stream::newline() {
  if (!curr_stream) return;
  string curr_contents = curr_stream->str();
  if (!curr_contents.empty()) {
    past_lines.push_back(trace_line(curr_contents, trim(curr_label), curr_depth));  // preserve indent in contents
    // maybe incrementally dump trace
    trace_line& t = past_lines.back();
    if (should_incrementally_print_trace()) {
      cerr       << std::setw(4) << t.depth << ' ' << t.label << ": " << t.contents << '\n';
    }
    if (Trace_file) {
      Trace_file << std::setw(4) << t.depth << ' ' << t.label << ": " << t.contents << '\n';
    }
    // End trace Commit
  }

  // clean up
  delete curr_stream;
  curr_stream = NULL;
  curr_label.clear();
  curr_depth = Max_depth;
}


lease_tracer::lease_tracer() { Trace_stream = new trace_stream; }
lease_tracer::~lease_tracer() {
  delete Trace_stream;
  Trace_stream = NULL;
}


bool trace_stream::should_incrementally_print_trace() {
  if (!Hide_errors && curr_depth == Error_depth) return true;
  if (!Hide_warnings && !Hide_errors && curr_depth == Warn_depth) return true;
  if (Dump_trace) return true;


  // End Incremental Trace Print Conditions
  return false;
}
bool trace_contains_errors() {
  return Trace_errors > 0 || trace_count("error") > 0;
}

bool check_trace_contents(string FUNCTION, string FILE, int LINE, string expected) {
  if (!Passed) return false;
  if (!Trace_stream) return false;
  vector<string> expected_lines = split(expected, "\n");
  int curr_expected_line = 0;
  while (curr_expected_line < SIZE(expected_lines) && expected_lines.at(curr_expected_line).empty())
    ++curr_expected_line;
  if (curr_expected_line == SIZE(expected_lines)) return true;
  string label, contents;
  split_label_contents(expected_lines.at(curr_expected_line), &label, &contents);
  for (vector<trace_line>::iterator p = Trace_stream->past_lines.begin();  p != Trace_stream->past_lines.end();  ++p) {
    if (label != p->label) continue;
    if (contents != trim(p->contents)) continue;
    ++curr_expected_line;
    while (curr_expected_line < SIZE(expected_lines) && expected_lines.at(curr_expected_line).empty())
      ++curr_expected_line;
    if (curr_expected_line == SIZE(expected_lines)) return true;
    split_label_contents(expected_lines.at(curr_expected_line), &label, &contents);
  }

  if (line_exists_anywhere(label, contents)) {
    cerr << "\nF - " << FUNCTION << "(" << FILE << ":" << LINE << "): line [" << label << ": " << contents << "] out of order in trace:\n";
    DUMP("");
  }
  else {
    cerr << "\nF - " << FUNCTION << "(" << FILE << ":" << LINE << "): missing [" << contents << "] in trace:\n";
    DUMP(label);
  }
  Passed = false;
  return false;
}

bool trace_doesnt_contain(string expected) {
  vector<string> tmp = split_first(expected, ": ");
  if (SIZE(tmp) == 1) {
    raise << expected << ": missing label or contents in trace line\n" << end();
    assert(false);
  }
  return trace_count(tmp.at(0), tmp.at(1)) == 0;
}

int trace_count(string label) {
  return trace_count(label, "");
}

int trace_count(string label, string line) {
  if (!Trace_stream) return 0;
  long result = 0;
  for (vector<trace_line>::iterator p = Trace_stream->past_lines.begin();  p != Trace_stream->past_lines.end();  ++p) {
    if (label == p->label) {
      if (line == "" || trim(line) == trim(p->contents))
        ++result;
    }
  }
  return result;
}

int trace_count_prefix(string label, string prefix) {
  if (!Trace_stream) return 0;
  long result = 0;
  for (vector<trace_line>::iterator p = Trace_stream->past_lines.begin();  p != Trace_stream->past_lines.end();  ++p) {
    if (label == p->label) {
      if (starts_with(trim(p->contents), trim(prefix)))
        ++result;
    }
  }
  return result;
}

void split_label_contents(const string& s, string* label, string* contents) {
  static const string delim(": ");
  size_t pos = s.find(delim);
  if (pos == string::npos) {
    *label = "";
    *contents = trim(s);
  }
  else {
    *label = trim(s.substr(0, pos));
    *contents = trim(s.substr(pos+SIZE(delim)));
  }
}

bool line_exists_anywhere(const string& label, const string& contents) {
  for (vector<trace_line>::iterator p = Trace_stream->past_lines.begin();  p != Trace_stream->past_lines.end();  ++p) {
    if (label != p->label) continue;
    if (contents == trim(p->contents)) return true;
  }
  return false;
}

vector<string> split(string s, string delim) {
  vector<string> result;
  size_t begin=0, end=s.find(delim);
  while (true) {
    if (end == string::npos) {
      result.push_back(string(s, begin, string::npos));
      break;
    }
    result.push_back(string(s, begin, end-begin));
    begin = end+SIZE(delim);
    end = s.find(delim, begin);
  }
  return result;
}

vector<string> split_first(string s, string delim) {
  vector<string> result;
  size_t end=s.find(delim);
  result.push_back(string(s, 0, end));
  if (end != string::npos)
    result.push_back(string(s, end+SIZE(delim), string::npos));
  return result;
}


void cleanup_main() {
  if (Trace_file) Trace_file.close();
  // End cleanup_main
}

string trim(const string& s) {
  string::const_iterator first = s.begin();
  while (first != s.end() && isspace(*first))
    ++first;
  if (first == s.end()) return "";

  string::const_iterator last = --s.end();
  while (last != s.begin() && isspace(*last))
    --last;
  ++last;
  return string(first, last);
}

void test_trace_check_compares() {
  trace("test layer") << "foo" << end();
  CHECK_TRACE_CONTENTS("test layer: foo");
}

void test_trace_check_ignores_other_layers() {
  trace("test layer 1") << "foo" << end();
  trace("test layer 2") << "bar" << end();
  CHECK_TRACE_CONTENTS("test layer 1: foo");
  CHECK_TRACE_DOESNT_CONTAIN("test layer 2: foo");
}

void test_trace_check_ignores_leading_whitespace() {
  trace("test layer 1") << " foo" << end();
  CHECK_EQ(trace_count("test layer 1", /*too little whitespace*/"foo"), 1);
  CHECK_EQ(trace_count("test layer 1", /*too much whitespace*/"  foo"), 1);
}

void test_trace_check_ignores_other_lines() {
  trace("test layer 1") << "foo" << end();
  trace("test layer 1") << "bar" << end();
  CHECK_TRACE_CONTENTS("test layer 1: foo");
}

void test_trace_check_ignores_other_lines2() {
  trace("test layer 1") << "foo" << end();
  trace("test layer 1") << "bar" << end();
  CHECK_TRACE_CONTENTS("test layer 1: bar");
}

void test_trace_ignores_trailing_whitespace() {
  trace("test layer 1") << "foo\n" << end();
  CHECK_TRACE_CONTENTS("test layer 1: foo");
}

void test_trace_ignores_trailing_whitespace2() {
  trace("test layer 1") << "foo " << end();
  CHECK_TRACE_CONTENTS("test layer 1: foo");
}

void test_trace_orders_across_layers() {
  trace("test layer 1") << "foo" << end();
  trace("test layer 2") << "bar" << end();
  trace("test layer 1") << "qux" << end();
  CHECK_TRACE_CONTENTS("test layer 1: foo\n"
                       "test layer 2: bar\n"
                       "test layer 1: qux\n");
}

void test_trace_supports_count() {
  trace("test layer 1") << "foo" << end();
  trace("test layer 1") << "foo" << end();
  CHECK_EQ(trace_count("test layer 1", "foo"), 2);
}

void test_trace_supports_count2() {
  trace("test layer 1") << "foo" << end();
  trace("test layer 1") << "bar" << end();
  CHECK_EQ(trace_count("test layer 1"), 2);
}

void test_trace_count_ignores_trailing_whitespace() {
  trace("test layer 1") << "foo\n" << end();
  CHECK_EQ(trace_count("test layer 1", "foo"), 1);
}

// pending: DUMP tests
// pending: readable_contents() adds newline if necessary.
// pending: raise also prints to stderr.
// pending: raise doesn't print to stderr if Hide_errors is set.
// pending: warn doesn't print to stderr if Hide_errors is set.
// pending: warn doesn't print to stderr if Hide_warnings is set.
// pending: raise doesn't have to be saved if Hide_errors is set, just printed.
// pending: raise prints to stderr if Trace_stream is NULL.
// pending: raise prints to stderr if Trace_stream is NULL even if Hide_errors is set.

// can't check trace because trace methods call 'split'

void test_split_returns_at_least_one_elem() {
  vector<string> result = split("", ",");
  CHECK_EQ(result.size(), 1);
  CHECK_EQ(result.at(0), "");
}

void test_split_returns_entire_input_when_no_delim() {
  vector<string> result = split("abc", ",");
  CHECK_EQ(result.size(), 1);
  CHECK_EQ(result.at(0), "abc");
}

void test_split_works() {
  vector<string> result = split("abc,def", ",");
  CHECK_EQ(result.size(), 2);
  CHECK_EQ(result.at(0), "abc");
  CHECK_EQ(result.at(1), "def");
}

void test_split_works2() {
  vector<string> result = split("abc,def,ghi", ",");
  CHECK_EQ(result.size(), 3);
  CHECK_EQ(result.at(0), "abc");
  CHECK_EQ(result.at(1), "def");
  CHECK_EQ(result.at(2), "ghi");
}

void test_split_handles_multichar_delim() {
  vector<string> result = split("abc,,def,,ghi", ",,");
  CHECK_EQ(result.size(), 3);
  CHECK_EQ(result.at(0), "abc");
  CHECK_EQ(result.at(1), "def");
  CHECK_EQ(result.at(2), "ghi");
}

void test_trim() {
  CHECK_EQ(trim(""), "");
  CHECK_EQ(trim(" "), "");
  CHECK_EQ(trim("  "), "");
  CHECK_EQ(trim("a"), "a");
  CHECK_EQ(trim(" a"), "a");
  CHECK_EQ(trim("  a"), "a");
  CHECK_EQ(trim("  ab"), "ab");
  CHECK_EQ(trim("a "), "a");
  CHECK_EQ(trim("a  "), "a");
  CHECK_EQ(trim("ab  "), "ab");
  CHECK_EQ(trim(" a "), "a");
  CHECK_EQ(trim("  a  "), "a");
  CHECK_EQ(trim("  ab  "), "ab");
}

