
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

// Very primitive/fixed/insecure ELF segments for now: just consecutive VMAs.
//   code: 0x09000000 -> 0x09ffffff
//   data/heap: 0x0a000000 -> 0x0affffff
//   stack: 0x0b000ffc -> 0x0b000000 (downward)
const int CODE_SEGMENT = 0x09000000;
const int DATA_SEGMENT = 0x0a000000;  // keep sync'd with `Heap.limit` in allocate.subx
const int STACK_SEGMENT = 0x0b000000;
const int AFTER_STACK = 0x0c000000;
const int ARGV_DATA_SEGMENT = 0x0c000000;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>

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
struct word {
  string original;
  string data;
  vector<string> metadata;
};


struct line {
  vector<word> words;
  vector<string> metadata;
  string original;
};
struct segment {
  uint32_t start;
  vector<line> lines;
  // End segment Fields
  segment() {
    start = 0;
    // End segment Constructor
  }
};
struct program {
  vector<segment> segments;
  // random ideas for other things we may eventually need
  //map<name, address> globals;
  //vector<recipe> recipes;
  //map<string, type_info> types;
};
typedef void (*transform_fn)(program&);
struct perr {};
struct die {};
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
vector<transform_fn> Transform;

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

  assert(argc > 1);
  if (is_equal(argv[1], "run")) {
    START_TRACING_UNTIL_END_OF_SCOPE;
    trace(2, "run") << "=== Starting to run" << end();
    assert(argc > 2);
    reset();
    cerr << std::hex;
    load_elf(argv[2], argc, argv);
    while (EIP < End_of_program)  // weak final-gasp termination check
      run_one_instruction();
    raise << "executed past end of the world: " << EIP << " vs " << End_of_program << '\n' << end();
    return 1;
  }

  // End Main
  cerr << "nothing to do\n";
  return 1;


  return 0;
}

// Unit Tests
void test_parse_int() {
  CHECK_EQ(0, parse_int("0"));
  CHECK_EQ(0, parse_int("0x0"));
  CHECK_EQ(0, parse_int("0x0"));
  CHECK_EQ(16, parse_int("10"));  // hex always
  CHECK_EQ(-1, parse_int("-1"));
  CHECK_EQ(-1, parse_int("0xffffffff"));
}

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

  cerr << "  syntax\n";

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

  put_new(Help, "syntax",
    "SubX programs consist of segments, each segment in turn consisting of lines.\n"
    "Line-endings are significant; each line should contain a single\n"
    "instruction, macro or directive.\n"
    "\n"
    "Comments start with the '#' character. It should be at the start of a word\n"
    "(start of line, or following a space).\n"
    "\n"
    "Each segment starts with a header line: a '==' delimiter followed by the name of\n"
    "the segment.\n"
    "\n"
    "The first segment contains code and should be called 'code'.\n"
    "The second segment should be called 'data'.\n"
    "The resulting binary starts running from the start of the code segment by default.\n"
    "To start elsewhere in the code segment, define a special label called 'Entry'.\n"
    "\n"
    "Segments with the same name get merged together. This rule helps keep functions and\n"
    "their data close together in .subx files.\n"
    "\n"
    "Lines consist of a series of words. Words can contain arbitrary metadata\n"
    "after a '/', but they can never contain whitespace. Metadata has no effect\n"
    "at runtime, but can be handy when rewriting macros.\n"
    "\n"
    "Check out the examples in the examples/ directory.\n"
    "Programming in machine code can be annoying, but let's see if we can make\n"
    "it nice enough to be able to write a compiler in it.\n"
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
    //? dump_stack();  // slow
    // End Dump Info for Instruction
  }
  uint32_t inst_start_address = EIP;
  op = next();
  trace(Callstack_depth, "run") << "0x" << HEXWORD << inst_start_address << " opcode: " << HEXBYTE << NUM(op) << call_label(op) << end();
  switch (op) {
  case 0xf4:  // hlt
    EIP = End_of_program;
    break;
  case 0x05: {  // add imm32 to EAX
    int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "add imm32 0x" << HEXWORD << arg2 << " to reg EAX" << end();
    BINARY_ARITHMETIC_OP(+, Reg[EAX].i, arg2);
    break;
  }

  case 0x01: {  // add r32 to r/m32
    uint8_t modrm = next();
    uint8_t arg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "add " << rname(arg2) << " to r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    BINARY_ARITHMETIC_OP(+, *arg1, Reg[arg2].i);
    break;
  }

  case 0x29: {  // subtract r32 from r/m32
    const uint8_t modrm = next();
    const uint8_t arg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "subtract " << rname(arg2) << " from r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    BINARY_ARITHMETIC_OP(-, *arg1, Reg[arg2].i);
    break;
  }


  case 0xf7: {
    const uint8_t modrm = next();
    trace(Callstack_depth+1, "run") << "operate on r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    switch (subop) {
    case 4: {  // mul unsigned EAX by r/m32
      trace(Callstack_depth+1, "run") << "subop: multiply EAX by r/m32" << end();
      const uint64_t result = Reg[EAX].u * static_cast<uint32_t>(*arg1);
      Reg[EAX].u = result & 0xffffffff;
      Reg[EDX].u = result >> 32;
      OF = (Reg[EDX].u != 0);
      trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << Reg[EAX].u << end();
      break;
    }
    case 3: {  // negate r/m32
      trace(Callstack_depth+1, "run") << "subop: negate" << end();
      // one case that can overflow
      if (static_cast<uint32_t>(*arg1) == 0x80000000) {
        trace(Callstack_depth+1, "run") << "overflow" << end();
        SF = true;
        ZF = false;
        OF = true;
        break;
      }
      *arg1 = -(*arg1);
      trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *arg1 << end();
      SF = (*arg1 >> 31);
      ZF = (*arg1 == 0);
      OF = false;
      break;
    }

    case 2: {  // not r/m32
      trace(Callstack_depth+1, "run") << "subop: not" << end();
      *arg1 = ~(*arg1);
      trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *arg1 << end();
      SF = (*arg1 >> 31);
      ZF = (*arg1 == 0);
      OF = false;
      break;
    }


    // End Op f7 Subops
    default:
      cerr << "unrecognized subop for opcode f7: " << NUM(subop) << '\n';
      exit(1);
    }
    break;
  }


  case 0xd3: {
    const uint8_t modrm = next();
    trace(Callstack_depth+1, "run") << "operate on r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    switch (subop) {
    case 4: {  // shift left r/m32 by CL
      trace(Callstack_depth+1, "run") << "subop: shift left by CL bits" << end();
      uint8_t count = Reg[ECX].u & 0x1f;
      // OF is only defined if count is 1
      if (count == 1) {
        bool msb = (*arg1 & 0x80000000) >> 1;
        bool pnsb = (*arg1 & 0x40000000);
        OF = (msb != pnsb);
      }
      *arg1 = (*arg1 << count);
      ZF = (*arg1 == 0);
      SF = (*arg1 < 0);
      trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *arg1 << end();
      break;
    }
    case 7: {  // shift right r/m32 by CL, preserving sign
      trace(Callstack_depth+1, "run") << "subop: shift right by CL bits, while preserving sign" << end();
      uint8_t count = Reg[ECX].u & 0x1f;
      *arg1 = (*arg1 >> count);
      ZF = (*arg1 == 0);
      SF = (*arg1 < 0);
      // OF is only defined if count is 1
      if (count == 1) OF = false;
      trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *arg1 << end();
      break;
    }

    case 5: {  // shift right r/m32 by CL, preserving sign
      trace(Callstack_depth+1, "run") << "subop: shift right by CL bits, while padding zeroes" << end();
      uint8_t count = Reg[ECX].u & 0x1f;
      // OF is only defined if count is 1
      if (count == 1) {
        bool msb = (*arg1 & 0x80000000) >> 1;
        bool pnsb = (*arg1 & 0x40000000);
        OF = (msb != pnsb);
      }
      uint32_t* uarg1 = reinterpret_cast<uint32_t*>(arg1);
      *uarg1 = (*uarg1 >> count);
      ZF = (*uarg1 == 0);
      // result is always positive by definition
      SF = false;
      trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *arg1 << end();
      break;
    }

    // End Op d3 Subops
    default:
      cerr << "unrecognized subop for opcode d3: " << NUM(subop) << '\n';
      exit(1);
    }
    break;
  }


  case 0x21: {  // and r32 with r/m32
    const uint8_t modrm = next();
    const uint8_t arg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "and " << rname(arg2) << " with r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    BINARY_BITWISE_OP(&, *arg1, Reg[arg2].u);
    break;
  }


  case 0x09: {  // or r32 with r/m32
    const uint8_t modrm = next();
    const uint8_t arg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "or " << rname(arg2) << " with r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    BINARY_BITWISE_OP(|, *arg1, Reg[arg2].u);
    break;
  }


  case 0x31: {  // xor r32 with r/m32
    const uint8_t modrm = next();
    const uint8_t arg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "xor " << rname(arg2) << " with r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    BINARY_BITWISE_OP(^, *arg1, Reg[arg2].u);
    break;
  }


  case 0x39: {  // set SF if r/m32 < r32
    const uint8_t modrm = next();
    const uint8_t reg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "compare " << rname(reg2) << " with r/m32" << end();
    const int32_t* arg1 = effective_address(modrm);
    const int32_t arg2 = Reg[reg2].i;
    const int32_t tmp1 = *arg1 - arg2;
    SF = (tmp1 < 0);
    ZF = (tmp1 == 0);
    const int64_t tmp2 = *arg1 - arg2;
    OF = (tmp1 != tmp2);
    trace(Callstack_depth+1, "run") << "SF=" << SF << "; ZF=" << ZF << "; OF=" << OF << end();
    break;
  }

  case 0x89: {  // copy r32 to r/m32
    const uint8_t modrm = next();
    const uint8_t rsrc = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "copy " << rname(rsrc) << " to r/m32" << end();
    int32_t* dest = effective_address(modrm);
    *dest = Reg[rsrc].i;
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *dest << end();
    break;
  }


  case 0x87: {  // exchange r32 with r/m32
    const uint8_t modrm = next();
    const uint8_t reg2 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "exchange " << rname(reg2) << " with r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    const int32_t tmp = *arg1;
    *arg1 = Reg[reg2].i;
    Reg[reg2].i = tmp;
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *arg1 << " in r/m32" << end();
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << Reg[reg2].i << " in " << rname(reg2) << end();
    break;
  }


  case 0x40:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47: {  // increment r32
    const uint8_t reg = op & 0x7;
    trace(Callstack_depth+1, "run") << "increment " << rname(reg) << end();
    ++Reg[reg].u;
    trace(Callstack_depth+1, "run") << "storing value 0x" << HEXWORD << Reg[reg].u << end();
    break;
  }

  case 0xff: {
    const uint8_t modrm = next();
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    switch (subop) {
      case 0: {  // increment r/m32
        trace(Callstack_depth+1, "run") << "increment r/m32" << end();
        int32_t* arg = effective_address(modrm);
        ++*arg;
        trace(Callstack_depth+1, "run") << "storing value 0x" << HEXWORD << *arg << end();
        break;
      }
      default:
        cerr << "unrecognized subop for ff: " << HEXBYTE << NUM(subop) << '\n';
        DUMP("");
        exit(1);
      case 1: {  // decrement r/m32
        trace(Callstack_depth+1, "run") << "decrement r/m32" << end();
        int32_t* arg = effective_address(modrm);
        --*arg;
        trace(Callstack_depth+1, "run") << "storing value 0x" << HEXWORD << *arg << end();
        break;
      }


      case 4: {  // jump to r/m32
        trace(Callstack_depth+1, "run") << "jump to r/m32" << end();
        const int32_t* arg2 = effective_address(modrm);
        EIP = *arg2;
        trace(Callstack_depth+1, "run") << "jumping to 0x" << HEXWORD << EIP << end();
        break;
      }


      case 6: {  // push r/m32 to stack
        trace(Callstack_depth+1, "run") << "push r/m32" << end();
        const int32_t* val = effective_address(modrm);
        push(*val);
        break;
      }


      // End Op ff Subops
    }
    break;
  }


  case 0x48:
  case 0x49:
  case 0x4a:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x4e:
  case 0x4f: {  // decrement r32
    const uint8_t reg = op & 0x7;
    trace(Callstack_depth+1, "run") << "decrement " << rname(reg) << end();
    --Reg[reg].u;
    trace(Callstack_depth+1, "run") << "storing value 0x" << HEXWORD << Reg[reg].u << end();
    break;
  }

  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57: {  // push r32 to stack
    uint8_t reg = op & 0x7;
    trace(Callstack_depth+1, "run") << "push " << rname(reg) << end();
  //?   cerr << "push: " << NUM(reg) << ": " << Reg[reg].u << " => " << Reg[ESP].u << '\n';
    push(Reg[reg].u);
    break;
  }


  case 0x58:
  case 0x59:
  case 0x5a:
  case 0x5b:
  case 0x5c:
  case 0x5d:
  case 0x5e:
  case 0x5f: {  // pop stack into r32
    const uint8_t reg = op & 0x7;
    trace(Callstack_depth+1, "run") << "pop into " << rname(reg) << end();
  //?   cerr << "pop from " << Reg[ESP].u << '\n';
    Reg[reg].u = pop();
  //?   cerr << "=> " << NUM(reg) << ": " << Reg[reg].u << '\n';
    break;
  }
  case 0x03: {  // add r/m32 to r32
    const uint8_t modrm = next();
    const uint8_t arg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "add r/m32 to " << rname(arg1) << end();
    const int32_t* arg2 = effective_address(modrm);
    BINARY_ARITHMETIC_OP(+, Reg[arg1].i, *arg2);
    break;
  }


  case 0x2b: {  // subtract r/m32 from r32
    const uint8_t modrm = next();
    const uint8_t arg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "subtract r/m32 from " << rname(arg1) << end();
    const int32_t* arg2 = effective_address(modrm);
    BINARY_ARITHMETIC_OP(-, Reg[arg1].i, *arg2);
    break;
  }

  case 0x23: {  // and r/m32 with r32
    const uint8_t modrm = next();
    const uint8_t arg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "and r/m32 with " << rname(arg1) << end();
    const int32_t* arg2 = effective_address(modrm);
    BINARY_BITWISE_OP(&, Reg[arg1].u, *arg2);
    break;
  }


  case 0x0b: {  // or r/m32 with r32
    const uint8_t modrm = next();
    const uint8_t arg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "or r/m32 with " << rname(arg1) << end();
    const int32_t* arg2 = effective_address(modrm);
    BINARY_BITWISE_OP(|, Reg[arg1].u, *arg2);
    break;
  }


  case 0x33: {  // xor r/m32 with r32
    const uint8_t modrm = next();
    const uint8_t arg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "xor r/m32 with " << rname(arg1) << end();
    const int32_t* arg2 = effective_address(modrm);
    BINARY_BITWISE_OP(|, Reg[arg1].u, *arg2);
    break;
  }


  case 0x3b: {  // set SF if r32 < r/m32
    const uint8_t modrm = next();
    const uint8_t reg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "compare r/m32 with " << rname(reg1) << end();
    const int32_t arg1 = Reg[reg1].i;
    const int32_t* arg2 = effective_address(modrm);
    const int32_t tmp1 = arg1 - *arg2;
    SF = (tmp1 < 0);
    ZF = (tmp1 == 0);
    int64_t tmp2 = arg1 - *arg2;
    OF = (tmp1 != tmp2);
    trace(Callstack_depth+1, "run") << "SF=" << SF << "; ZF=" << ZF << "; OF=" << OF << end();
    break;
  }

  case 0x8b: {  // copy r32 to r/m32
    const uint8_t modrm = next();
    const uint8_t rdest = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "copy r/m32 to " << rname(rdest) << end();
    const int32_t* src = effective_address(modrm);
    Reg[rdest].i = *src;
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXWORD << *src << end();
    break;
  }


  case 0x8f: {  // pop stack into r/m32
    const uint8_t modrm = next();
    const uint8_t subop = (modrm>>3)&0x7;
    switch (subop) {
      case 0: {
        trace(Callstack_depth+1, "run") << "pop into r/m32" << end();
        int32_t* dest = effective_address(modrm);
        *dest = pop();
        break;
      }
    }
    break;
  }


  case 0x8d: {  // copy address of m32 to r32
    const uint8_t modrm = next();
    const uint8_t arg1 = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "copy address into " << rname(arg1) << end();
    Reg[arg1].u = effective_address_number(modrm);
    break;
  }

  // End Single-Byte Opcodes
  case 0x0f:
    switch(op2 = next()) {
    case 0xaf: {  // multiply r32 into r/m32
      const uint8_t modrm = next();
      const uint8_t arg2 = (modrm>>3)&0x7;
      trace(Callstack_depth+1, "run") << "multiply r/m32 into " << rname(arg2) << end();
      const int32_t* arg1 = effective_address(modrm);
      BINARY_ARITHMETIC_OP(*, Reg[arg2].i, *arg1);
      break;
    }


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
  put_new(Name, "05", "add imm32 to EAX (add)");

  put_new(Name, "01", "add r32 to rm32 (add)");

  put_new(Name, "29", "subtract r32 from rm32 (sub)");

  put_new(Name, "f7", "negate/multiply rm32 (with EAX if necessary) depending on subop (neg/mul)");

  put_new(Name_0f, "af", "multiply rm32 into r32 (imul)");

  put_new(Name, "d3", "shift rm32 by CL bits depending on subop (sal/sar/shl/shr)");

  put_new(Name, "21", "rm32 = bitwise AND of r32 with rm32 (and)");

  put_new(Name, "09", "rm32 = bitwise OR of r32 with rm32 (or)");

  put_new(Name, "31", "rm32 = bitwise XOR of r32 with rm32 (xor)");

  put_new(Name, "39", "compare: set SF if rm32 < r32 (cmp)");

  put_new(Name, "89", "copy r32 to rm32 (mov)");

  put_new(Name, "87", "swap the contents of r32 and rm32 (xchg)");

  put_new(Name, "40", "increment EAX (inc)");
  put_new(Name, "41", "increment ECX (inc)");
  put_new(Name, "42", "increment EDX (inc)");
  put_new(Name, "43", "increment EBX (inc)");
  put_new(Name, "44", "increment ESP (inc)");
  put_new(Name, "45", "increment EBP (inc)");
  put_new(Name, "46", "increment ESI (inc)");
  put_new(Name, "47", "increment EDI (inc)");

  put_new(Name, "ff", "increment/decrement/jump/push/call rm32 based on subop (inc/dec/jmp/push/call)");

  put_new(Name, "48", "decrement EAX (dec)");
  put_new(Name, "49", "decrement ECX (dec)");
  put_new(Name, "4a", "decrement EDX (dec)");
  put_new(Name, "4b", "decrement EBX (dec)");
  put_new(Name, "4c", "decrement ESP (dec)");
  put_new(Name, "4d", "decrement EBP (dec)");
  put_new(Name, "4e", "decrement ESI (dec)");
  put_new(Name, "4f", "decrement EDI (dec)");

  put_new(Name, "50", "push EAX to stack (push)");
  put_new(Name, "51", "push ECX to stack (push)");
  put_new(Name, "52", "push EDX to stack (push)");
  put_new(Name, "53", "push EBX to stack (push)");
  put_new(Name, "54", "push ESP to stack (push)");
  put_new(Name, "55", "push EBP to stack (push)");
  put_new(Name, "56", "push ESI to stack (push)");
  put_new(Name, "57", "push EDI to stack (push)");

  put_new(Name, "58", "pop top of stack to EAX (pop)");
  put_new(Name, "59", "pop top of stack to ECX (pop)");
  put_new(Name, "5a", "pop top of stack to EDX (pop)");
  put_new(Name, "5b", "pop top of stack to EBX (pop)");
  put_new(Name, "5c", "pop top of stack to ESP (pop)");
  put_new(Name, "5d", "pop top of stack to EBP (pop)");
  put_new(Name, "5e", "pop top of stack to ESI (pop)");
  put_new(Name, "5f", "pop top of stack to EDI (pop)");

  put_new(Name, "03", "add rm32 to r32 (add)");

  put_new(Name, "2b", "subtract rm32 from r32 (sub)");

  put_new(Name, "23", "r32 = bitwise AND of r32 with rm32 (and)");

  put_new(Name, "0b", "r32 = bitwise OR of r32 with rm32 (or)");

  put_new(Name, "33", "r32 = bitwise XOR of r32 with rm32 (xor)");

  put_new(Name, "3b", "compare: set SF if r32 < rm32 (cmp)");

  put_new(Name, "8b", "copy rm32 to r32 (mov)");

  put_new(Name, "8f", "pop top of stack to rm32 (pop)");

  put_new(Name, "8d", "copy address in rm32 into r32 (lea)");

  // End Initialize Op Names
}



void test_add_imm32_to_eax() {
  // At the lowest level, SubX programs are a series of hex bytes, each
  // (variable-length) instruction on one line.
  run(
      // Comments start with '#' and are ignored.
      "# comment\n"
      // Segment headers start with '==' and a name or starting hex address.
      // There's usually one code and one data segment. The code segment
      // always comes first.
      "== 0x1\n"  // code segment

      // After the header, each segment consists of lines, and each line
      // consists of words separated by whitespace.
      //
      // All words can have metadata after a '/'. No spaces allowed in
      // metadata, of course.
      // Unrecognized metadata never causes errors, so you can use it for
      // documentation.
      //
      // Within the code segment in particular, x86 instructions consist of
      // some number of the following parts and sub-parts (see the Readme and
      // cheatsheet.pdf for details):
      //   opcodes: 1-3 bytes
      //   ModR/M byte
      //   SIB byte
      //   displacement: 0/1/2/4 bytes
      //   immediate: 0/1/2/4 bytes
      // opcode        ModR/M                    SIB                   displacement    immediate
      // instruction   mod, reg, Reg/Mem bits    scale, index, base
      // 1-3 bytes     0/1 byte                  0/1 byte              0/1/2/4 bytes   0/1/2/4 bytes
      "  05            .                         .                     .               0a 0b 0c 0d\n"  // add 0x0d0c0b0a to EAX
      // The periods are just to help the eye track long gaps between columns,
      // and are otherwise ignored.
  );
  // This program, when run, causes the following events in the trace:
  CHECK_TRACE_CONTENTS(
      "load: 0x00000001 -> 05\n"
      "load: 0x00000002 -> 0a\n"
      "load: 0x00000003 -> 0b\n"
      "load: 0x00000004 -> 0c\n"
      "load: 0x00000005 -> 0d\n"
      "run: add imm32 0x0d0c0b0a to reg EAX\n"
      "run: storing 0x0d0c0b0a\n"
  );
}

// top-level helper for scenarios: parse the input, transform any macros, load
// the final hex bytes into memory, run it
void run(const string& text_bytes) {
  program p;
  istringstream in(text_bytes);
  parse(in, p);
  if (trace_contains_errors()) return;  // if any stage raises errors, stop immediately
  transform(p);
  if (trace_contains_errors()) return;
  load(p);
  if (trace_contains_errors()) return;
  while (EIP < End_of_program)
    run_one_instruction();
}


void parse(istream& fin, program& out) {
  vector<line> l;
  while (has_data(fin)) {
    string line_data;
    line curr;
    getline(fin, line_data);
    curr.original = line_data;
    trace(99, "parse") << "line: " << line_data << end();
    // End Line Parsing Special-cases(line_data -> l)
    istringstream lin(line_data);
    while (has_data(lin)) {
      string word_data;
      lin >> word_data;
      if (word_data.empty()) continue;
      if (word_data[0] == '#') break;  // comment
      if (word_data == ".") continue;  // comment token
      if (word_data == "==") {
        flush(out, l);
        string segment_title;
        lin >> segment_title;
        if (starts_with(segment_title, "0x")) {
          segment s;
          s.start = parse_int(segment_title);
          sanity_check_program_segment(out, s.start);
          if (trace_contains_errors()) continue;
          trace(3, "parse") << "new segment from 0x" << HEXWORD << s.start << end();
          out.segments.push_back(s);
        }
        // End Segment Parsing Special-cases(segment_title)
        // todo: segment segment metadata
        break;  // skip rest of line
      }
      if (word_data[0] == ':') {
        // todo: line metadata
        break;
      }
      curr.words.push_back(word());
      parse_word(word_data, curr.words.back());
      trace(99, "parse") << "word: " << to_string(curr.words.back());
    }
    if (!curr.words.empty())
      l.push_back(curr);
  }
  flush(out, l);
  trace(99, "parse") << "done" << end();
}

void flush(program& p, vector<line>& lines) {
  if (lines.empty()) return;
  if (p.segments.empty()) {
    raise << "input does not start with a '==' section header\n" << end();
    return;
  }
  // End flush(p, lines) Special-cases
  trace(99, "parse") << "flushing segment" << end();
  p.segments.back().lines.swap(lines);
}

void parse_word(const string& data, word& out) {
  out.original = data;
  istringstream win(data);
  if (getline(win, out.data, '/')) {
    string m;
    while (getline(win, m, '/'))
      out.metadata.push_back(m);
  }
}

void sanity_check_program_segment(const program& p, uint32_t addr) {
  for (int i = 0;  i < SIZE(p.segments);  ++i) {
    if (p.segments.at(i).start == addr)
      raise << "can't have multiple segments starting at address 0x" << HEXWORD << addr << '\n' << end();
  }
}

// helper for tests
void parse(const string& text_bytes) {
  program p;
  istringstream in(text_bytes);
  parse(in, p);
}

void test_detect_duplicate_segments() {
  Hide_errors = true;
  parse(
      "== 0xee\n"
      "ab\n"
      "== 0xee\n"
      "cd\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: can't have multiple segments starting at address 0x000000ee\n"
  );
}


void transform(program& p) {
  for (int t = 0;  t < SIZE(Transform);  ++t)
    (*Transform.at(t))(p);
}


void load(const program& p) {
  if (p.segments.empty()) {
    raise << "no code to run\n" << end();
    return;
  }
  // Ensure segments are disjoint.
  set<uint32_t> overlap;
  for (int i = 0;   i < SIZE(p.segments);  ++i) {
    const segment& seg = p.segments.at(i);
    uint32_t addr = seg.start;
    if (!already_allocated(addr))
      Mem.push_back(vma(seg.start));
    trace(99, "load") << "loading segment " << i << " from " << HEXWORD << addr << end();
    for (int j = 0;  j < SIZE(seg.lines);  ++j) {
      const line& l = seg.lines.at(j);
      for (int k = 0;  k < SIZE(l.words);  ++k) {
        const word& w = l.words.at(k);
        uint8_t val = hex_byte(w.data);
        if (trace_contains_errors()) return;
        assert(overlap.find(addr) == overlap.end());
        write_mem_u8(addr, val);
        overlap.insert(addr);
        trace(99, "load") << "0x" << HEXWORD << addr << " -> " << HEXBYTE << NUM(read_mem_u8(addr)) << end();
        ++addr;
      }
    }
    if (i == 0) End_of_program = addr;
  }
  EIP = p.segments.at(0).start;
  // End Initialize EIP
}

uint8_t hex_byte(const string& s) {
  istringstream in(s);
  int result = 0;
  in >> std::hex >> result;
  if (!in || !in.eof()) {
    raise << "token '" << s << "' is not a hex byte\n" << end();
    return '\0';
  }
  if (result > 0xff || result < -0x8f) {
    raise << "token '" << s << "' is not a hex byte\n" << end();
    return '\0';
  }
  return static_cast<uint8_t>(result);
}

void test_number_too_large() {
  Hide_errors = true;
  parse_and_load(
      "== 0x1\n"
      "05 cab\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: token 'cab' is not a hex byte\n"
  );
}

void test_invalid_hex() {
  Hide_errors = true;
  parse_and_load(
      "== 0x1\n"
      "05 cx\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: token 'cx' is not a hex byte\n"
  );
}

void test_negative_number() {
  parse_and_load(
      "== 0x1\n"
      "05 -12\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

void test_negative_number_too_small() {
  Hide_errors = true;
  parse_and_load(
      "== 0x1\n"
      "05 -12345\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: token '-12345' is not a hex byte\n"
  );
}

void test_hex_prefix() {
  parse_and_load(
      "== 0x1\n"
      "0x05 -0x12\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

void parse_and_load(const string& text_bytes) {
  program p;
  istringstream in(text_bytes);
  parse(in, p);
  if (trace_contains_errors()) return;  // if any stage raises errors, stop immediately
  load(p);
}


// read a 32-bit int in little-endian order from the instruction stream
int32_t next32() {
  int32_t result = read_mem_i32(EIP);
  EIP+=4;
  return result;
}


string to_string(const word& w) {
  ostringstream out;
  out << w.data;
  for (int i = 0;  i < SIZE(w.metadata);  ++i)
    out << " /" << w.metadata.at(i);
  return out.str();
}

int32_t parse_int(const string& s) {
  if (s.empty()) return 0;
  istringstream in(s);
  in >> std::hex;
  if (s.at(0) == '-') {
    int32_t result = 0;
    in >> result;
    if (!in || !in.eof()) {
      raise << "not a number: " << s << '\n' << end();
      return 0;
    }
    return result;
  }
  uint32_t uresult = 0;
  in >> uresult;
  if (!in || !in.eof()) {
    raise << "not a number: " << s << '\n' << end();
    return 0;
  }
  return static_cast<int32_t>(uresult);
}

void load_elf(const string& filename, int argc, char* argv[]) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd < 0) raise << filename.c_str() << ": open" << perr() << '\n' << die();
  off_t size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  uint8_t* elf_contents = static_cast<uint8_t*>(malloc(size));
  if (elf_contents == NULL) raise << "malloc(" << size << ')' << perr() << '\n' << die();
  ssize_t read_size = read(fd, elf_contents, size);
  if (size != read_size) raise << "read → " << size << " (!= " << read_size << ')' << perr() << '\n' << die();
  load_elf_contents(elf_contents, size, argc, argv);
  free(elf_contents);
}

void load_elf_contents(uint8_t* elf_contents, size_t size, int argc, char* argv[]) {
  uint8_t magic[5] = {0};
  memcpy(magic, elf_contents, 4);
  if (memcmp(magic, "\177ELF", 4) != 0)
    raise << "Invalid ELF file; starts with \"" << magic << '"' << die();
  if (elf_contents[4] != 1)
    raise << "Only 32-bit ELF files (4-byte words; virtual addresses up to 4GB) supported.\n" << die();
  if (elf_contents[5] != 1)
    raise << "Only little-endian ELF files supported.\n" << die();
  // unused: remaining 10 bytes of e_ident
  uint32_t e_machine_type = u32_in(&elf_contents[16]);
  if (e_machine_type != 0x00030002)
    raise << "ELF type/machine 0x" << HEXWORD << e_machine_type << " isn't i386 executable\n" << die();
  // unused: e_version. We only support version 1, and later versions will be backwards compatible.
  uint32_t e_entry = u32_in(&elf_contents[24]);
  uint32_t e_phoff = u32_in(&elf_contents[28]);
  // unused: e_shoff
  // unused: e_flags
  uint32_t e_ehsize = u16_in(&elf_contents[40]);
  if (e_ehsize < 52) raise << "Invalid binary; ELF header too small\n" << die();
  uint32_t e_phentsize = u16_in(&elf_contents[42]);
  uint32_t e_phnum = u16_in(&elf_contents[44]);
  trace(90, "load") << e_phnum << " entries in the program header, each " << e_phentsize << " bytes long" << end();
  // unused: e_shentsize
  // unused: e_shnum
  // unused: e_shstrndx

  set<uint32_t> overlap;  // to detect overlapping segments
  for (size_t i = 0;  i < e_phnum;  ++i)
    load_segment_from_program_header(elf_contents, i, size, e_phoff + i*e_phentsize, e_ehsize, overlap);

  // initialize code and stack
  assert(overlap.find(STACK_SEGMENT) == overlap.end());
  Mem.push_back(vma(STACK_SEGMENT));
  assert(overlap.find(AFTER_STACK) == overlap.end());
  Reg[ESP].u = AFTER_STACK;
  Reg[EBP].u = 0;
  EIP = e_entry;

  // initialize args on stack
  // no envp for now
  // we wastefully use a separate page of memory for argv
  Mem.push_back(vma(ARGV_DATA_SEGMENT));
  uint32_t argv_data = ARGV_DATA_SEGMENT;
  for (int i = argc-1;  i >= /*skip 'subx_bin' and 'run'*/2;  --i) {
    push(argv_data);
    for (size_t j = 0;  j <= strlen(argv[i]);  ++j) {
      assert(overlap.find(argv_data) == overlap.end());  // don't bother comparing ARGV and STACK
      write_mem_u8(argv_data, argv[i][j]);
      argv_data += sizeof(char);
      assert(argv_data < ARGV_DATA_SEGMENT + SEGMENT_ALIGNMENT);
    }
  }
  push(argc-/*skip 'subx_bin' and 'run'*/2);
}

void push(uint32_t val) {
  Reg[ESP].u -= 4;
  trace(Callstack_depth+1, "run") << "decrementing ESP to 0x" << HEXWORD << Reg[ESP].u << end();
  trace(Callstack_depth+1, "run") << "pushing value 0x" << HEXWORD << val << end();
  write_mem_u32(Reg[ESP].u, val);
}

void load_segment_from_program_header(uint8_t* elf_contents, int segment_index, size_t size, uint32_t offset, uint32_t e_ehsize, set<uint32_t>& overlap) {
  uint32_t p_type = u32_in(&elf_contents[offset]);
  trace(90, "load") << "program header at offset " << offset << ": type " << p_type << end();
  if (p_type != 1) {
    trace(90, "load") << "ignoring segment at offset " << offset << " of non PT_LOAD type " << p_type << " (see http://refspecs.linuxbase.org/elf/elf.pdf)" << end();
    return;
  }
  uint32_t p_offset = u32_in(&elf_contents[offset + 4]);
  uint32_t p_vaddr = u32_in(&elf_contents[offset + 8]);
  if (e_ehsize > p_vaddr) raise << "Invalid binary; program header overlaps ELF header\n" << die();
  // unused: p_paddr
  uint32_t p_filesz = u32_in(&elf_contents[offset + 16]);
  uint32_t p_memsz = u32_in(&elf_contents[offset + 20]);
  if (p_filesz != p_memsz)
    raise << "Can't yet handle segments where p_filesz != p_memsz (see http://refspecs.linuxbase.org/elf/elf.pdf)\n" << die();

  if (p_offset + p_filesz > size)
    raise << "Invalid binary; segment at offset " << offset << " is too large: wants to end at " << p_offset+p_filesz << " but the file ends at " << size << '\n' << die();
  if (p_memsz >= SEGMENT_ALIGNMENT) {
    raise << "Code segment too small for SubX; for now please manually increase SEGMENT_ALIGNMENT.\n" << end();
    return;
  }
  trace(90, "load") << "blitting file offsets (" << p_offset << ", " << (p_offset+p_filesz) << ") to addresses (" << p_vaddr << ", " << (p_vaddr+p_memsz) << ')' << end();
  if (size > p_memsz) size = p_memsz;
  Mem.push_back(vma(p_vaddr));
  for (size_t i = 0;  i < p_filesz;  ++i) {
    assert(overlap.find(p_vaddr+i) == overlap.end());
    write_mem_u8(p_vaddr+i, elf_contents[p_offset+i]);
    overlap.insert(p_vaddr+i);
  }
  if (segment_index == 0 && End_of_program < p_vaddr+p_memsz)
    End_of_program = p_vaddr+p_memsz;
}

void dump_stack() {
  ostringstream out;
  trace(Callstack_depth+1, "run") << "stack:" << end();
  for (uint32_t a = AFTER_STACK-4;  a > Reg[ESP].u;  a -= 4)
    trace(Callstack_depth+2, "run") << "  0x" << HEXWORD << a << " => 0x" << HEXWORD << read_mem_u32(a) << end();
  trace(Callstack_depth+2, "run") << "  0x" << HEXWORD << Reg[ESP].u << " => 0x" << HEXWORD << read_mem_u32(Reg[ESP].u) << "  <=== ESP" << end();
  for (uint32_t a = Reg[ESP].u-4;  a > Reg[ESP].u-40;  a -= 4)
    trace(Callstack_depth+2, "run") << "  0x" << HEXWORD << a << " => 0x" << HEXWORD << read_mem_u32(a) << end();
}

inline uint32_t u32_in(uint8_t* p) {
  return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}

inline uint16_t u16_in(uint8_t* p) {
  return p[0] | p[1] << 8;
}

ostream& operator<<(ostream& os, perr /*unused*/) {
  if (errno)
    os << ": " << strerror(errno);
  return os;
}

ostream& operator<<(ostream& /*unused*/, die /*unused*/) {
  if (Trace_stream) Trace_stream->newline();
  exit(1);
}


void test_add_r32_to_r32() {
  Reg[EAX].i = 0x10;
  Reg[EBX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     d8                                    \n" // add EBX to EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0x00000011\n"
  );
}

// Implement tables 2-2 and 2-3 in the Intel manual, Volume 2.
// We return a pointer so that instructions can write to multiple bytes in
// 'Mem' at once.
// beware: will eventually have side-effects
int32_t* effective_address(uint8_t modrm) {
  const uint8_t mod = (modrm>>6);
  // ignore middle 3 'reg opcode' bits
  const uint8_t rm = modrm & 0x7;
  if (mod == 3) {
    // mod 3 is just register direct addressing
    trace(Callstack_depth+1, "run") << "r/m32 is " << rname(rm) << end();
    return &Reg[rm].i;
  }
  uint32_t addr = effective_address_number(modrm);
  trace(Callstack_depth+1, "run") << "effective address contains " << read_mem_i32(addr) << end();
  return mem_addr_i32(addr);
}

// beware: will eventually have side-effects
uint32_t effective_address_number(uint8_t modrm) {
  const uint8_t mod = (modrm>>6);
  // ignore middle 3 'reg opcode' bits
  const uint8_t rm = modrm & 0x7;
  uint32_t addr = 0;
  switch (mod) {
  case 3:
    // mod 3 is just register direct addressing
    raise << "unexpected direct addressing mode\n" << end();
    return 0;
  case 0:  // indirect addressing
    switch (rm) {
    default:  // address in register
      trace(Callstack_depth+1, "run") << "effective address is 0x" << HEXWORD << Reg[rm].u << " (" << rname(rm) << ")" << end();
      addr = Reg[rm].u;
      break;
    case 5:  // exception: mod 0b00 rm 0b101 => incoming disp32
      addr = next32();
      trace(Callstack_depth+1, "run") << "effective address is 0x" << HEXWORD << addr << " (disp32)" << end();
      break;


    // End Mod 0 Special-cases(addr)
    }
    break;


  case 1:  // indirect + disp8 addressing
    switch (rm) {
    default:
      addr = Reg[rm].u;
      trace(Callstack_depth+1, "run") << "effective address is initially 0x" << HEXWORD << addr << " (" << rname(rm) << ")" << end();
      break;
    // End Mod 1 Special-cases(addr)
    }
    if (addr > 0) {
      addr += static_cast<int8_t>(next());
      trace(Callstack_depth+1, "run") << "effective address is 0x" << HEXWORD << addr << " (after adding disp8)" << end();
    }
    break;

  case 2:  // indirect + disp32 addressing
    switch (rm) {
    default:
      addr = Reg[rm].u;
      trace(Callstack_depth+1, "run") << "effective address is initially 0x" << HEXWORD << addr << " (" << rname(rm) << ")" << end();
      break;
    // End Mod 2 Special-cases(addr)
    }
    if (addr > 0) {
      addr += next32();
      trace(Callstack_depth+1, "run") << "effective address is 0x" << HEXWORD << addr << " (after adding disp32)" << end();
    }
    break;

  // End Mod Special-cases(addr)
  default:
    cerr << "unrecognized mod bits: " << NUM(mod) << '\n';
    exit(1);
  }
  // Found effective_address(addr)
  return addr;
}

string rname(uint8_t r) {
  switch (r) {
  case 0: return "EAX";
  case 1: return "ECX";
  case 2: return "EDX";
  case 3: return "EBX";
  case 4: return "ESP";
  case 5: return "EBP";
  case 6: return "ESI";
  case 7: return "EDI";
  default: raise << "invalid register " << r << '\n' << end();  return "";
  }
}


void test_subtract_r32_from_r32() {
  Reg[EAX].i = 10;
  Reg[EBX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  29     d8                                    \n"  // subtract EBX from EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: subtract EBX from r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0x00000009\n"
  );
}

void test_multiply_eax_by_r32() {
  Reg[EAX].i = 4;
  Reg[ECX].i = 3;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  f7     e1                                    \n"  // multiply EAX by ECX
      // ModR/M in binary: 11 (direct mode) 100 (subop mul) 001 (src ECX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is ECX\n"
      "run: subop: multiply EAX by r/m32\n"
      "run: storing 0x0000000c\n"
  );
}

void test_multiply_r32_into_r32() {
  Reg[EAX].i = 4;
  Reg[EBX].i = 2;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f af  d8                                    \n"  // subtract EBX into EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: multiply r/m32 into EBX\n"
      "run: r/m32 is EAX\n"
      "run: storing 0x00000008\n"
  );
}

void test_negate_r32() {
  Reg[EBX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  f7 db                                        \n"  // negate EBX
      // ModR/M in binary: 11 (direct mode) 011 (subop negate) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: negate\n"
      "run: storing 0xffffffff\n"
  );
}

// negate can overflow in exactly one situation
void test_negate_can_overflow() {
  Reg[EBX].i = 0x80000000;  // INT_MIN
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  f7 db                                        \n"  // negate EBX
      // ModR/M in binary: 11 (direct mode) 011 (subop negate) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: negate\n"
      "run: overflow\n"
  );
}


void test_shift_left_r32_with_cl() {
  Reg[EBX].i = 13;
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     e3                                    \n"  // shift EBX left by CL bits
      // ModR/M in binary: 11 (direct mode) 100 (subop shift left) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift left by CL bits\n"
      "run: storing 0x0000001a\n"
  );
}

void test_shift_right_arithmetic_r32_with_cl() {
  Reg[EBX].i = 26;
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     fb                                    \n"  // shift EBX right by CL bits, while preserving sign
      // ModR/M in binary: 11 (direct mode) 111 (subop shift right arithmetic) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while preserving sign\n"
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_arithmetic_odd_r32_with_cl() {
  Reg[EBX].i = 27;
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     fb                                    \n"  // shift EBX right by CL bits, while preserving sign
      // ModR/M in binary: 11 (direct mode) 111 (subop shift right arithmetic) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while preserving sign\n"
      // result: 13
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_arithmetic_negative_r32_with_cl() {
  Reg[EBX].i = 0xfffffffd;  // -3
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     fb                                    \n"  // shift EBX right by CL bits, while preserving sign
      // ModR/M in binary: 11 (direct mode) 111 (subop shift right arithmetic) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while preserving sign\n"
      // result: -2
      "run: storing 0xfffffffe\n"
  );
}


void test_shift_right_logical_r32_with_cl() {
  Reg[EBX].i = 26;
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     eb                                    \n"  // shift EBX right by CL bits, while padding zeroes
      // ModR/M in binary: 11 (direct mode) 101 (subop shift right logical) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while padding zeroes\n"
      // result: 13
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_logical_odd_r32_with_cl() {
  Reg[EBX].i = 27;
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     eb                                    \n"  // shift EBX right by CL bits, while padding zeroes
      // ModR/M in binary: 11 (direct mode) 101 (subop shift right logical) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while padding zeroes\n"
      // result: 13
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_logical_negative_r32_with_cl() {
  Reg[EBX].i = 0xfffffffd;
  Reg[ECX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  d3     eb                                    \n"  // shift EBX right by CL bits, while padding zeroes
      // ModR/M in binary: 11 (direct mode) 101 (subop shift right logical) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while padding zeroes\n"
      "run: storing 0x7ffffffe\n"
  );
}


void test_and_r32_with_r32() {
  Reg[EAX].i = 0x0a0b0c0d;
  Reg[EBX].i = 0x000000ff;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  21     d8                                    \n"  // and EBX with destination EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: and EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0x0000000d\n"
  );
}

void test_or_r32_with_r32() {
  Reg[EAX].i = 0x0a0b0c0d;
  Reg[EBX].i = 0xa0b0c0d0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  09     d8                                    \n"  // or EBX with destination EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: or EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0xaabbccdd\n"
  );
}

void test_xor_r32_with_r32() {
  Reg[EAX].i = 0x0a0b0c0d;
  Reg[EBX].i = 0xaabbc0d0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  31     d8                                    \n"  // xor EBX with destination EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: xor EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0xa0b0ccdd\n"
  );
}

void test_not_r32() {
  Reg[EBX].i = 0x0f0f00ff;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  f7     d3                                    \n"  // not EBX
      // ModR/M in binary: 11 (direct mode) 010 (subop not) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: not\n"
      "run: storing 0xf0f0ff00\n"
  );
}

void test_compare_r32_with_r32_greater() {
  Reg[EAX].i = 0x0a0b0c0d;
  Reg[EBX].i = 0x0a0b0c07;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  39     d8                                    \n"  // compare EBX with EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: SF=0; ZF=0; OF=0\n"
  );
}

void test_compare_r32_with_r32_lesser() {
  Reg[EAX].i = 0x0a0b0c07;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  39     d8                                    \n"  // compare EBX with EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: SF=1; ZF=0; OF=0\n"
  );
}

void test_compare_r32_with_r32_equal() {
  Reg[EAX].i = 0x0a0b0c0d;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  39     d8                                    \n"  // compare EBX with EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: SF=0; ZF=1; OF=0\n"
  );
}


void test_copy_r32_to_r32() {
  Reg[EBX].i = 0xaf;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  89     d8                                    \n"  // copy EBX to EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: copy EBX to r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0x000000af\n"
  );
}

void test_xchg_r32_with_r32() {
  Reg[EBX].i = 0xaf;
  Reg[EAX].i = 0x2e;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  87     d8                                    \n"  // exchange EBX with EAX
      // ModR/M in binary: 11 (direct mode) 011 (src EBX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: exchange EBX with r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing 0x000000af in r/m32\n"
      "run: storing 0x0000002e in EBX\n"
  );
}

void test_increment_r32() {
  Reg[ECX].u = 0x1f;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  41                                           \n"  // increment ECX
  );
  CHECK_TRACE_CONTENTS(
      "run: increment ECX\n"
      "run: storing value 0x00000020\n"
  );
}

void test_increment_rm32() {
  Reg[EAX].u = 0x20;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  ff     c0                                    \n"  // increment EAX
      // ModR/M in binary: 11 (direct mode) 000 (subop inc) 000 (EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: increment r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing value 0x00000021\n"
  );
}

void test_decrement_r32() {
  Reg[ECX].u = 0x1f;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  49                                           \n"  // decrement ECX
  );
  CHECK_TRACE_CONTENTS(
      "run: decrement ECX\n"
      "run: storing value 0x0000001e\n"
  );
}

void test_decrement_rm32() {
  Reg[EAX].u = 0x20;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  ff     c8                                    \n"  // decrement EAX
      // ModR/M in binary: 11 (direct mode) 001 (subop inc) 000 (EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: decrement r/m32\n"
      "run: r/m32 is EAX\n"
      "run: storing value 0x0000001f\n"
  );
}

void test_push_r32() {
  Reg[ESP].u = 0x64;
  Reg[EBX].i = 0x0000000a;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  53                                           \n"  // push EBX to stack
  );
  CHECK_TRACE_CONTENTS(
      "run: push EBX\n"
      "run: decrementing ESP to 0x00000060\n"
      "run: pushing value 0x0000000a\n"
  );
}

void test_pop_r32() {
  Reg[ESP].u = 0x02000000;
  Mem.push_back(vma(0x02000000));  // manually allocate memory
  write_mem_i32(0x02000000, 0x0000000a);  // ..before this write
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  5b                                           \n"  // pop stack to EBX
      "== 0x2000\n"  // data segment
      "0a 00 00 00\n"  // 0x0000000a
  );
  CHECK_TRACE_CONTENTS(
      "run: pop into EBX\n"
      "run: popping value 0x0000000a\n"
      "run: incrementing ESP to 0x02000004\n"
  );
}

uint32_t pop() {
  const uint32_t result = read_mem_u32(Reg[ESP].u);
  trace(Callstack_depth+1, "run") << "popping value 0x" << HEXWORD << result << end();
  Reg[ESP].u += 4;
  trace(Callstack_depth+1, "run") << "incrementing ESP to 0x" << HEXWORD << Reg[ESP].u << end();
  return result;
}


void test_add_r32_to_mem_at_r32() {
  Reg[EBX].i = 0x10;
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01  18                                       \n"  // add EBX to *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x00000011\n"
  );
}

void test_add_mem_at_r32_to_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x10;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  03  18                                       \n"  // add *EAX to EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add r/m32 to EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x00000011\n"
  );
}

void test_subtract_r32_from_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  29  18                                       \n"  // subtract EBX from *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0a 00 00 00\n"  // 0x0000000a
  );
  CHECK_TRACE_CONTENTS(
      "run: subtract EBX from r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x00000009\n"
  );
}


void test_subtract_mem_at_r32_from_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 10;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  2b     18                                    \n"  // subtract *EAX from EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: subtract r/m32 from EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x00000009\n"
  );
}

void test_and_r32_with_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0xff;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  21     18                                    \n"  // and EBX with *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: and EBX with r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x0000000d\n"
  );
}


void test_and_mem_at_r32_with_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  23     18                                    \n"  // and *EAX with EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "ff 00 00 00\n"  // 0x000000ff
  );
  CHECK_TRACE_CONTENTS(
      "run: and r/m32 with EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x0000000d\n"
  );
}

void test_or_r32_with_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0xa0b0c0d0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  09  18                                      #\n"  // EBX with *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: or EBX with r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0xaabbccdd\n"
  );
}


void test_or_mem_at_r32_with_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0xa0b0c0d0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0b     18                                    \n"  // or *EAX with EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: or r/m32 with EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0xaabbccdd\n"
  );
}

void test_xor_r32_with_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0xa0b0c0d0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  31     18                                    \n"  // xor EBX with *EAX
      "== 0x2000\n"  // data segment
      "0d 0c bb aa\n"  // 0xaabb0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: xor EBX with r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x0a0bccdd\n"
  );
}


void test_xor_mem_at_r32_with_r32() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0xa0b0c0d0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  33     18                                    \n"  // xor *EAX with EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: xor r/m32 with EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0xaabbccdd\n"
  );
}

void test_not_of_mem_at_r32() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  f7     13                                    \n"  // not *EBX
      // ModR/M in binary: 00 (indirect mode) 010 (subop not) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "ff 00 0f 0f\n"  // 0x0f0f00ff
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: subop: not\n"
      "run: storing 0xf0f0ff00\n"
  );
}


void test_compare_mem_at_r32_with_r32_greater() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c07;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  39     18                                    \n"  // compare EBX with *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EBX with r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: SF=0; ZF=0; OF=0\n"
  );
}

void test_compare_mem_at_r32_with_r32_lesser() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  39     18                                    \n"  // compare EBX with *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "07 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EBX with r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: SF=1; ZF=0; OF=0\n"
  );
}

void test_compare_mem_at_r32_with_r32_equal() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  39     18                                    \n"  // compare EBX with *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EBX with r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: SF=0; ZF=1; OF=0\n"
  );
}


void test_compare_r32_with_mem_at_r32_greater() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  3b     18                                    \n"  // compare *EAX with EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "07 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: compare r/m32 with EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: SF=0; ZF=0; OF=0\n"
  );
}

void test_compare_r32_with_mem_at_r32_lesser() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c07;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  3b     18                                    \n"  // compare *EAX with EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: compare r/m32 with EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: SF=1; ZF=0; OF=0\n"
  );
}

void test_compare_r32_with_mem_at_r32_equal() {
  Reg[EAX].i = 0x2000;
  Reg[EBX].i = 0x0a0b0c0d;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  3b     18                                    \n"  // compare *EAX with EBX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "0d 0c 0b 0a\n"  // 0x0a0b0c0d
  );
  CHECK_TRACE_CONTENTS(
      "run: compare r/m32 with EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: SF=0; ZF=1; OF=0\n"
  );
}


void test_copy_r32_to_mem_at_r32() {
  Reg[EBX].i = 0xaf;
  Reg[EAX].i = 0x60;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  89     18                                    \n"  // copy EBX to *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EAX) 000 (dest EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: copy EBX to r/m32\n"
      "run: effective address is 0x00000060 (EAX)\n"
      "run: storing 0x000000af\n"
  );
}


void test_copy_mem_at_r32_to_r32() {
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  8b     18                                    \n"  // copy *EAX to EBX
      "== 0x2000\n"  // data segment
      "af 00 00 00\n"  // 0x000000af
  );
  CHECK_TRACE_CONTENTS(
      "run: copy r/m32 to EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0x000000af\n"
  );
}

void test_jump_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  ff     20                                    \n"  // jump to *EAX
      // ModR/M in binary: 00 (indirect mode) 100 (jump to r/m32) 000 (src EAX)
      "  05                                 00 00 00 01\n"
      "  05                                 00 00 00 02\n"
      "== 0x2000\n"  // data segment
      "08 00 00 00\n"  // 0x00000008
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: ff\n"
      "run: jump to r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: jumping to 0x00000008\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_push_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  Reg[ESP].u = 0x14;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  ff     30                                    \n"  // push *EAX to stack
      "== 0x2000\n"  // data segment
      "af 00 00 00\n"  // 0x000000af
  );
  CHECK_TRACE_CONTENTS(
      "run: push r/m32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: decrementing ESP to 0x00000010\n"
      "run: pushing value 0x000000af\n"
  );
}

void test_pop_mem_at_r32() {
  Reg[EAX].i = 0x60;
  Reg[ESP].u = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  8f     00                                    \n"  // pop stack into *EAX
      // ModR/M in binary: 00 (indirect mode) 000 (pop r/m32) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "30 00 00 00\n"  // 0x00000030
  );
  CHECK_TRACE_CONTENTS(
      "run: pop into r/m32\n"
      "run: effective address is 0x00000060 (EAX)\n"
      "run: popping value 0x00000030\n"
      "run: incrementing ESP to 0x00002004\n"
  );
}

void test_add_r32_to_mem_at_displacement() {
  Reg[EBX].i = 0x10;  // source
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     1d            00 20 00 00             \n"  // add EBX to *0x2000
      // ModR/M in binary: 00 (indirect mode) 011 (src EBX) 101 (dest in disp32)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is 0x00002000 (disp32)\n"
      "run: storing 0x00000011\n"
  );
}

void test_add_r32_to_mem_at_r32_plus_disp8() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x1ffe;  // dest
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     58            02                      \n"  // add EBX to *(EAX+2)
      // ModR/M in binary: 01 (indirect+disp8 mode) 011 (src EBX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00001ffe (EAX)\n"
      "run: effective address is 0x00002000 (after adding disp8)\n"
      "run: storing 0x00000011\n"
  );
}

void test_add_r32_to_mem_at_r32_plus_negative_disp8() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x2001;  // dest
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     58            ff                      \n"  // add EBX to *(EAX-1)
      // ModR/M in binary: 01 (indirect+disp8 mode) 011 (src EBX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00002001 (EAX)\n"
      "run: effective address is 0x00002000 (after adding disp8)\n"
      "run: storing 0x00000011\n"
  );
}


void test_add_r32_to_mem_at_r32_plus_disp32() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x1ffe;  // dest
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     98            02 00 00 00             \n"  // add EBX to *(EAX+2)
      // ModR/M in binary: 10 (indirect+disp32 mode) 011 (src EBX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00001ffe (EAX)\n"
      "run: effective address is 0x00002000 (after adding disp32)\n"
      "run: storing 0x00000011\n"
  );
}

void test_add_r32_to_mem_at_r32_plus_negative_disp32() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x2001;  // dest
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     98            ff ff ff ff             \n"  // add EBX to *(EAX-1)
      // ModR/M in binary: 10 (indirect+disp32 mode) 011 (src EBX) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00002001 (EAX)\n"
      "run: effective address is 0x00002000 (after adding disp32)\n"
      "run: storing 0x00000011\n"
  );
}


void test_copy_address() {
  Reg[EAX].u = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  8d     18                                    \n"  // copy address in EAX into EBX
      // ModR/M in binary: 00 (indirect mode) 011 (dest EBX) 000 (src EAX)
  );
  CHECK_TRACE_CONTENTS(
      "run: copy address into EBX\n"
      "run: effective address is 0x00002000 (EAX)\n"
  );
}

