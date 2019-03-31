
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

// Combine 'arg1' and 'arg2' with arithmetic operation 'op' and store the
// result in 'arg1', then update flags.
// beware: no side-effects in args
#define BINARY_ARITHMETIC_OP(op, arg1, arg2) { \
  /* arg1 and arg2 must be signed */ \
  int64_t tmp = arg1 op arg2; \
  arg1 = arg1 op arg2; \
  trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << arg1 << end(); \
  SF = (arg1 < 0); \
  ZF = (arg1 == 0); \
  OF = (arg1 != tmp); \
}

// Combine 'arg1' and 'arg2' with bitwise operation 'op' and store the result
// in 'arg1', then update flags.
#define BINARY_BITWISE_OP(op, arg1, arg2) { \
  /* arg1 and arg2 must be unsigned */ \
  arg1 = arg1 op arg2; \
  trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << arg1 << end(); \
  SF = (arg1 >> 31); \
  ZF = (arg1 == 0); \
  OF = false; \
}


#include <iomanip>
#define HEXBYTE  std::hex << std::setw(2) << std::setfill('0')
#define HEXWORD  std::hex << std::setw(8) << std::setfill('0')
// ugly that iostream doesn't print uint8_t as an integer
#define NUM(X) static_cast<int>(X)
#include <stdint.h>

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
enum {
  EAX,
  ECX,
  EDX,
  EBX,
  ESP,
  EBP,
  ESI,
  EDI,
  NUM_INT_REGISTERS,
};
union reg {
  int32_t i;
  uint32_t u;
};
const uint32_t SEGMENT_ALIGNMENT = 0x1000000;  // 16MB
inline uint32_t align_upwards(uint32_t x, uint32_t align) {
  return (x+align-1) & -(align);
}

// Like in real-world Linux, we'll allocate RAM for our programs in disjoint
// slabs called VMAs or Virtual Memory Areas.
struct vma {
  uint32_t start;  // inclusive
  uint32_t end;  // exclusive
  vector<uint8_t> _data;
  vma(uint32_t s, uint32_t e) :start(s), end(e) {}
  vma(uint32_t s) :start(s), end(align_upwards(s+1, SEGMENT_ALIGNMENT)) {}
  bool match(uint32_t a) {
    return a >= start && a < end;
  }
  bool match32(uint32_t a) {
    return a >= start && a+4 <= end;
  }
  uint8_t& data(uint32_t a) {
    assert(match(a));
    uint32_t result_index = a-start;
    if (_data.size() <= result_index) {
      const int align = 0x1000;
      uint32_t result_size = result_index + 1;  // size needed for result_index to be valid
      uint32_t new_size = align_upwards(result_size, align);
      // grow at least 2x to maintain some amortized complexity guarantees
      if (new_size < _data.size() * 2)
        new_size = _data.size() * 2;
      // never grow past the stated limit
      if (new_size > end-start)
        new_size = end-start;
      _data.resize(new_size);
    }
    return _data.at(result_index);
  }
  void grow_until(uint32_t new_end_address) {
    if (new_end_address < end) return;
    // Ugly: vma knows about the global Memory list of vmas
    void sanity_check(uint32_t start, uint32_t end);
    sanity_check(start, new_end_address);
    end = new_end_address;
  }
  // End vma Methods
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
reg Reg[NUM_INT_REGISTERS] = { {0} };
uint32_t EIP = 1;  // preserve null pointer
// the subset of x86 flag registers we care about
bool SF = false;  // sign flag
bool ZF = false;  // zero flag
bool OF = false;  // overflow flag
// RAM is made of VMAs.
vector<vma> Mem;
uint32_t End_of_program = 0;  // when the program executes past this address in tests we'll stop the test
// The stack grows downward. Can't increase its size for now.
map</*op*/string, string> Name;
map</*op*/string, string> Name_0f;
map</*op*/string, string> Name_f3;
map</*op*/string, string> Name_f3_0f;
extern const int Initial_callstack_depth = 2;
int Callstack_depth = Initial_callstack_depth;
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
  init_op_names();
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
    if (key == "opcodes") {
      cerr << "Opcodes currently supported by SubX:\n";
      for (map<string, string>::iterator p = Name.begin();  p != Name.end();  ++p)
        cerr << "  " << p->first << ": " << p->second << '\n';
      for (map<string, string>::iterator p = Name_0f.begin();  p != Name_0f.end();  ++p)
        cerr << "  0f " << p->first << ": " << p->second << '\n';
      for (map<string, string>::iterator p = Name_f3.begin();  p != Name_f3.end();  ++p)
        cerr << "  f3 " << p->first << ": " << p->second << '\n';
      for (map<string, string>::iterator p = Name_f3_0f.begin();  p != Name_f3_0f.end();  ++p)
        cerr << "  f3 0f " << p->first << ": " << p->second << '\n';
      cerr << "Run `subx help instructions` for details on words like 'r32' and 'disp8'.\n"
              "For complete details on these instructions, consult the IA-32 manual (volume 2).\n"
              "There's various versions of it online, such as https://c9x.me/x86.\n"
              "The mnemonics in brackets will help you locate each instruction.\n";
      return 0;
    }
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
  bzero(Reg, sizeof(Reg));
  EIP = 1;  // preserve null pointer

  SF = ZF = OF = false;


  Mem.clear();
  End_of_program = 0;
  Callstack_depth = Initial_callstack_depth;

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
  cerr << "  registers\n";
  cerr << "  opcodes\n";

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
  put_new(Help, "registers",
    "SubX currently supports eight 32-bit integer registers. From 0 to 7, they are:\n"
    "  EAX ECX EDX EBX ESP EBP ESI EDI\n"
    "ESP contains the top of the stack.\n"
    "\n"
    "-- 8-bit registers\n"
    "Some instructions operate on eight *overlapping* 8-bit registers.\n"
    "From 0 to 7, they are:\n"
    "  AL CL DL BL AH CH DH BH\n"
    "The 8-bit registers overlap with the 32-bit ones. AL is the lowest signicant byte\n"
    "of EAX, AH is the second lowest significant byte, and so on.\n"
    "\n"
    "For example, if EBX contains 0x11223344, then BL contains 0x44, and BH contains 0x33.\n"
    "\n"
    "There is no way to access bytes within ESP, EBP, ESI or EDI.\n"
    "\n"
    "For complete details consult the IA-32 software developer's manual, volume 2,\n"
    "table 2-2, \"32-bit addressing forms with the ModR/M byte\".\n"
    "It is included in this repository as 'modrm.pdf'.\n"
    "The register encodings are described in the top row of the table, but you'll need\n"
    "to spend some time with it.\n"
    "\n"
    "-- flag registers\n"
    "Various instructions (particularly 'compare') modify one or more of three 1-bit 'flag'\n"
    "registers, as a side-effect:\n"
    "- the sign flag (SF): usually set if an arithmetic result is negative, or\n"
    "  reset if not.\n"
    "- the zero flag (ZF): usually set if a result is zero, or reset if not.\n"
    "- the overflow flag (OF): usually set if an arithmetic result overflows.\n"
    "The flag bits are read by conditional jumps.\n"
    "\n"
    "For complete details on how different instructions update the flags, consult the IA-32\n"
    "manual (volume 2). There's various versions of it online, such as https://c9x.me/x86,\n"
    "though of course you'll need to be careful to ignore instructions and flag registers\n"
    "that SubX doesn't support.\n"
    "\n"
    "It isn't simple, but if this is the processor you have running on your computer,\n"
    "might as well get good at it.\n"
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



void sanity_check(uint32_t start, uint32_t end) {
  bool dup_found = false;
  for (int i = 0;  i < SIZE(Mem);  ++i) {
    const vma& curr = Mem.at(i);
    if (curr.start == start) {
      assert(!dup_found);
      dup_found = true;
    }
    else if (curr.start > start) {
      assert(curr.start > end);
    }
    else if (curr.start < start) {
      assert(curr.end < start);
    }
  }
}

// The first 3 VMAs are special. When loading ELF binaries in later layers,
// we'll assume that the first VMA is for code, the second is for data
// (including the heap), and the third for the stack.
void grow_code_segment(uint32_t new_end_address) {
  assert(!Mem.empty());
  Mem.at(0).grow_until(new_end_address);
}
void grow_data_segment(uint32_t new_end_address) {
  assert(SIZE(Mem) > 1);
  Mem.at(1).grow_until(new_end_address);
}
// These helpers depend on Mem being laid out contiguously (so you can't use a
// map, etc.) and on the host also being little-endian.
inline uint8_t read_mem_u8(uint32_t addr) {
  uint8_t* handle = mem_addr_u8(addr);  // error messages get printed here
  return handle ? *handle : 0;
}
inline int8_t read_mem_i8(uint32_t addr) {
  return static_cast<int8_t>(read_mem_u8(addr));
}
inline uint32_t read_mem_u32(uint32_t addr) {
  uint32_t* handle = mem_addr_u32(addr);  // error messages get printed here
  return handle ? *handle : 0;
}
inline int32_t read_mem_i32(uint32_t addr) {
  return static_cast<int32_t>(read_mem_u32(addr));
}

inline uint8_t* mem_addr_u8(uint32_t addr) {
  uint8_t* result = NULL;
  for (int i = 0;  i < SIZE(Mem);  ++i) {
    if (Mem.at(i).match(addr)) {
      if (result)
        raise << "address 0x" << HEXWORD << addr << " is in two segments\n" << end();
      result = &Mem.at(i).data(addr);
    }
  }
  if (result == NULL)
    raise << "Tried to access uninitialized memory at address 0x" << HEXWORD << addr << '\n' << end();
  return result;
}
inline int8_t* mem_addr_i8(uint32_t addr) {
  return reinterpret_cast<int8_t*>(mem_addr_u8(addr));
}
inline uint32_t* mem_addr_u32(uint32_t addr) {
  uint32_t* result = NULL;
  for (int i = 0;  i < SIZE(Mem);  ++i) {
    if (Mem.at(i).match32(addr)) {
      if (result)
        raise << "address 0x" << HEXWORD << addr << " is in two segments\n" << end();
      result = reinterpret_cast<uint32_t*>(&Mem.at(i).data(addr));
    }
  }
  if (result == NULL) {
    raise << "Tried to access uninitialized memory at address 0x" << HEXWORD << addr << '\n' << end();
    raise << "The entire 4-byte word should be initialized and lie in a single segment.\n" << end();
  }
  return result;
}
inline int32_t* mem_addr_i32(uint32_t addr) {
  return reinterpret_cast<int32_t*>(mem_addr_u32(addr));
}
// helper for some syscalls. But read-only.
inline const char* mem_addr_kernel_string(uint32_t addr) {
  return reinterpret_cast<const char*>(mem_addr_u8(addr));
}
inline string mem_addr_string(uint32_t addr, uint32_t size) {
  ostringstream out;
  for (size_t i = 0;  i < size;  ++i)
    out << read_mem_u8(addr+i);
  return out.str();
}


inline void write_mem_u8(uint32_t addr, uint8_t val) {
  uint8_t* handle = mem_addr_u8(addr);
  if (handle != NULL) *handle = val;
}
inline void write_mem_i8(uint32_t addr, int8_t val) {
  int8_t* handle = mem_addr_i8(addr);
  if (handle != NULL) *handle = val;
}
inline void write_mem_u32(uint32_t addr, uint32_t val) {
  uint32_t* handle = mem_addr_u32(addr);
  if (handle != NULL) *handle = val;
}
inline void write_mem_i32(uint32_t addr, int32_t val) {
  int32_t* handle = mem_addr_i32(addr);
  if (handle != NULL) *handle = val;
}

inline bool already_allocated(uint32_t addr) {
  bool result = false;
  for (int i = 0;  i < SIZE(Mem);  ++i) {
    if (Mem.at(i).match(addr)) {
      if (result)
        raise << "address 0x" << HEXWORD << addr << " is in two segments\n" << end();
      result = true;
    }
  }
  return result;
}


// skeleton of how x86 instructions are decoded
void run_one_instruction() {
  uint8_t op=0, op2=0, op3=0;
  // Run One Instruction
  if (Trace_file) {
    dump_registers();
    // End Dump Info for Instruction
  }
  uint32_t inst_start_address = EIP;
  op = next();
  trace(Callstack_depth, "run") << "0x" << HEXWORD << inst_start_address << " opcode: " << HEXBYTE << NUM(op) << call_label(op) << end();
  switch (op) {
  case 0xf4:  // hlt
    EIP = End_of_program;
    break;
  // End Single-Byte Opcodes
  case 0x0f:
    switch(op2 = next()) {
    // End Two-Byte Opcodes Starting With 0f
    default:
      cerr << "unrecognized second opcode after 0f: " << HEXBYTE << NUM(op2) << '\n';
      DUMP("");
      exit(1);
    }
    break;
  case 0xf2:
    switch(op2 = next()) {
    // End Two-Byte Opcodes Starting With f2
    case 0x0f:
      switch(op3 = next()) {
      // End Three-Byte Opcodes Starting With f2 0f
      default:
        cerr << "unrecognized third opcode after f2 0f: " << HEXBYTE << NUM(op3) << '\n';
        DUMP("");
        exit(1);
      }
      break;
    default:
      cerr << "unrecognized second opcode after f2: " << HEXBYTE << NUM(op2) << '\n';
      DUMP("");
      exit(1);
    }
    break;
  case 0xf3:
    switch(op2 = next()) {
    // End Two-Byte Opcodes Starting With f3
    case 0x0f:
      switch(op3 = next()) {
      // End Three-Byte Opcodes Starting With f3 0f
      default:
        cerr << "unrecognized third opcode after f3 0f: " << HEXBYTE << NUM(op3) << '\n';
        DUMP("");
        exit(1);
      }
      break;
    default:
      cerr << "unrecognized second opcode after f3: " << HEXBYTE << NUM(op2) << '\n';
      DUMP("");
      exit(1);
    }
    break;
  default:
    cerr << "unrecognized opcode: " << HEXBYTE << NUM(op) << '\n';
    DUMP("");
    exit(1);
  }
}

inline uint8_t next() {
  return read_mem_u8(EIP++);
}

void dump_registers() {
  ostringstream out;
  out << "registers: ";
  for (int i = 0;  i < NUM_INT_REGISTERS;  ++i) {
    if (i > 0) out << "; ";
    out << "  " << i << ": " << std::hex << std::setw(8) << std::setfill('_') << Reg[i].u;
  }
  out << " -- SF: " << SF << "; ZF: " << ZF << "; OF: " << OF;
  trace(Callstack_depth+1, "run") << out.str() << end();
}

// debugging info from a later layer
string call_label(uint8_t op) {
  if (op != 0xe8) return "";
  // End Trace Call Instruction
  return "/call";
}

void init_op_names() {
  put(Name, "f4", "halt (hlt)");
  // End Initialize Op Names
}

