
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

using std::ios;

#include<cctype>

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
enum expected_operand_type {
  // start from the least significant bit
  MODRM,  // more complex, may also involve disp8 or disp32
  SUBOP,
  DISP8,
  DISP16,
  DISP32,
  IMM8,
  IMM32,
  NUM_OPERAND_TYPES
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
vector<transform_fn> Transform;

uint32_t Next_segment = 0xb0000000;  // 0xc0000000 and up is reserved for Linux kernel
const uint32_t SPACE_FOR_SEGMENT = 0x01000000;
set<string> Instruction_operands;
vector<string> Operand_type_name;
map<string, expected_operand_type> Operand_type;
map</*op*/string, /*bitvector*/uint8_t> Permitted_operands;
const uint8_t INVALID_OPERANDS = 0xff;  // no instruction uses all the operand types
map</*op*/string, /*bitvector*/uint8_t> Permitted_operands_0f;
map<string, uint32_t> Operand_bound;
map</*name*/string, int> Segment_index;
bool Currently_parsing_named_segment = false;  // global to permit cross-layer communication
int Currently_parsing_segment_index = -1;  // global to permit cross-layer communication
uint32_t Entry_address = 0;
bool Dump_map = false;  // currently used only by 'subx translate'
ofstream Map_file;
int Next_auto_global = 1;
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
  // Begin Transforms
  // Begin Level-3 Transforms
  Transform.push_back(transform_literal_strings);
  // End Level-3 Transforms

  // Begin Level-2 Transforms
  Transform.push_back(correlate_disp32_with_mod);
  Transform.push_back(pack_operands);
  Transform.push_back(compute_segment_starts);

  Transform.push_back(rewrite_labels);
  Transform.push_back(rewrite_global_variables);
  // End Level-2 Transforms

  // End Transforms

  Instruction_operands.insert("subop");
  Instruction_operands.insert("mod");
  Instruction_operands.insert("rm32");
  Instruction_operands.insert("base");
  Instruction_operands.insert("index");
  Instruction_operands.insert("scale");
  Instruction_operands.insert("r32");
  Instruction_operands.insert("disp8");
  Instruction_operands.insert("disp16");
  Instruction_operands.insert("disp32");
  Instruction_operands.insert("imm8");
  Instruction_operands.insert("imm32");

  init_op_types();
  init_permitted_operands();
  put_new(Operand_bound, "subop", 1<<3);
  put_new(Operand_bound, "mod", 1<<2);
  put_new(Operand_bound, "rm32", 1<<3);
  put_new(Operand_bound, "base", 1<<3);
  put_new(Operand_bound, "index", 1<<3);
  put_new(Operand_bound, "scale", 1<<2);
  put_new(Operand_bound, "r32", 1<<3);
  put_new(Operand_bound, "disp8", 1<<8);
  put_new(Operand_bound, "disp16", 1<<16);
  // no bound needed for disp32
  put_new(Operand_bound, "imm8", 1<<8);
  // no bound needed for imm32

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
    else if (is_equal(*arg, "--map")) {
      Dump_map = true;
      // End --map Settings
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

  if (is_equal(argv[1], "translate")) {
    START_TRACING_UNTIL_END_OF_SCOPE;
    reset();
    // Begin subx translate
    if (Dump_map)
      Map_file.open("map");
    program p;
    string output_filename;
    for (int i = /*skip 'subx translate'*/2;  i < argc;  ++i) {
      if (is_equal(argv[i], "-o")) {
        ++i;
        if (i >= argc) {
          print_translate_usage();
          cerr << "'-o' must be followed by a filename to write results to\n";
          exit(1);
        }
        output_filename = argv[i];
      }
      else {
        trace(2, "parse") << argv[i] << end();
        ifstream fin(argv[i]);
        if (!fin) {
          cerr << "could not open " << argv[i] << '\n';
          return 1;
        }
        parse(fin, p);
        if (trace_contains_errors()) return 1;
      }
    }
    if (p.segments.empty()) {
      print_translate_usage();
      cerr << "nothing to do; must provide at least one file to read\n";
      exit(1);
    }
    if (output_filename.empty()) {
      print_translate_usage();
      cerr << "must provide a filename to write to using '-o'\n";
      exit(1);
    }
    trace(2, "transform") << "begin" << end();
    transform(p);
    if (trace_contains_errors()) return 1;
    trace(2, "translate") << "begin" << end();
    save_elf(p, output_filename);
    if (trace_contains_errors()) {
      unlink(output_filename.c_str());
      return 1;
    }
    if (Dump_map)
      Map_file.close();

    // End subx translate
    return 0;
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

void test_preserve_metadata_when_emitting_single_byte() {
  word in;
  in.data = "f0";
  in.original = "f0/foo";
  line out;
  emit_hex_bytes(out, in, 1);
  CHECK_EQ(out.words.at(0).data, "f0");
  CHECK_EQ(out.words.at(0).original, "f0/foo");
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

  Segment_index.clear();
  Currently_parsing_named_segment = false;
  Currently_parsing_segment_index = -1;

  Entry_address = 0;
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

  cerr << "  instructions\n";

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
  put_new(Help, "instructions",
    "Each x86 instruction consists of an instruction or opcode and some number\n"
    "of operands.\n"
    "Each operand has a type. An instruction won't have more than one operand of\n"
    "any type.\n"
    "Each instruction has some set of allowed operand types. It'll reject others.\n"
    "The complete list of operand types: mod, subop, r32 (register), rm32\n"
    "(register or memory), scale, index, base, disp8, disp16, disp32, imm8,\n"
    "imm32.\n"
    "Each of these has its own help page. Try reading 'subx help mod' next.\n"
  );
  init_operand_type_help();
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


      case 2: {  // call function pointer at r/m32
        trace(Callstack_depth+1, "run") << "call to r/m32" << end();
        const int32_t* offset = effective_address(modrm);
        push(EIP);
        EIP += *offset;
        trace(Callstack_depth+1, "run") << "jumping to 0x" << HEXWORD << EIP << end();
        ++Callstack_depth;
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

  case 0x81: {  // combine imm32 with r/m32
    trace(Callstack_depth+1, "run") << "combine imm32 with r/m32" << end();
    const uint8_t modrm = next();
    int32_t* arg1 = effective_address(modrm);
    const int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "imm32 is 0x" << HEXWORD << arg2 << end();
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    switch (subop) {
    case 0:
      trace(Callstack_depth+1, "run") << "subop add" << end();
      BINARY_ARITHMETIC_OP(+, *arg1, arg2);
      break;
    case 5: {
      trace(Callstack_depth+1, "run") << "subop subtract" << end();
      BINARY_ARITHMETIC_OP(-, *arg1, arg2);
      break;
    }


    case 4: {
      trace(Callstack_depth+1, "run") << "subop and" << end();
      BINARY_BITWISE_OP(&, *arg1, arg2);
      break;
    }


    case 1: {
      trace(Callstack_depth+1, "run") << "subop or" << end();
      BINARY_BITWISE_OP(|, *arg1, arg2);
      break;
    }

    case 6: {
      trace(Callstack_depth+1, "run") << "subop xor" << end();
      BINARY_BITWISE_OP(^, *arg1, arg2);
      break;
    }

    case 7: {
      trace(Callstack_depth+1, "run") << "subop compare" << end();
      const int32_t tmp1 = *arg1 - arg2;
      SF = (tmp1 < 0);
      ZF = (tmp1 == 0);
      const int64_t tmp2 = *arg1 - arg2;
      OF = (tmp1 != tmp2);
      trace(Callstack_depth+1, "run") << "SF=" << SF << "; ZF=" << ZF << "; OF=" << OF << end();
      break;
    }

    // End Op 81 Subops
    default:
      cerr << "unrecognized subop for opcode 81: " << NUM(subop) << '\n';
      exit(1);
    }
    break;
  }


  case 0x2d: {  // subtract imm32 from EAX
    const int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "subtract imm32 0x" << HEXWORD << arg2 << " from EAX" << end();
    BINARY_ARITHMETIC_OP(-, Reg[EAX].i, arg2);
    break;
  }


  case 0xc1: {
    const uint8_t modrm = next();
    trace(Callstack_depth+1, "run") << "operate on r/m32" << end();
    int32_t* arg1 = effective_address(modrm);
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    switch (subop) {
    case 4: {  // shift left r/m32 by CL
      trace(Callstack_depth+1, "run") << "subop: shift left by CL bits" << end();
      uint8_t count = next() & 0x1f;
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
      uint8_t count = next() & 0x1f;
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
      uint8_t count = next() & 0x1f;
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

    // End Op c1 Subops
    default:
      cerr << "unrecognized subop for opcode c1: " << NUM(subop) << '\n';
      exit(1);
    }
    break;
  }


  case 0x25: {  // and imm32 with EAX
    const int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "and imm32 0x" << HEXWORD << arg2 << " with EAX" << end();
    BINARY_BITWISE_OP(&, Reg[EAX].i, arg2);
    break;
  }


  case 0x0d: {  // or imm32 with EAX
    const int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "or imm32 0x" << HEXWORD << arg2 << " with EAX" << end();
    BINARY_BITWISE_OP(|, Reg[EAX].i, arg2);
    break;
  }


  case 0x35: {  // xor imm32 with EAX
    const int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "xor imm32 0x" << HEXWORD << arg2 << " with EAX" << end();
    BINARY_BITWISE_OP(^, Reg[EAX].i, arg2);
    break;
  }


  case 0x3d: {  // compare EAX with imm32
    const int32_t arg1 = Reg[EAX].i;
    const int32_t arg2 = next32();
    trace(Callstack_depth+1, "run") << "compare EAX and imm32 0x" << HEXWORD << arg2 << end();
    const int32_t tmp1 = arg1 - arg2;
    SF = (tmp1 < 0);
    ZF = (tmp1 == 0);
    const int64_t tmp2 = arg1 - arg2;
    OF = (tmp1 != tmp2);
    trace(Callstack_depth+1, "run") << "SF=" << SF << "; ZF=" << ZF << "; OF=" << OF << end();
    break;
  }

  case 0xb8:
  case 0xb9:
  case 0xba:
  case 0xbb:
  case 0xbc:
  case 0xbd:
  case 0xbe:
  case 0xbf: {  // copy imm32 to r32
    const uint8_t rdest = op & 0x7;
    const int32_t src = next32();
    trace(Callstack_depth+1, "run") << "copy imm32 0x" << HEXWORD << src << " to " << rname(rdest) << end();
    Reg[rdest].i = src;
    break;
  }


  case 0xc7: {  // copy imm32 to r32
    const uint8_t modrm = next();
    trace(Callstack_depth+1, "run") << "copy imm32 to r/m32" << end();
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    if (subop != 0) {
      cerr << "unrecognized subop for opcode c7: " << NUM(subop) << " (only 0/copy currently implemented)\n";
      exit(1);
    }
    int32_t* dest = effective_address(modrm);
    const int32_t src = next32();
    trace(Callstack_depth+1, "run") << "imm32 is 0x" << HEXWORD << src << end();
    *dest = src;
    break;
  }


  case 0x68: {
    const uint32_t val = static_cast<uint32_t>(next32());
    trace(Callstack_depth+1, "run") << "push imm32 0x" << HEXWORD << val << end();
  //?   cerr << "push: " << val << " => " << Reg[ESP].u << '\n';
    push(val);
    trace(Callstack_depth+1, "run") << "ESP is now 0x" << HEXWORD << Reg[ESP].u << end();
    trace(Callstack_depth+1, "run") << "contents at ESP: 0x" << HEXWORD << read_mem_u32(Reg[ESP].u) << end();
    break;
  }

  case 0xeb: {  // jump rel8
    int8_t offset = static_cast<int>(next());
    trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
    EIP += offset;
    break;
  }


  case 0x74: {  // jump rel8 if ZF
    const int8_t offset = static_cast<int>(next());
    if (ZF) {
      trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
      EIP += offset;
    }
    break;
  }

  case 0x75: {  // jump rel8 unless ZF
    const int8_t offset = static_cast<int>(next());
    if (!ZF) {
      trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
      EIP += offset;
    }
    break;
  }

  case 0x7f: {  // jump rel8 if !SF and !ZF
    const int8_t offset = static_cast<int>(next());
    if (!ZF && SF == OF) {
      trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
      EIP += offset;
    }
    break;
  }

  case 0x7d: {  // jump rel8 if !SF
    const int8_t offset = static_cast<int>(next());
    if (SF == OF) {
      trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
      EIP += offset;
    }
    break;
  }

  case 0x7c: {  // jump rel8 if SF and !ZF
    const int8_t offset = static_cast<int>(next());
    if (SF != OF) {
      trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
      EIP += offset;
    }
    break;
  }

  case 0x7e: {  // jump rel8 if SF or ZF
    const int8_t offset = static_cast<int>(next());
    if (ZF || SF != OF) {
      trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
      EIP += offset;
    }
    break;
  }

  case 0xe9: {  // jump disp32
    const int32_t offset = next32();
    trace(Callstack_depth+1, "run") << "jump " << offset << end();
    EIP += offset;
    break;
  }


  case 0xe8: {  // call disp32 relative to next EIP
    const int32_t offset = next32();
    ++Callstack_depth;
    trace(Callstack_depth+1, "run") << "call imm32 0x" << HEXWORD << offset << end();
  //?   cerr << "push: EIP: " << EIP << " => " << Reg[ESP].u << '\n';
    push(EIP);
    EIP += offset;
    trace(Callstack_depth+1, "run") << "jumping to 0x" << HEXWORD << EIP << end();
    break;
  }


  case 0xc3: {  // return from a call
    trace(Callstack_depth+1, "run") << "return" << end();
    --Callstack_depth;
    EIP = pop();
    trace(Callstack_depth+1, "run") << "jumping to 0x" << HEXWORD << EIP << end();
    break;
  }

  case 0xcd: {  // int imm8 (software interrupt)
    trace(Callstack_depth+1, "run") << "syscall" << end();
    uint8_t code = next();
    if (code != 0x80) {
      raise << "Unimplemented interrupt code " << HEXBYTE << code << '\n' << end();
      raise << "  Only `int 80h` supported for now.\n" << end();
      break;
    }
    process_int80();
    break;
  }

  case 0x88: {  // copy r8 to r/m8
    const uint8_t modrm = next();
    const uint8_t rsrc = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "copy " << rname_8bit(rsrc) << " to r8/m8-at-r32" << end();
    // use unsigned to zero-extend 8-bit value to 32 bits
    uint8_t* dest = reinterpret_cast<uint8_t*>(effective_byte_address(modrm));
    const uint8_t* src = reg_8bit(rsrc);
    *dest = *src;
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXBYTE << NUM(*dest) << end();
    break;
  }


  case 0x8a: {  // copy r/m8 to r8
    const uint8_t modrm = next();
    const uint8_t rdest = (modrm>>3)&0x7;
    trace(Callstack_depth+1, "run") << "copy r8/m8-at-r32 to " << rname_8bit(rdest) << end();
    // use unsigned to zero-extend 8-bit value to 32 bits
    const uint8_t* src = reinterpret_cast<uint8_t*>(effective_byte_address(modrm));
    uint8_t* dest = reg_8bit(rdest);
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXBYTE << NUM(*src) << end();
    *dest = *src;
    const uint8_t rdest_32bit = rdest & 0x3;
    trace(Callstack_depth+1, "run") << rname(rdest_32bit) << " now contains 0x" << HEXWORD << Reg[rdest_32bit].u << end();
    break;
  }

  case 0xc6: {  // copy imm8 to r/m8
    const uint8_t modrm = next();
    const uint8_t src = next();
    trace(Callstack_depth+1, "run") << "copy imm8 to r8/m8-at-r32" << end();
    trace(Callstack_depth+1, "run") << "imm8 is 0x" << HEXWORD << src << end();
    const uint8_t subop = (modrm>>3)&0x7;  // middle 3 'reg opcode' bits
    if (subop != 0) {
      cerr << "unrecognized subop for opcode c6: " << NUM(subop) << " (only 0/copy currently implemented)\n";
      exit(1);
    }
    // use unsigned to zero-extend 8-bit value to 32 bits
    uint8_t* dest = reinterpret_cast<uint8_t*>(effective_byte_address(modrm));
    *dest = src;
    trace(Callstack_depth+1, "run") << "storing 0x" << HEXBYTE << NUM(*dest) << end();
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


    case 0x84: {  // jump disp32 if ZF
      const int32_t offset = next32();
      if (ZF) {
        trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
        EIP += offset;
      }
      break;
    }

    case 0x85: {  // jump disp32 unless ZF
      const int32_t offset = next32();
      if (!ZF) {
        trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
        EIP += offset;
      }
      break;
    }

    case 0x8f: {  // jump disp32 if !SF and !ZF
      const int32_t offset = next32();
      if (!ZF && SF == OF) {
        trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
        EIP += offset;
      }
      break;
    }

    case 0x8d: {  // jump disp32 if !SF
      const int32_t offset = next32();
      if (SF == OF) {
        trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
        EIP += offset;
      }
      break;
    }

    case 0x8c: {  // jump disp32 if SF and !ZF
      const int32_t offset = next32();
      if (SF != OF) {
        trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
        EIP += offset;
      }
      break;
    }

    case 0x8e: {  // jump disp32 if SF or ZF
      const int32_t offset = next32();
      if (ZF || SF != OF) {
        trace(Callstack_depth+1, "run") << "jump " << NUM(offset) << end();
        EIP += offset;
      }
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

  put_new(Name, "81", "combine rm32 with imm32 based on subop (add/sub/and/or/xor/cmp)");

  put_new(Name, "2d", "subtract imm32 from EAX (sub)");

  put_new(Name, "c1", "shift rm32 by imm8 bits depending on subop (sal/sar/shl/shr)");

  put_new(Name, "25", "EAX = bitwise AND of imm32 with EAX (and)");

  put_new(Name, "0d", "EAX = bitwise OR of imm32 with EAX (or)");

  put_new(Name, "35", "EAX = bitwise XOR of imm32 with EAX (xor)");

  put_new(Name, "3d", "compare: set SF if EAX < imm32 (cmp)");

  put_new(Name, "b8", "copy imm32 to EAX (mov)");
  put_new(Name, "b9", "copy imm32 to ECX (mov)");
  put_new(Name, "ba", "copy imm32 to EDX (mov)");
  put_new(Name, "bb", "copy imm32 to EBX (mov)");
  put_new(Name, "bc", "copy imm32 to ESP (mov)");
  put_new(Name, "bd", "copy imm32 to EBP (mov)");
  put_new(Name, "be", "copy imm32 to ESI (mov)");
  put_new(Name, "bf", "copy imm32 to EDI (mov)");

  put_new(Name, "c7", "copy imm32 to rm32 (mov)");

  put_new(Name, "68", "push imm32 to stack (push)");

  put_new(Name, "eb", "jump disp8 bytes away (jmp)");

  put_new(Name, "74", "jump disp8 bytes away if equal, if ZF is set (jcc/jz/je)");

  put_new(Name, "75", "jump disp8 bytes away if not equal, if ZF is not set (jcc/jnz/jne)");

  put_new(Name, "7f", "jump disp8 bytes away if greater, if ZF is unset and SF == OF (jcc/jg/jnle)");

  put_new(Name, "7d", "jump disp8 bytes away if greater or equal, if SF == OF (jcc/jge/jnl)");

  put_new(Name, "7c", "jump disp8 bytes away if lesser, if SF != OF (jcc/jl/jnge)");

  put_new(Name, "7e", "jump disp8 bytes away if lesser or equal, if ZF is set or SF != OF (jcc/jle/jng)");

  put_new(Name, "e9", "jump disp32 bytes away (jmp)");

  put_new(Name_0f, "84", "jump disp32 bytes away if equal, if ZF is set (jcc/jz/je)");

  put_new(Name_0f, "85", "jump disp32 bytes away if not equal, if ZF is not set (jcc/jnz/jne)");

  put_new(Name_0f, "8f", "jump disp32 bytes away if greater, if ZF is unset and SF == OF (jcc/jg/jnle)");

  put_new(Name_0f, "8d", "jump disp32 bytes away if greater or equal, if SF == OF (jcc/jge/jnl)");

  put_new(Name_0f, "8c", "jump disp32 bytes away if lesser, if SF != OF (jcc/jl/jnge)");

  put_new(Name_0f, "8e", "jump disp32 bytes away if lesser or equal, if ZF is set or SF != OF (jcc/jle/jng)");

  put_new(Name, "e8", "call disp32 (call)");

  put_new(Name, "c3", "return from most recent unfinished call (ret)");

  put_new(Name, "cd", "software interrupt (int)");

  put_new(Name, "88", "copy r8 to r8/m8-at-r32");

  put_new(Name, "8a", "copy r8/m8-at-r32 to r8");

  put_new(Name, "c6", "copy imm8 to r8/m8-at-r32 (mov)");

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
    if (line_data.find('"') != string::npos) {  // can cause false-positives, but we can handle them
      parse_instruction_character_by_character(line_data, l);
      continue;
    }

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
        if (!starts_with(segment_title, "0x")) {
          Currently_parsing_named_segment = true;
          if (!contains_key(Segment_index, segment_title)) {
            trace(3, "parse") << "new segment '" << segment_title << "'" << end();
            if (out.segments.empty() && segment_title != "code") {
              raise << "first segment must be 'code' but is '" << segment_title << "'\n" << end();
              return;
            }
            if (SIZE(out.segments) == 1 && segment_title != "data") {
              raise << "second segment must be 'data' but is '" << segment_title << "'\n" << end();
              return;
            }
            put(Segment_index, segment_title, SIZE(out.segments));
            out.segments.push_back(segment());
          }
          else {
            trace(3, "parse") << "appending to segment '" << segment_title << "'" << end();
          }
          Currently_parsing_segment_index = get(Segment_index, segment_title);
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
  if (Currently_parsing_named_segment) {
    assert(!p.segments.empty());
    trace(3, "parse") << "flushing segment" << end();
    vector<line>& curr_segment_data = p.segments.at(Currently_parsing_segment_index).lines;
    curr_segment_data.insert(curr_segment_data.end(), lines.begin(), lines.end());
    lines.clear();
    Currently_parsing_named_segment = false;
    Currently_parsing_segment_index = -1;
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
  if (Entry_address) EIP = Entry_address;
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


    case 4:  // exception: mod 0b00 rm 0b100 => incoming SIB (scale-index-base) byte
      addr = effective_address_from_sib(mod);
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
    case 4:  // exception: mod 0b01 rm 0b100 => incoming SIB (scale-index-base) byte
      addr = effective_address_from_sib(mod);
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
    case 4:  // exception: mod 0b10 rm 0b100 => incoming SIB (scale-index-base) byte
      addr = effective_address_from_sib(mod);
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


void test_add_imm32_to_r32() {
  Reg[EBX].i = 1;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     c3                          0a 0b 0c 0d\n"  // add 0x0d0c0b0a to EBX
      // ModR/M in binary: 11 (direct mode) 000 (add imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop add\n"
      "run: storing 0x0d0c0b0b\n"
  );
}

void test_add_imm32_to_mem_at_r32() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     03                          0a 0b 0c 0d \n"  // add 0x0d0c0b0a to *EBX
      // ModR/M in binary: 00 (indirect mode) 000 (add imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop add\n"
      "run: storing 0x0d0c0b0b\n"
  );
}


void test_subtract_imm32_from_eax() {
  Reg[EAX].i = 0x0d0c0baa;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  2d                                 0a 0b 0c 0d \n"  // subtract 0x0d0c0b0a from EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: subtract imm32 0x0d0c0b0a from EAX\n"
      "run: storing 0x000000a0\n"
  );
}

void test_subtract_imm32_from_mem_at_r32() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     2b                          01 00 00 00 \n"  // subtract 1 from *EBX
      // ModR/M in binary: 00 (indirect mode) 101 (subtract imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "0a 00 00 00\n"  // 0x0000000a
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x00000001\n"
      "run: subop subtract\n"
      "run: storing 0x00000009\n"
  );
}

void test_subtract_imm32_from_r32() {
  Reg[EBX].i = 10;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     eb                          01 00 00 00 \n"  // subtract 1 from EBX
      // ModR/M in binary: 11 (direct mode) 101 (subtract imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x00000001\n"
      "run: subop subtract\n"
      "run: storing 0x00000009\n"
  );
}


void test_shift_left_r32_with_imm8() {
  Reg[EBX].i = 13;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     e3                          01          \n"  // shift EBX left by 1 bit
      // ModR/M in binary: 11 (direct mode) 100 (subop shift left) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift left by CL bits\n"
      "run: storing 0x0000001a\n"
  );
}

void test_shift_right_arithmetic_r32_with_imm8() {
  Reg[EBX].i = 26;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     fb                          01          \n"  // shift EBX right by 1 bit
      // ModR/M in binary: 11 (direct mode) 111 (subop shift right arithmetic) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while preserving sign\n"
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_arithmetic_odd_r32_with_imm8() {
  Reg[EBX].i = 27;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     fb                          01          \n"  // shift EBX right by 1 bit
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

void test_shift_right_arithmetic_negative_r32_with_imm8() {
  Reg[EBX].i = 0xfffffffd;  // -3
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     fb                          01          \n"  // shift EBX right by 1 bit, while preserving sign
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


void test_shift_right_logical_r32_with_imm8() {
  Reg[EBX].i = 26;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     eb                          01          \n"  // shift EBX right by 1 bit, while padding zeroes
      // ModR/M in binary: 11 (direct mode) 101 (subop shift right logical) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while padding zeroes\n"
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_logical_odd_r32_with_imm8() {
  Reg[EBX].i = 27;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     eb                          01          \n"  // shift EBX right by 1 bit, while padding zeroes
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while padding zeroes\n"
      // result: 13
      "run: storing 0x0000000d\n"
  );
}

void test_shift_right_logical_negative_r32_with_imm8() {
  Reg[EBX].i = 0xfffffffd;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c1     eb                          01          \n"  // shift EBX right by 1 bit, while padding zeroes
      // ModR/M in binary: 11 (direct mode) 101 (subop shift right logical) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: operate on r/m32\n"
      "run: r/m32 is EBX\n"
      "run: subop: shift right by CL bits, while padding zeroes\n"
      "run: storing 0x7ffffffe\n"
  );
}


void test_and_imm32_with_eax() {
  Reg[EAX].i = 0xff;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  25                                 0a 0b 0c 0d \n"  // and 0x0d0c0b0a with EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: and imm32 0x0d0c0b0a with EAX\n"
      "run: storing 0x0000000a\n"
  );
}

void test_and_imm32_with_mem_at_r32() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     23                          0a 0b 0c 0d \n"  // and 0x0d0c0b0a with *EBX
      // ModR/M in binary: 00 (indirect mode) 100 (and imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "ff 00 00 00\n"  // 0x000000ff
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop and\n"
      "run: storing 0x0000000a\n"
  );
}

void test_and_imm32_with_r32() {
  Reg[EBX].i = 0xff;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     e3                          0a 0b 0c 0d \n"  // and 0x0d0c0b0a with EBX
      // ModR/M in binary: 11 (direct mode) 100 (and imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop and\n"
      "run: storing 0x0000000a\n"
  );
}


void test_or_imm32_with_eax() {
  Reg[EAX].i = 0xd0c0b0a0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0d                                 0a 0b 0c 0d \n"  // or 0x0d0c0b0a with EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: or imm32 0x0d0c0b0a with EAX\n"
      "run: storing 0xddccbbaa\n"
  );
}

void test_or_imm32_with_mem_at_r32() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     0b                          0a 0b 0c 0d \n"  // or 0x0d0c0b0a with *EBX
      // ModR/M in binary: 00 (indirect mode) 001 (or imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "a0 b0 c0 d0\n"  // 0xd0c0b0a0
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop or\n"
      "run: storing 0xddccbbaa\n"
  );
}

void test_or_imm32_with_r32() {
  Reg[EBX].i = 0xd0c0b0a0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     cb                          0a 0b 0c 0d \n"  // or 0x0d0c0b0a with EBX
      // ModR/M in binary: 11 (direct mode) 001 (or imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop or\n"
      "run: storing 0xddccbbaa\n"
  );
}


void test_xor_imm32_with_eax() {
  Reg[EAX].i = 0xddccb0a0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  35                                 0a 0b 0c 0d \n"  // xor 0x0d0c0b0a with EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: xor imm32 0x0d0c0b0a with EAX\n"
      "run: storing 0xd0c0bbaa\n"
  );
}

void test_xor_imm32_with_mem_at_r32() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     33                          0a 0b 0c 0d \n"  // xor 0x0d0c0b0a with *EBX
      // ModR/M in binary: 00 (indirect mode) 110 (xor imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "a0 b0 c0 d0\n"  // 0xd0c0b0a0
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop xor\n"
      "run: storing 0xddccbbaa\n"
  );
}

void test_xor_imm32_with_r32() {
  Reg[EBX].i = 0xd0c0b0a0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     f3                          0a 0b 0c 0d \n"  // xor 0x0d0c0b0a with EBX
      // ModR/M in binary: 11 (direct mode) 110 (xor imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: subop xor\n"
      "run: storing 0xddccbbaa\n"
  );
}


void test_compare_imm32_with_eax_greater() {
  Reg[EAX].i = 0x0d0c0b0a;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  3d                                 07 0b 0c 0d \n"  // compare 0x0d0c0b07 with EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EAX and imm32 0x0d0c0b07\n"
      "run: SF=0; ZF=0; OF=0\n"
  );
}

void test_compare_imm32_with_eax_lesser() {
  Reg[EAX].i = 0x0d0c0b07;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  3d                                 0a 0b 0c 0d \n"  // compare 0x0d0c0b0a with EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EAX and imm32 0x0d0c0b0a\n"
      "run: SF=1; ZF=0; OF=0\n"
  );
}

void test_compare_imm32_with_eax_equal() {
  Reg[EAX].i = 0x0d0c0b0a;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  3d                                 0a 0b 0c 0d \n"  // compare 0x0d0c0b0a with EAX
  );
  CHECK_TRACE_CONTENTS(
      "run: compare EAX and imm32 0x0d0c0b0a\n"
      "run: SF=0; ZF=1; OF=0\n"
  );
}


void test_compare_imm32_with_r32_greater() {
  Reg[EBX].i = 0x0d0c0b0a;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     fb                          07 0b 0c 0d \n"  // compare 0x0d0c0b07 with EBX
      // ModR/M in binary: 11 (direct mode) 111 (compare imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b07\n"
      "run: SF=0; ZF=0; OF=0\n"
  );
}

void test_compare_imm32_with_r32_lesser() {
  Reg[EBX].i = 0x0d0c0b07;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     fb                          0a 0b 0c 0d \n"  // compare 0x0d0c0b0a with EBX
      // ModR/M in binary: 11 (direct mode) 111 (compare imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: SF=1; ZF=0; OF=0\n"
  );
}

void test_compare_imm32_with_r32_equal() {
  Reg[EBX].i = 0x0d0c0b0a;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     fb                          0a 0b 0c 0d \n"  // compare 0x0d0c0b0a with EBX
      // ModR/M in binary: 11 (direct mode) 111 (compare imm32) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: r/m32 is EBX\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: SF=0; ZF=1; OF=0\n"
  );
}

void test_compare_imm32_with_mem_at_r32_greater() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     3b                          07 0b 0c 0d \n"  // compare 0x0d0c0b07 with *EBX
      // ModR/M in binary: 00 (indirect mode) 111 (compare imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "0a 0b 0c 0d\n"  // 0x0d0c0b0a
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b07\n"
      "run: SF=0; ZF=0; OF=0\n"
  );
}

void test_compare_imm32_with_mem_at_r32_lesser() {
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     3b                          0a 0b 0c 0d \n"  // compare 0x0d0c0b0a with *EBX
      // ModR/M in binary: 00 (indirect mode) 111 (compare imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "07 0b 0c 0d\n"  // 0x0d0c0b07
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: SF=1; ZF=0; OF=0\n"
  );
}

void test_compare_imm32_with_mem_at_r32_equal() {
  Reg[EBX].i = 0x0d0c0b0a;
  Reg[EBX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  81     3b                          0a 0b 0c 0d \n"  // compare 0x0d0c0b0a with *EBX
      // ModR/M in binary: 00 (indirect mode) 111 (compare imm32) 011 (dest EBX)
      "== 0x2000\n"  // data segment
      "0a 0b 0c 0d\n"  // 0x0d0c0b0a
  );
  CHECK_TRACE_CONTENTS(
      "run: combine imm32 with r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
      "run: SF=0; ZF=1; OF=0\n"
  );
}


void test_copy_imm32_to_r32() {
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  bb                                 0a 0b 0c 0d \n"  // copy 0x0d0c0b0a to EBX
  );
  CHECK_TRACE_CONTENTS(
      "run: copy imm32 0x0d0c0b0a to EBX\n"
  );
}

void test_copy_imm32_to_mem_at_r32() {
  Reg[EBX].i = 0x60;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c7     03                          0a 0b 0c 0d \n"  // copy 0x0d0c0b0a to *EBX
      // ModR/M in binary: 00 (indirect mode) 000 (unused) 011 (dest EBX)
  );
  CHECK_TRACE_CONTENTS(
      "run: copy imm32 to r/m32\n"
      "run: effective address is 0x00000060 (EBX)\n"
      "run: imm32 is 0x0d0c0b0a\n"
  );
}

void test_push_imm32() {
  Reg[ESP].u = 0x14;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  68                                 af 00 00 00 \n"  // push *EAX to stack
  );
  CHECK_TRACE_CONTENTS(
      "run: push imm32 0x000000af\n"
      "run: ESP is now 0x00000010\n"
      "run: contents at ESP: 0x000000af\n"
  );
}


void test_add_r32_to_mem_at_r32_with_sib() {
  Reg[EBX].i = 0x10;
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     1c      20                              \n"  // add EBX to *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src EBX) 100 (dest in SIB)
      // SIB in binary: 00 (scale 1) 100 (no index) 000 (base EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00002000 (EAX)\n"
      "run: effective address is 0x00002000\n"
      "run: storing 0x00000011\n"
  );
}

uint32_t effective_address_from_sib(uint8_t mod) {
  const uint8_t sib = next();
  const uint8_t base = sib&0x7;
  uint32_t addr = 0;
  if (base != EBP || mod != 0) {
    addr = Reg[base].u;
    trace(Callstack_depth+1, "run") << "effective address is initially 0x" << HEXWORD << addr << " (" << rname(base) << ")" << end();
  }
  else {
    // base == EBP && mod == 0
    addr = next32();  // ignore base
    trace(Callstack_depth+1, "run") << "effective address is initially 0x" << HEXWORD << addr << " (disp32)" << end();
  }
  const uint8_t index = (sib>>3)&0x7;
  if (index == ESP) {
    // ignore index and scale
    trace(Callstack_depth+1, "run") << "effective address is 0x" << HEXWORD << addr << end();
  }
  else {
    const uint8_t scale = (1 << (sib>>6));
    addr += Reg[index].i*scale;  // treat index register as signed. Maybe base as well? But we'll always ensure it's non-negative.
    trace(Callstack_depth+1, "run") << "effective address is 0x" << HEXWORD << addr << " (after adding " << rname(index) << "*" << NUM(scale) << ")" << end();
  }
  return addr;
}

void test_add_r32_to_mem_at_base_r32_index_r32() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x1ffe;  // dest base
  Reg[ECX].i = 0x2;  // dest index
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     1c      08                              \n"  // add EBX to *(EAX+ECX)
      // ModR/M in binary: 00 (indirect mode) 011 (src EBX) 100 (dest in SIB)
      // SIB in binary: 00 (scale 1) 001 (index ECX) 000 (base EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00001ffe (EAX)\n"
      "run: effective address is 0x00002000 (after adding ECX*1)\n"
      "run: storing 0x00000011\n"
  );
}

void test_add_r32_to_mem_at_displacement_using_sib() {
  Reg[EBX].i = 0x10;  // source
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     1c      25    00 20 00 00               \n"  // add EBX to *0x2000
      // ModR/M in binary: 00 (indirect mode) 011 (src EBX) 100 (dest in SIB)
      // SIB in binary: 00 (scale 1) 100 (no index) 101 (not EBP but disp32)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00002000 (disp32)\n"
      "run: effective address is 0x00002000\n"
      "run: storing 0x00000011\n"
  );
}


void test_add_r32_to_mem_at_base_r32_index_r32_plus_disp8() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x1ff9;  // dest base
  Reg[ECX].i = 0x5;  // dest index
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     5c      08    02                        \n"  // add EBX to *(EAX+ECX+2)
      // ModR/M in binary: 01 (indirect+disp8 mode) 011 (src EBX) 100 (dest in SIB)
      // SIB in binary: 00 (scale 1) 001 (index ECX) 000 (base EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00001ff9 (EAX)\n"
      "run: effective address is 0x00001ffe (after adding ECX*1)\n"
      "run: effective address is 0x00002000 (after adding disp8)\n"
      "run: storing 0x00000011\n"
  );
}

void test_add_r32_to_mem_at_base_r32_index_r32_plus_disp32() {
  Reg[EBX].i = 0x10;  // source
  Reg[EAX].i = 0x1ff9;  // dest base
  Reg[ECX].i = 0x5;  // dest index
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  01     9c      08    02 00 00 00               \n"  // add EBX to *(EAX+ECX+2)
      // ModR/M in binary: 10 (indirect+disp32 mode) 011 (src EBX) 100 (dest in SIB)
      // SIB in binary: 00 (scale 1) 001 (index ECX) 000 (base EAX)
      "== 0x2000\n"  // data segment
      "01 00 00 00\n"  // 0x00000001
  );
  CHECK_TRACE_CONTENTS(
      "run: add EBX to r/m32\n"
      "run: effective address is initially 0x00001ff9 (EAX)\n"
      "run: effective address is 0x00001ffe (after adding ECX*1)\n"
      "run: effective address is 0x00002000 (after adding disp32)\n"
      "run: storing 0x00000011\n"
  );
}



void test_jump_rel8() {
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  eb                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: eb\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_je_rel8_success() {
  ZF = true;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  74                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 74\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_je_rel8_fail() {
  ZF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  74                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 74\n"
      "run: 0x00000003 opcode: 05\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jne_rel8_success() {
  ZF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  75                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 75\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_jne_rel8_fail() {
  ZF = true;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  75                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 75\n"
      "run: 0x00000003 opcode: 05\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jg_rel8_success() {
  ZF = false;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7f                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7f\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_jg_rel8_fail() {
  ZF = false;
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7f                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7f\n"
      "run: 0x00000003 opcode: 05\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jge_rel8_success() {
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7d                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7d\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_jge_rel8_fail() {
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7d                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7d\n"
      "run: 0x00000003 opcode: 05\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jl_rel8_success() {
  ZF = false;
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7c                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7c\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_jl_rel8_fail() {
  ZF = false;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7c                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7c\n"
      "run: 0x00000003 opcode: 05\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jle_rel8_equal() {
  ZF = true;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7e                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7e\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_jle_rel8_lesser() {
  ZF = false;
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7e                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7e\n"
      "run: jump 5\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000003 opcode: 05");
}

void test_jle_rel8_greater() {
  ZF = false;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  7e                   05                        \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 7e\n"
      "run: 0x00000003 opcode: 05\n"
      "run: 0x00000008 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}



void test_jump_disp32() {
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  e9                   05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: e9\n"
      "run: jump 5\n"
      "run: 0x0000000b opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000006 opcode: 05");
}

void test_je_disp32_success() {
  ZF = true;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 84                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_je_disp32_fail() {
  ZF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 84                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: 0x00000007 opcode: 05\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jne_disp32_success() {
  ZF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 85                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_jne_disp32_fail() {
  ZF = true;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 85                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: 0x00000007 opcode: 05\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jg_disp32_success() {
  ZF = false;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8f                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_jg_disp32_fail() {
  ZF = false;
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8f                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: 0x00000007 opcode: 05\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jge_disp32_success() {
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8d                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_jge_disp32_fail() {
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8d                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: 0x00000007 opcode: 05\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jl_disp32_success() {
  ZF = false;
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8c                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_jl_disp32_fail() {
  ZF = false;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8c                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: 0x00000007 opcode: 05\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_jle_disp32_equal() {
  ZF = true;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8e                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_jle_disp32_lesser() {
  ZF = false;
  SF = true;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8e                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: jump 5\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000007 opcode: 05");
}

void test_jle_disp32_greater() {
  ZF = false;
  SF = false;
  OF = false;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  0f 8e                05 00 00 00               \n"  // skip 1 instruction
      "  05                                 00 00 00 01 \n"
      "  05                                 00 00 00 02 \n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000001 opcode: 0f\n"
      "run: 0x00000007 opcode: 05\n"
      "run: 0x0000000c opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: jump 5");
}


void test_call_disp32() {
  Reg[ESP].u = 0x64;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  e8                                 a0 00 00 00 \n"  // call function offset at 0x000000a0
      // next EIP is 6
  );
  CHECK_TRACE_CONTENTS(
      "run: call imm32 0x000000a0\n"
      "run: decrementing ESP to 0x00000060\n"
      "run: pushing value 0x00000006\n"
      "run: jumping to 0x000000a6\n"
  );
}

void test_call_r32() {
  Reg[ESP].u = 0x64;
  Reg[EBX].u = 0x000000a0;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  ff     d3                                      \n"  // call function offset at EBX
      // next EIP is 3
  );
  CHECK_TRACE_CONTENTS(
      "run: call to r/m32\n"
      "run: r/m32 is EBX\n"
      "run: decrementing ESP to 0x00000060\n"
      "run: pushing value 0x00000003\n"
      "run: jumping to 0x000000a3\n"
  );
}

void test_call_mem_at_r32() {
  Reg[ESP].u = 0x64;
  Reg[EBX].u = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  ff     13                                      \n"  // call function offset at *EBX
      // next EIP is 3
      "== 0x2000\n"  // data segment
      "a0 00 00 00\n"  // 0x000000a0
  );
  CHECK_TRACE_CONTENTS(
      "run: call to r/m32\n"
      "run: effective address is 0x00002000 (EBX)\n"
      "run: decrementing ESP to 0x00000060\n"
      "run: pushing value 0x00000003\n"
      "run: jumping to 0x000000a3\n"
  );
}


void test_ret() {
  Reg[ESP].u = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c3                                           \n"  // return
      "== 0x2000\n"  // data segment
      "10 00 00 00\n"  // 0x00000010
  );
  CHECK_TRACE_CONTENTS(
      "run: return\n"
      "run: popping value 0x00000010\n"
      "run: jumping to 0x00000010\n"
  );
}

void process_int80() {
  switch (Reg[EAX].u) {
  case 1:
    exit(/*exit code*/Reg[EBX].u);
    break;
  case 3:
    trace(Callstack_depth+1, "run") << "read: " << Reg[EBX].u << ' ' << Reg[ECX].u << ' ' << Reg[EDX].u << end();
    Reg[EAX].i = read(/*file descriptor*/Reg[EBX].u, /*memory buffer*/mem_addr_u8(Reg[ECX].u), /*size*/Reg[EDX].u);
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  case 4:
    trace(Callstack_depth+1, "run") << "write: " << Reg[EBX].u << ' ' << Reg[ECX].u << ' ' << Reg[EDX].u << end();
    trace(Callstack_depth+1, "run") << Reg[ECX].u << " => " << mem_addr_string(Reg[ECX].u, Reg[EDX].u) << end();
    Reg[EAX].i = write(/*file descriptor*/Reg[EBX].u, /*memory buffer*/mem_addr_u8(Reg[ECX].u), /*size*/Reg[EDX].u);
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  case 5: {
    check_flags(ECX);
    check_mode(EDX);
    trace(Callstack_depth+1, "run") << "open: " << Reg[EBX].u << ' ' << Reg[ECX].u << end();
    trace(Callstack_depth+1, "run") << Reg[EBX].u << " => " << mem_addr_kernel_string(Reg[EBX].u) << end();
    Reg[EAX].i = open(/*filename*/mem_addr_kernel_string(Reg[EBX].u), /*flags*/Reg[ECX].u, /*mode*/0640);
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  }
  case 6:
    trace(Callstack_depth+1, "run") << "close: " << Reg[EBX].u << end();
    Reg[EAX].i = close(/*file descriptor*/Reg[EBX].u);
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  case 8:
    check_mode(ECX);
    trace(Callstack_depth+1, "run") << "creat: " << Reg[EBX].u << end();
    trace(Callstack_depth+1, "run") << Reg[EBX].u << " => " << mem_addr_kernel_string(Reg[EBX].u) << end();
    Reg[EAX].i = creat(/*filename*/mem_addr_kernel_string(Reg[EBX].u), /*mode*/0640);
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  case 10:
    trace(Callstack_depth+1, "run") << "unlink: " << Reg[EBX].u << end();
    trace(Callstack_depth+1, "run") << Reg[EBX].u << " => " << mem_addr_kernel_string(Reg[EBX].u) << end();
    Reg[EAX].i = unlink(/*filename*/mem_addr_kernel_string(Reg[EBX].u));
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  case 38:
    trace(Callstack_depth+1, "run") << "rename: " << Reg[EBX].u << " -> " << Reg[ECX].u << end();
    trace(Callstack_depth+1, "run") << Reg[EBX].u << " => " << mem_addr_kernel_string(Reg[EBX].u) << end();
    trace(Callstack_depth+1, "run") << Reg[ECX].u << " => " << mem_addr_kernel_string(Reg[ECX].u) << end();
    Reg[EAX].i = rename(/*old filename*/mem_addr_kernel_string(Reg[EBX].u), /*new filename*/mem_addr_kernel_string(Reg[ECX].u));
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].i << end();
    if (Reg[EAX].i == -1) raise << strerror(errno) << '\n' << end();
    break;
  case 45:  // brk: modify size of data segment
    trace(Callstack_depth+1, "run") << "grow data segment to " << Reg[EBX].u << end();
    grow_data_segment(/*new end address*/Reg[EBX].u);
    break;
  case 90:  // mmap: allocate memory outside existing segment allocations
    trace(Callstack_depth+1, "run") << "mmap: allocate new segment" << end();
    // Ignore most arguments for now: address hint, protection flags, sharing flags, fd, offset.
    // We only support anonymous maps.
    Reg[EAX].u = new_segment(/*length*/read_mem_u32(Reg[EBX].u+0x4));
    trace(Callstack_depth+1, "run") << "result: " << Reg[EAX].u << end();
    break;
  default:
    raise << HEXWORD << EIP << ": unimplemented syscall " << Reg[EAX].u << '\n' << end();
  }
}

// SubX is oblivious to file permissions, directories, symbolic links, terminals, and much else besides.
// Also ignoring any concurrency considerations for now.
void check_flags(int reg) {
  uint32_t flags = Reg[reg].u;
  if (flags != ((flags & O_RDONLY) | (flags & O_WRONLY))) {
    cerr << HEXWORD << EIP << ": most POSIX flags to the open() syscall are not supported. Just O_RDONLY and O_WRONLY for now. Zero concurrent access support.\n";
    exit(1);
  }
  if ((flags & O_RDONLY) && (flags & O_WRONLY)) {
    cerr << HEXWORD << EIP << ": can't open a file for both reading and writing at once. See http://man7.org/linux/man-pages/man2/open.2.html.\n";
    exit(1);
  }
}

void check_mode(int reg) {
  if (Reg[reg].u != 0600) {
    cerr << HEXWORD << EIP << ": SubX is oblivious to file permissions; register " << reg << " must be 0.\n";
    exit(1);
  }
}

uint32_t new_segment(uint32_t length) {
  uint32_t result = Next_segment;
  Mem.push_back(vma(Next_segment, Next_segment+length));
  Next_segment -= SPACE_FOR_SEGMENT;
  return result;
}


string rname_8bit(uint8_t r) {
  switch (r) {
  case 0: return "AL";  // lowest byte of EAX
  case 1: return "CL";  // lowest byte of ECX
  case 2: return "DL";  // lowest byte of EDX
  case 3: return "BL";  // lowest byte of EBX
  case 4: return "AH";  // second lowest byte of EAX
  case 5: return "CH";  // second lowest byte of ECX
  case 6: return "DH";  // second lowest byte of EDX
  case 7: return "BH";  // second lowest byte of EBX
  default: raise << "invalid 8-bit register " << r << '\n' << end();  return "";
  }
}

uint8_t* effective_byte_address(uint8_t modrm) {
  uint8_t mod = (modrm>>6);
  uint8_t rm = modrm & 0x7;
  if (mod == 3) {
    // select an 8-bit register
    trace(Callstack_depth+1, "run") << "r/m8 is " << rname_8bit(rm) << end();
    return reg_8bit(rm);
  }
  // the rest is as usual
  return mem_addr_u8(effective_address_number(modrm));
}

uint8_t* reg_8bit(uint8_t rm) {
  uint8_t* result = reinterpret_cast<uint8_t*>(&Reg[rm & 0x3].i);  // _L register
  if (rm & 0x4)
    ++result;  // _H register;  assumes host is little-endian
  return result;
}

void test_copy_r8_to_mem_at_r32() {
  Reg[EBX].i = 0x224488ab;
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  88     18                                      \n"  // copy BL to the byte at *EAX
      // ModR/M in binary: 00 (indirect mode) 011 (src BL) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "f0 cc bb aa\n"
  );
  CHECK_TRACE_CONTENTS(
      "run: copy BL to r8/m8-at-r32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0xab\n"
  );
  CHECK_EQ(0xaabbccab, read_mem_u32(0x2000));
}

void test_copy_mem_at_r32_to_r8() {
  Reg[EBX].i = 0xaabbcc0f;  // one nibble each of lowest byte set to all 0s and all 1s, to maximize value of this test
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  8a     18                                      \n"  // copy just the byte at *EAX to BL
      // ModR/M in binary: 00 (indirect mode) 011 (dest EBX) 000 (src EAX)
      "== 0x2000\n"  // data segment
      "ab ff ff ff\n"  // 0xab with more data in following bytes
  );
  CHECK_TRACE_CONTENTS(
      "run: copy r8/m8-at-r32 to BL\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0xab\n"
      // remaining bytes of EBX are *not* cleared
      "run: EBX now contains 0xaabbccab\n"
  );
}

void test_cannot_copy_byte_to_ESP_EBP_ESI_EDI() {
  Reg[ESI].u = 0xaabbccdd;
  Reg[EBX].u = 0x11223344;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  8a     f3                                      \n"  // copy just the byte at *EBX to 8-bit register '6'
      // ModR/M in binary: 11 (direct mode) 110 (dest 8-bit 'register 6') 011 (src EBX)
  );
  CHECK_TRACE_CONTENTS(
      // ensure 8-bit register '6' is DH, not ESI
      "run: copy r8/m8-at-r32 to DH\n"
      "run: storing 0x44\n"
  );
  // ensure ESI is unchanged
  CHECK_EQ(Reg[ESI].u, 0xaabbccdd);
}


void test_copy_imm8_to_mem_at_r32() {
  Reg[EAX].i = 0x2000;
  run(
      "== 0x1\n"  // code segment
      // op     ModR/M  SIB   displacement  immediate
      "  c6     00                          dd          \n"  // copy to the byte at *EAX
      // ModR/M in binary: 00 (indirect mode) 000 (unused) 000 (dest EAX)
      "== 0x2000\n"  // data segment
      "f0 cc bb aa\n"
  );
  CHECK_TRACE_CONTENTS(
      "run: copy imm8 to r8/m8-at-r32\n"
      "run: effective address is 0x00002000 (EAX)\n"
      "run: storing 0xdd\n"
  );
  CHECK_EQ(0xaabbccdd, read_mem_u32(0x2000));
}



void print_translate_usage() {
  cerr << "Usage: subx translate file1 file2 ... -o output\n";
}

// write out a program to a bare-bones ELF file
void save_elf(const program& p, const string& filename) {
  ofstream out(filename.c_str(), ios::binary);
  write_elf_header(out, p);
  for (size_t i = 0;  i < p.segments.size();  ++i)
    write_segment(p.segments.at(i), out);
  out.close();
}

void write_elf_header(ostream& out, const program& p) {
  char c = '\0';
#define O(X)  c = (X); out.write(&c, sizeof(c))
// host is required to be little-endian
#define emit(X)  out.write(reinterpret_cast<const char*>(&X), sizeof(X))
  //// ehdr
  // e_ident
  O(0x7f); O(/*E*/0x45); O(/*L*/0x4c); O(/*F*/0x46);
    O(0x1);  // 32-bit format
    O(0x1);  // little-endian
    O(0x1); O(0x0);
  for (size_t i = 0;  i < 8;  ++i) { O(0x0); }
  // e_type
  O(0x02); O(0x00);
  // e_machine
  O(0x03); O(0x00);
  // e_version
  O(0x01); O(0x00); O(0x00); O(0x00);
  // e_entry
  uint32_t e_entry = p.segments.at(0).start;  // convention
  // Override e_entry
  if (Entry_address) e_entry = Entry_address;

  emit(e_entry);
  // e_phoff -- immediately after ELF header
  uint32_t e_phoff = 0x34;
  emit(e_phoff);
  // e_shoff; unused
  uint32_t dummy32 = 0;
  emit(dummy32);
  // e_flags; unused
  emit(dummy32);
  // e_ehsize
  uint16_t e_ehsize = 0x34;
  emit(e_ehsize);
  // e_phentsize
  uint16_t e_phentsize = 0x20;
  emit(e_phentsize);
  // e_phnum
  uint16_t e_phnum = SIZE(p.segments);
  emit(e_phnum);
  // e_shentsize
  uint16_t dummy16 = 0x0;
  emit(dummy16);
  // e_shnum
  emit(dummy16);
  // e_shstrndx
  emit(dummy16);

  uint32_t p_offset = /*size of ehdr*/0x34 + SIZE(p.segments)*0x20/*size of each phdr*/;
  for (int i = 0;  i < SIZE(p.segments);  ++i) {
    //// phdr
    // p_type
    uint32_t p_type = 0x1;
    emit(p_type);
    // p_offset
    emit(p_offset);
    // p_vaddr
    uint32_t p_start = p.segments.at(i).start;
    emit(p_start);
    // p_paddr
    emit(p_start);
    // p_filesz
    uint32_t size = num_words(p.segments.at(i));
    assert(p_offset + size < SEGMENT_ALIGNMENT);
    emit(size);
    // p_memsz
    emit(size);
    // p_flags
    uint32_t p_flags = (i == 0) ? /*r-x*/0x5 : /*rw-*/0x6;  // convention: only first segment is code
    emit(p_flags);

    // p_align
    // "As the system creates or augments a process image, it logically copies
    // a file's segment to a virtual memory segment.  When—and if— the system
    // physically reads the file depends on the program's execution behavior,
    // system load, and so on.  A process does not require a physical page
    // unless it references the logical page during execution, and processes
    // commonly leave many pages unreferenced. Therefore delaying physical
    // reads frequently obviates them, improving system performance. To obtain
    // this efficiency in practice, executable and shared object files must
    // have segment images whose file offsets and virtual addresses are
    // congruent, modulo the page size." -- http://refspecs.linuxbase.org/elf/elf.pdf (page 95)
    uint32_t p_align = 0x1000;  // default page size on linux
    emit(p_align);
    if (p_offset % p_align != p_start % p_align) {
      raise << "segment starting at 0x" << HEXWORD << p_start << " is improperly aligned; alignment for p_offset " << p_offset << " should be " << (p_offset % p_align) << " but is " << (p_start % p_align) << '\n' << end();
      return;
    }

    // prepare for next segment
    p_offset += size;
  }
#undef O
#undef emit
}

void write_segment(const segment& s, ostream& out) {
  for (int i = 0;  i < SIZE(s.lines);  ++i) {
    const vector<word>& w = s.lines.at(i).words;
    for (int j = 0;  j < SIZE(w);  ++j) {
      uint8_t x = hex_byte(w.at(j).data);  // we're done with metadata by this point
      out.write(reinterpret_cast<const char*>(&x), /*sizeof(byte)*/1);
    }
  }
}

uint32_t num_words(const segment& s) {
  uint32_t sum = 0;
  for (int i = 0;  i < SIZE(s.lines);  ++i)
    sum += SIZE(s.lines.at(i).words);
  return sum;
}



void test_pack_immediate_constants() {
  run(
      "== 0x1\n"  // code segment
      "bb  0x2a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'bb 0x2a/imm32'\n"
      "transform: instruction after packing: 'bb 2a 00 00 00'\n"
      "run: copy imm32 0x0000002a to EBX\n"
  );
}


void init_operand_type_help() {
  put(Help, "mod",
    "2-bit operand controlling the _addressing mode_ of many instructions,\n"
    "to determine how to compute the _effective address_ to look up memory at\n"
    "based on the 'rm32' operand and potentially others.\n"
    "\n"
    "If mod = 3, just operate on the contents of the register specified by rm32\n"
    "            (direct mode).\n"
    "If mod = 2, effective address is usually* rm32 + disp32\n"
    "            (indirect mode with displacement).\n"
    "If mod = 1, effective address is usually* rm32 + disp8\n"
    "            (indirect mode with displacement).\n"
    "If mod = 0, effective address is usually* rm32 (indirect mode).\n"
    "(* - The exception is when rm32 is '4'. Register 4 is the stack pointer (ESP).\n"
    "     Using it as an address gets more involved. For more details,\n"
    "     try reading the help pages for 'base', 'index' and 'scale'.)\n"
    "\n"
    "For complete details, spend some time with two tables in the IA-32 software\n"
    "developer's manual that are also included in this repo:\n"
    "  - modrm.pdf: volume 2, table 2-2, \"32-bit addressing with the ModR/M byte.\".\n"
    "  - sib.pdf: volume 2, table 2-3, \"32-bit addressing with the SIB byte.\".\n"
  );
  put(Help, "subop",
    "Additional 3-bit operand for determining the instruction when the opcode is 81, 8f or ff.\n"
    "Can't coexist with operand of type 'r32' in a single instruction, because the two use the same bits.\n"
  );
  put(Help, "r32",
    "3-bit operand specifying a register operand used directly, without any further addressing modes.\n"
  );
  put(Help, "rm32",
    "32-bit value in register or memory. The precise details of its construction\n"
    "depend on the eponymous 3-bit 'rm32' operand, the 'mod' operand, and also\n"
    "potentially the 'SIB' operands ('scale', 'index' and 'base') and a displacement\n"
    "('disp8' or 'disp32').\n"
    "\n"
    "For complete details, spend some time with two tables in the IA-32 software\n"
    "developer's manual that are also included in this repo:\n"
    "  - modrm.pdf: volume 2, table 2-2, \"32-bit addressing with the ModR/M byte.\".\n"
    "  - sib.pdf: volume 2, table 2-3, \"32-bit addressing with the SIB byte.\".\n"
  );
  put(Help, "base",
    "Additional 3-bit operand (when 'rm32' is 4, unless 'mod' is 3) specifying the\n"
    "register containing an address to look up.\n"
    "This address may be further modified by 'index' and 'scale' operands.\n"
    "  effective address = base + index*scale + displacement (disp8 or disp32)\n"
    "For complete details, spend some time with the IA-32 software developer's manual,\n"
    "volume 2, table 2-3, \"32-bit addressing with the SIB byte\".\n"
    "It is included in this repository as 'sib.pdf'.\n"
  );
  put(Help, "index",
    "Optional 3-bit operand (when 'rm32' is 4 unless 'mod' is 3) that can be added to\n"
    "the 'base' operand to compute the 'effective address' at which to look up memory.\n"
    "  effective address = base + index*scale + displacement (disp8 or disp32)\n"
    "For complete details, spend some time with the IA-32 software developer's manual,\n"
    "volume 2, table 2-3, \"32-bit addressing with the SIB byte\".\n"
    "It is included in this repository as 'sib.pdf'.\n"
  );
  put(Help, "scale",
    "Optional 2-bit operand (when 'rm32' is 4 unless 'mod' is 3) that encodes a\n"
    "power of 2 to be multiplied to the 'index' operand before adding the result to\n"
    "the 'base' operand to compute the _effective address_ to operate on.\n"
    "  effective address = base + index * scale + displacement (disp8 or disp32)\n"
    "\n"
    "When scale is 0, use index unmodified.\n"
    "When scale is 1, multiply index by 2.\n"
    "When scale is 2, multiply index by 4.\n"
    "When scale is 3, multiply index by 8.\n"
    "\n"
    "For complete details, spend some time with the IA-32 software developer's manual,\n"
    "volume 2, table 2-3, \"32-bit addressing with the SIB byte\".\n"
    "It is included in this repository as 'sib.pdf'.\n"
  );
  put(Help, "disp8",
    "8-bit value to be added in many instructions.\n"
  );
  put(Help, "disp16",
    "16-bit value to be added in many instructions.\n"
    "Currently not used in any SubX instructions.\n"
  );
  put(Help, "disp32",
    "32-bit value to be added in many instructions.\n"
  );
  put(Help, "imm8",
    "8-bit value for many instructions.\n"
  );
  put(Help, "imm32",
    "32-bit value for many instructions.\n"
  );
}


void pack_operands(program& p) {
  if (p.segments.empty()) return;
  segment& code = p.segments.at(0);
  check_operands(code);
  if (trace_contains_errors()) return;

  check_operand_bounds(code);
  if (trace_contains_errors()) return;
  // Pack Operands(segment code)
  trace(3, "transform") << "-- pack operands" << end();
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    line& inst = code.lines.at(i);
    if (all_hex_bytes(inst)) continue;
    trace(99, "transform") << "packing instruction '" << to_string(/*with metadata*/inst) << "'" << end();
    pack_operands(inst);
    trace(99, "transform") << "instruction after packing: '" << to_string(/*without metadata*/inst.words) << "'" << end();
  }
}

void pack_operands(line& inst) {
  line new_inst;
  add_opcodes(inst, new_inst);
  add_modrm_byte(inst, new_inst);
  add_sib_byte(inst, new_inst);
  add_disp_bytes(inst, new_inst);
  add_imm_bytes(inst, new_inst);
  inst.words.swap(new_inst.words);
}

void add_opcodes(const line& in, line& out) {
  out.words.push_back(in.words.at(0));
  if (in.words.at(0).data == "0f" || in.words.at(0).data == "f2" || in.words.at(0).data == "f3")
    out.words.push_back(in.words.at(1));
  if (in.words.at(0).data == "f3" && in.words.at(1).data == "0f")
    out.words.push_back(in.words.at(2));
  if (in.words.at(0).data == "f2" && in.words.at(1).data == "0f")
    out.words.push_back(in.words.at(2));
}

void add_modrm_byte(const line& in, line& out) {
  uint8_t mod=0, reg_subop=0, rm32=0;
  bool emit = false;
  for (int i = 0;  i < SIZE(in.words);  ++i) {
    const word& curr = in.words.at(i);
    if (has_operand_metadata(curr, "mod")) {
      mod = hex_byte(curr.data);
      emit = true;
    }
    else if (has_operand_metadata(curr, "rm32")) {
      rm32 = hex_byte(curr.data);
      emit = true;
    }
    else if (has_operand_metadata(curr, "r32")) {
      reg_subop = hex_byte(curr.data);
      emit = true;
    }
    else if (has_operand_metadata(curr, "subop")) {
      reg_subop = hex_byte(curr.data);
      emit = true;
    }
  }
  if (emit)
    out.words.push_back(hex_byte_text((mod << 6) | (reg_subop << 3) | rm32));
}

void add_sib_byte(const line& in, line& out) {
  uint8_t scale=0, index=0, base=0;
  bool emit = false;
  for (int i = 0;  i < SIZE(in.words);  ++i) {
    const word& curr = in.words.at(i);
    if (has_operand_metadata(curr, "scale")) {
      scale = hex_byte(curr.data);
      emit = true;
    }
    else if (has_operand_metadata(curr, "index")) {
      index = hex_byte(curr.data);
      emit = true;
    }
    else if (has_operand_metadata(curr, "base")) {
      base = hex_byte(curr.data);
      emit = true;
    }
  }
  if (emit)
    out.words.push_back(hex_byte_text((scale << 6) | (index << 3) | base));
}

void add_disp_bytes(const line& in, line& out) {
  for (int i = 0;  i < SIZE(in.words);  ++i) {
    const word& curr = in.words.at(i);
    if (has_operand_metadata(curr, "disp8"))
      emit_hex_bytes(out, curr, 1);
    if (has_operand_metadata(curr, "disp16"))
      emit_hex_bytes(out, curr, 2);
    else if (has_operand_metadata(curr, "disp32"))
      emit_hex_bytes(out, curr, 4);
  }
}

void add_imm_bytes(const line& in, line& out) {
  for (int i = 0;  i < SIZE(in.words);  ++i) {
    const word& curr = in.words.at(i);
    if (has_operand_metadata(curr, "imm8"))
      emit_hex_bytes(out, curr, 1);
    else if (has_operand_metadata(curr, "imm32"))
      emit_hex_bytes(out, curr, 4);
  }
}

void emit_hex_bytes(line& out, const word& w, int num) {
  assert(num <= 4);
  bool is_number = looks_like_hex_int(w.data);
  if (num == 1 || !is_number) {
    out.words.push_back(w);  // preserve existing metadata
    if (is_number)
      out.words.back().data = hex_byte_to_string(parse_int(w.data));
    return;
  }
  emit_hex_bytes(out, static_cast<uint32_t>(parse_int(w.data)), num);
}

void emit_hex_bytes(line& out, uint32_t val, int num) {
  assert(num <= 4);
  for (int i = 0;  i < num;  ++i) {
    out.words.push_back(hex_byte_text(val & 0xff));
    val = val >> 8;
  }
}

word hex_byte_text(uint8_t val) {
  word result;
  result.data = hex_byte_to_string(val);
  result.original = result.data+"/auto";
  return result;
}

string hex_byte_to_string(uint8_t val) {
  ostringstream out;
  // uint8_t prints without padding, but int8_t will expand to 32 bits again
  out << HEXBYTE << NUM(val);
  return out.str();
}

string to_string(const vector<word>& in) {
  ostringstream out;
  for (int i = 0;  i < SIZE(in);  ++i) {
    if (i > 0) out << ' ';
    out << in.at(i).data;
  }
  return out.str();
}

void test_pack_disp8() {
  run(
      "== 0x1\n"  // code segment
      "74 2/disp8\n"  // jump 2 bytes away if ZF is set
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction '74 2/disp8'\n"
      "transform: instruction after packing: '74 02'\n"
  );
}

void test_pack_disp8_negative() {
  transform(
      "== 0x1\n"  // code segment
      // running this will cause an infinite loop
      "74 -1/disp8\n"  // jump 1 byte before if ZF is set
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction '74 -1/disp8'\n"
      "transform: instruction after packing: '74 ff'\n"
  );
}

void transform(const string& text_bytes) {
  program p;
  istringstream in(text_bytes);
  parse(in, p);
  if (trace_contains_errors()) return;
  transform(p);
}

void test_pack_modrm_imm32() {
  run(
      "== 0x1\n"  // code segment
      // instruction                     effective address                                                   operand     displacement    immediate\n"
      // op          subop               mod             rm32          base        index         scale       r32\n"
      // 1-3 bytes   3 bits              2 bits          3 bits        3 bits      3 bits        2 bits      2 bits      0/1/2/4 bytes   0/1/2/4 bytes\n"
      "  81          0/add/subop         3/mod/direct    3/ebx/rm32                                                                      1/imm32      \n"  // add 1 to EBX
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction '81 0/add/subop 3/mod/direct 3/ebx/rm32 1/imm32'\n"
      "transform: instruction after packing: '81 c3 01 00 00 00'\n"
  );
}

void test_pack_imm32_large() {
  run(
      "== 0x1\n"  // code segment
      "b9  0x080490a7/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'b9 0x080490a7/imm32'\n"
      "transform: instruction after packing: 'b9 a7 90 04 08'\n"
  );
}

void test_pack_immediate_constants_hex() {
  run(
      "== 0x1\n"  // code segment
      "b9  0x2a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'b9 0x2a/imm32'\n"
      "transform: instruction after packing: 'b9 2a 00 00 00'\n"
      "run: copy imm32 0x0000002a to ECX\n"
  );
}

void test_pack_silently_ignores_non_hex() {
  Hide_errors = true;
  transform(
      "== 0x1\n"  // code segment
      "b9  foo/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'b9 foo/imm32'\n"
      // no change (we're just not printing metadata to the trace)
      "transform: instruction after packing: 'b9 foo'\n"
  );
}

void test_pack_flags_bad_hex() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "b9  0xfoo/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: not a number: 0xfoo\n"
  );
}


bool all_hex_bytes(const line& inst) {
  for (int i = 0;  i < SIZE(inst.words);  ++i)
    if (!is_hex_byte(inst.words.at(i)))
      return false;
  return true;
}

bool is_hex_byte(const word& curr) {
  if (contains_any_operand_metadata(curr))
    return false;
  if (SIZE(curr.data) != 2)
    return false;
  if (curr.data.find_first_not_of("0123456789abcdefABCDEF") != string::npos)
    return false;
  return true;
}

bool contains_any_operand_metadata(const word& word) {
  for (int i = 0;  i < SIZE(word.metadata);  ++i)
    if (Instruction_operands.find(word.metadata.at(i)) != Instruction_operands.end())
      return true;
  return false;
}

bool has_operand_metadata(const line& inst, const string& m) {
  bool result = false;
  for (int i = 0;  i < SIZE(inst.words);  ++i) {
    if (!has_operand_metadata(inst.words.at(i), m)) continue;
    if (result) {
      raise << "'" << to_string(inst) << "' has conflicting " << m << " operands\n" << end();
      return false;
    }
    result = true;
  }
  return result;
}

bool has_operand_metadata(const word& w, const string& m) {
  bool result = false;
  bool metadata_found = false;
  for (int i = 0;  i < SIZE(w.metadata);  ++i) {
    const string& curr = w.metadata.at(i);
    if (Instruction_operands.find(curr) == Instruction_operands.end()) continue;  // ignore unrecognized metadata
    if (metadata_found) {
      raise << "'" << w.original << "' has conflicting operand types; it should have only one\n" << end();
      return false;
    }
    metadata_found = true;
    result = (curr == m);
  }
  return result;
}

word metadata(const line& inst, const string& m) {
  for (int i = 0;  i < SIZE(inst.words);  ++i)
    if (has_operand_metadata(inst.words.at(i), m))
      return inst.words.at(i);
  assert(false);
}

bool looks_like_hex_int(const string& s) {
  if (s.empty()) return false;
  if (s.at(0) == '-' || s.at(0) == '+') return true;
  if (isdigit(s.at(0))) return true;  // includes '0x' prefix
  if (SIZE(s) == 2) return true;

  // End looks_like_hex_int(s) Detectors
  return false;
}

string to_string(const line& inst) {
  ostringstream out;
  for (int i = 0;  i < SIZE(inst.words);  ++i) {
    if (i > 0) out << ' ';
    out << inst.words.at(i).original;
  }
  return out.str();
}


void test_check_missing_imm8_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "cd\n"  // interrupt ??
  );
  CHECK_TRACE_CONTENTS(
      "error: 'cd' (software interrupt): missing imm8 operand\n"
  );
}

void check_operands(const segment& code) {
  trace(3, "transform") << "-- check operands" << end();
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    check_operands(code.lines.at(i));
    if (trace_contains_errors()) return;  // stop at the first mal-formed instruction
  }
}

void check_operands(const line& inst) {
  word op = preprocess_op(inst.words.at(0));
  if (op.data == "0f") {
    check_operands_0f(inst);
    return;
  }
  if (op.data == "f3") {
    check_operands_f3(inst);
    return;
  }
  check_operands(inst, op);
}

word preprocess_op(word/*copy*/ op) {
  op.data = tolower(op.data.c_str());
  // opcodes can't be negative
  if (starts_with(op.data, "0x"))
    op.data = op.data.substr(2);
  if (SIZE(op.data) == 1)
    op.data = string("0")+op.data;
  return op;
}

void test_preprocess_op() {
  word w1;  w1.data = "0xf";
  word w2;  w2.data = "0f";
  CHECK_EQ(preprocess_op(w1).data, preprocess_op(w2).data);
}


void init_op_types() {
  assert(NUM_OPERAND_TYPES <= /*bits in a uint8_t*/8);
  Operand_type_name.resize(NUM_OPERAND_TYPES);
  #define DEF(type) Operand_type_name.at(type) = tolower(#type), put(Operand_type, tolower(#type), type);
  DEF(MODRM);
  DEF(SUBOP);
  DEF(DISP8);
  DEF(DISP16);
  DEF(DISP32);
  DEF(IMM8);
  DEF(IMM32);
  #undef DEF
}

void init_permitted_operands() {
  //// Class A: just op, no operands
  // halt
  put(Permitted_operands, "f4", 0x00);
  // inc
  put(Permitted_operands, "40", 0x00);
  put(Permitted_operands, "41", 0x00);
  put(Permitted_operands, "42", 0x00);
  put(Permitted_operands, "43", 0x00);
  put(Permitted_operands, "44", 0x00);
  put(Permitted_operands, "45", 0x00);
  put(Permitted_operands, "46", 0x00);
  put(Permitted_operands, "47", 0x00);
  // dec
  put(Permitted_operands, "48", 0x00);
  put(Permitted_operands, "49", 0x00);
  put(Permitted_operands, "4a", 0x00);
  put(Permitted_operands, "4b", 0x00);
  put(Permitted_operands, "4c", 0x00);
  put(Permitted_operands, "4d", 0x00);
  put(Permitted_operands, "4e", 0x00);
  put(Permitted_operands, "4f", 0x00);
  // push
  put(Permitted_operands, "50", 0x00);
  put(Permitted_operands, "51", 0x00);
  put(Permitted_operands, "52", 0x00);
  put(Permitted_operands, "53", 0x00);
  put(Permitted_operands, "54", 0x00);
  put(Permitted_operands, "55", 0x00);
  put(Permitted_operands, "56", 0x00);
  put(Permitted_operands, "57", 0x00);
  // pop
  put(Permitted_operands, "58", 0x00);
  put(Permitted_operands, "59", 0x00);
  put(Permitted_operands, "5a", 0x00);
  put(Permitted_operands, "5b", 0x00);
  put(Permitted_operands, "5c", 0x00);
  put(Permitted_operands, "5d", 0x00);
  put(Permitted_operands, "5e", 0x00);
  put(Permitted_operands, "5f", 0x00);
  // return
  put(Permitted_operands, "c3", 0x00);

  //// Class B: just op and disp8
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     0     0      |0       1     0     0

  // jump
  put(Permitted_operands, "eb", 0x04);
  put(Permitted_operands, "74", 0x04);
  put(Permitted_operands, "75", 0x04);
  put(Permitted_operands, "7c", 0x04);
  put(Permitted_operands, "7d", 0x04);
  put(Permitted_operands, "7e", 0x04);
  put(Permitted_operands, "7f", 0x04);

  //// Class D: just op and disp32
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     0     1      |0       0     0     0
  put(Permitted_operands, "e8", 0x10);  // call
  put(Permitted_operands, "e9", 0x10);  // jump

  //// Class E: just op and imm8
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     1     0      |0       0     0     0
  put(Permitted_operands, "cd", 0x20);  // software interrupt

  //// Class F: just op and imm32
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  1     0     0      |0       0     0     0
  put(Permitted_operands, "05", 0x40);  // add
  put(Permitted_operands, "2d", 0x40);  // subtract
  put(Permitted_operands, "25", 0x40);  // and
  put(Permitted_operands, "0d", 0x40);  // or
  put(Permitted_operands, "35", 0x40);  // xor
  put(Permitted_operands, "3d", 0x40);  // compare
  put(Permitted_operands, "68", 0x40);  // push
  // copy
  put(Permitted_operands, "b8", 0x40);
  put(Permitted_operands, "b9", 0x40);
  put(Permitted_operands, "ba", 0x40);
  put(Permitted_operands, "bb", 0x40);
  put(Permitted_operands, "bc", 0x40);
  put(Permitted_operands, "bd", 0x40);
  put(Permitted_operands, "be", 0x40);
  put(Permitted_operands, "bf", 0x40);

  //// Class M: using ModR/M byte
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     0     0      |0       0     0     1

  // add
  put(Permitted_operands, "01", 0x01);
  put(Permitted_operands, "03", 0x01);
  // subtract
  put(Permitted_operands, "29", 0x01);
  put(Permitted_operands, "2b", 0x01);
  // and
  put(Permitted_operands, "21", 0x01);
  put(Permitted_operands, "23", 0x01);
  // or
  put(Permitted_operands, "09", 0x01);
  put(Permitted_operands, "0b", 0x01);
  // xor
  put(Permitted_operands, "31", 0x01);
  put(Permitted_operands, "33", 0x01);
  // compare
  put(Permitted_operands, "39", 0x01);
  put(Permitted_operands, "3b", 0x01);
  // copy
  put(Permitted_operands, "88", 0x01);
  put(Permitted_operands, "89", 0x01);
  put(Permitted_operands, "8a", 0x01);
  put(Permitted_operands, "8b", 0x01);
  // swap
  put(Permitted_operands, "87", 0x01);
  // copy address (lea)
  put(Permitted_operands, "8d", 0x01);
  // pop
  put(Permitted_operands, "8f", 0x01);

  //// Class N: op, ModR/M and subop (not r32)
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     0     0      |0       0     1     1
  put(Permitted_operands, "d3", 0x03);  // shift
  put(Permitted_operands, "f7", 0x03);  // test/not/mul/div
  put(Permitted_operands, "ff", 0x03);  // jump/push/call

  //// Class O: op, ModR/M, subop (not r32) and imm8
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     1     0      |0       0     1     1
  put(Permitted_operands, "c1", 0x23);  // combine
  put(Permitted_operands, "c6", 0x23);  // copy

  //// Class P: op, ModR/M, subop (not r32) and imm32
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  1     0     0      |0       0     1     1
  put(Permitted_operands, "81", 0x43);  // combine
  put(Permitted_operands, "c7", 0x43);  // copy

  //// Class D: just op and disp32
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     0     1      |0       0     0     0
  put_new(Permitted_operands_0f, "84", 0x10);
  put_new(Permitted_operands_0f, "85", 0x10);
  put_new(Permitted_operands_0f, "8c", 0x10);
  put_new(Permitted_operands_0f, "8d", 0x10);
  put_new(Permitted_operands_0f, "8e", 0x10);
  put_new(Permitted_operands_0f, "8f", 0x10);

  //// Class M: using ModR/M byte
  //  imm32 imm8  disp32 |disp16  disp8 subop modrm
  //  0     0     0      |0       0     0     1
  put_new(Permitted_operands_0f, "af", 0x01);

  // End Init Permitted Operands
}

#define HAS(bitvector, bit)  ((bitvector) & (1 << (bit)))
#define SET(bitvector, bit)  ((bitvector) | (1 << (bit)))
#define CLEAR(bitvector, bit)  ((bitvector) & (~(1 << (bit))))

void check_operands(const line& inst, const word& op) {
  if (!is_hex_byte(op)) return;
  uint8_t expected_bitvector = get(Permitted_operands, op.data);
  if (HAS(expected_bitvector, MODRM)) {
    check_operands_modrm(inst, op);
    compare_bitvector_modrm(inst, expected_bitvector, op);
  }
  else {
    compare_bitvector(inst, expected_bitvector, op);
  }
}


void compare_bitvector(const line& inst, uint8_t expected, const word& op) {
  if (all_hex_bytes(inst) && has_operands(inst)) return;  // deliberately programming in raw hex; we'll raise a warning elsewhere
  uint8_t bitvector = compute_expected_operand_bitvector(inst);
  if (trace_contains_errors()) return;  // duplicate operand type
  if (bitvector == expected) return;  // all good with this instruction
  for (int i = 0;  i < NUM_OPERAND_TYPES;  ++i, bitvector >>= 1, expected >>= 1) {
//?     cerr << "comparing " << HEXBYTE << NUM(bitvector) << " with " << NUM(expected) << '\n';
    if ((bitvector & 0x1) == (expected & 0x1)) continue;  // all good with this operand
    const string& optype = Operand_type_name.at(i);
    if ((bitvector & 0x1) > (expected & 0x1))
      raise << "'" << to_string(inst) << "'" << maybe_name(op) << ": unexpected " << optype << " operand\n" << end();
    else
      raise << "'" << to_string(inst) << "'" << maybe_name(op) << ": missing " << optype << " operand\n" << end();
    // continue giving all errors for a single instruction
  }
  // ignore settings in any unused bits
}

string maybe_name(const word& op) {
  if (!is_hex_byte(op)) return "";
  if (!contains_key(Name, op.data)) return "";
  // strip stuff in parens from the name
  const string& s = get(Name, op.data);
  return " ("+s.substr(0, s.find(" ("))+')';
}

uint32_t compute_expected_operand_bitvector(const line& inst) {
  set<string> operands_found;
  uint32_t bitvector = 0;
  for (int i = /*skip op*/1;  i < SIZE(inst.words);  ++i) {
    bitvector = bitvector | expected_bit_for_received_operand(inst.words.at(i), operands_found, inst);
    if (trace_contains_errors()) return INVALID_OPERANDS;  // duplicate operand type
  }
  return bitvector;
}

bool has_operands(const line& inst) {
  return SIZE(inst.words) > first_operand(inst);
}

int first_operand(const line& inst) {
  if (inst.words.at(0).data == "0f") return 2;
  if (inst.words.at(0).data == "f2" || inst.words.at(0).data == "f3") {
    if (inst.words.at(1).data == "0f")
      return 3;
    else
      return 2;
  }
  return 1;
}

// Scan the metadata of 'w' and return the expected bit corresponding to any operand type.
// Also raise an error if metadata contains multiple operand types.
uint32_t expected_bit_for_received_operand(const word& w, set<string>& instruction_operands, const line& inst) {
  uint32_t bv = 0;
  bool found = false;
  for (int i = 0;  i < SIZE(w.metadata);  ++i) {
    string/*copy*/ curr = w.metadata.at(i);
    string expected_metadata = curr;
    if (curr == "mod" || curr == "rm32" || curr == "r32" || curr == "scale" || curr == "index" || curr == "base")
      expected_metadata = "modrm";
    else if (!contains_key(Operand_type, curr)) continue;  // ignore unrecognized metadata
    if (found) {
      raise << "'" << w.original << "' has conflicting operand types; it should have only one\n" << end();
      return INVALID_OPERANDS;
    }
    if (instruction_operands.find(curr) != instruction_operands.end()) {
      raise << "'" << to_string(inst) << "': duplicate " << curr << " operand\n" << end();
      return INVALID_OPERANDS;
    }
    instruction_operands.insert(curr);
    bv = (1 << get(Operand_type, expected_metadata));
    found = true;
  }
  return bv;
}

void test_conflicting_operand_type() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "cd/software-interrupt 80/imm8/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '80/imm8/imm32' has conflicting operand types; it should have only one\n"
  );
}


void test_check_missing_mod_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "81 0/add/subop       3/rm32/ebx 1/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '81 0/add/subop 3/rm32/ebx 1/imm32' (combine rm32 with imm32 based on subop): missing mod operand\n"
  );
}

void check_operands_modrm(const line& inst, const word& op) {
  if (all_hex_bytes(inst)) return;  // deliberately programming in raw hex; we'll raise a warning elsewhere
  check_operand_metadata_present(inst, "mod", op);
  check_operand_metadata_present(inst, "rm32", op);
  // no check for r32; some instructions don't use it; just assume it's 0 if missing
  if (op.data == "81" || op.data == "8f" || op.data == "ff") {  // keep sync'd with 'help subop'
    check_operand_metadata_present(inst, "subop", op);
    check_operand_metadata_absent(inst, "r32", op, "should be replaced by subop");
  }
  if (trace_contains_errors()) return;
  if (metadata(inst, "rm32").data != "4") return;
  // SIB byte checks
  uint8_t mod = hex_byte(metadata(inst, "mod").data);
  if (mod != /*direct*/3) {
    check_operand_metadata_present(inst, "base", op);
    check_operand_metadata_present(inst, "index", op);  // otherwise why go to SIB?
  }
  else {
    check_operand_metadata_absent(inst, "base", op, "direct mode");
    check_operand_metadata_absent(inst, "index", op, "direct mode");
  }
  // no check for scale; 0 (2**0 = 1) by default
}

// same as compare_bitvector, with one additional exception for modrm-based
// instructions: they may use an extra displacement on occasion
void compare_bitvector_modrm(const line& inst, uint8_t expected, const word& op) {
  if (all_hex_bytes(inst) && has_operands(inst)) return;  // deliberately programming in raw hex; we'll raise a warning elsewhere
  uint8_t bitvector = compute_expected_operand_bitvector(inst);
  if (trace_contains_errors()) return;  // duplicate operand type
  // update 'expected' bitvector for the additional exception
  if (has_operand_metadata(inst, "mod")) {
    int32_t mod = parse_int(metadata(inst, "mod").data);
    switch (mod) {
    case 0:
      if (has_operand_metadata(inst, "rm32") && parse_int(metadata(inst, "rm32").data) == 5)
        expected |= (1<<DISP32);
      break;
    case 1:
      expected |= (1<<DISP8);
      break;
    case 2:
      expected |= (1<<DISP32);
      break;
    }
  }
  if (bitvector == expected) return;  // all good with this instruction
  for (int i = 0;  i < NUM_OPERAND_TYPES;  ++i, bitvector >>= 1, expected >>= 1) {
//?     cerr << "comparing for modrm " << HEXBYTE << NUM(bitvector) << " with " << NUM(expected) << '\n';
    if ((bitvector & 0x1) == (expected & 0x1)) continue;  // all good with this operand
    const string& optype = Operand_type_name.at(i);
    if ((bitvector & 0x1) > (expected & 0x1))
      raise << "'" << to_string(inst) << "'" << maybe_name(op) << ": unexpected " << optype << " operand\n" << end();
    else
      raise << "'" << to_string(inst) << "'" << maybe_name(op) << ": missing " << optype << " operand\n" << end();
    // continue giving all errors for a single instruction
  }
  // ignore settings in any unused bits
}

void check_operand_metadata_present(const line& inst, const string& type, const word& op) {
  if (!has_operand_metadata(inst, type))
    raise << "'" << to_string(inst) << "'" << maybe_name(op) << ": missing " << type << " operand\n" << end();
}

void check_operand_metadata_absent(const line& inst, const string& type, const word& op, const string& msg) {
  if (has_operand_metadata(inst, type))
    raise << "'" << to_string(inst) << "'" << maybe_name(op) << ": unexpected " << type << " operand (" << msg << ")\n" << end();
}

void test_modrm_with_displacement() {
  Reg[EAX].u = 0x1;
  transform(
      "== 0x1\n"
      // just avoid null pointer
      "8b/copy 1/mod/lookup+disp8 0/rm32/EAX 2/r32/EDX 4/disp8\n"  // copy *(EAX+4) to EDX
  );
  CHECK_TRACE_COUNT("error", 0);
}

void test_check_missing_disp8() {
  Hide_errors = true;
  transform(
      "== 0x1\n"  // code segment
      "89/copy 1/mod/lookup+disp8 0/rm32/EAX 1/r32/ECX\n"  // missing disp8
  );
  CHECK_TRACE_CONTENTS(
      "error: '89/copy 1/mod/lookup+disp8 0/rm32/EAX 1/r32/ECX' (copy r32 to rm32): missing disp8 operand\n"
  );
}

void test_check_missing_disp32() {
  Hide_errors = true;
  transform(
      "== 0x1\n"  // code segment
      "8b/copy 0/mod/indirect 5/rm32/.disp32 2/r32/EDX\n"  // missing disp32
  );
  CHECK_TRACE_CONTENTS(
      "error: '8b/copy 0/mod/indirect 5/rm32/.disp32 2/r32/EDX' (copy rm32 to r32): missing disp32 operand\n"
  );
}

void test_conflicting_operands_in_modrm_instruction() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "01/add 0/mod 3/mod\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '01/add 0/mod 3/mod' has conflicting mod operands\n"
  );
}

void test_conflicting_operand_type_modrm() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "01/add 0/mod 3/rm32/r32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '3/rm32/r32' has conflicting operand types; it should have only one\n"
  );
}

void test_check_missing_rm32_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "81 0/add/subop 0/mod            1/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '81 0/add/subop 0/mod 1/imm32' (combine rm32 with imm32 based on subop): missing rm32 operand\n"
  );
}

void test_check_missing_subop_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "81             0/mod 3/rm32/ebx 1/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '81 0/mod 3/rm32/ebx 1/imm32' (combine rm32 with imm32 based on subop): missing subop operand\n"
  );
}

void test_check_missing_base_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "81 0/add/subop 0/mod/indirect 4/rm32/use-sib 1/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '81 0/add/subop 0/mod/indirect 4/rm32/use-sib 1/imm32' (combine rm32 with imm32 based on subop): missing base operand\n"
  );
}

void test_check_missing_index_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "81 0/add/subop 0/mod/indirect 4/rm32/use-sib 0/base 1/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '81 0/add/subop 0/mod/indirect 4/rm32/use-sib 0/base 1/imm32' (combine rm32 with imm32 based on subop): missing index operand\n"
  );
}

void test_check_missing_base_operand_2() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "81 0/add/subop 0/mod/indirect 4/rm32/use-sib 2/index 3/scale 1/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '81 0/add/subop 0/mod/indirect 4/rm32/use-sib 2/index 3/scale 1/imm32' (combine rm32 with imm32 based on subop): missing base operand\n"
  );
}

void test_check_extra_displacement() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "89/copy 0/mod/indirect 0/rm32/EAX 1/r32/ECX 4/disp8\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '89/copy 0/mod/indirect 0/rm32/EAX 1/r32/ECX 4/disp8' (copy r32 to rm32): unexpected disp8 operand\n"
  );
}

void test_check_duplicate_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "89/copy 0/mod/indirect 0/rm32/EAX 1/r32/ECX 1/r32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '89/copy 0/mod/indirect 0/rm32/EAX 1/r32/ECX 1/r32': duplicate r32 operand\n"
  );
}

void test_check_base_operand_not_needed_in_direct_mode() {
  run(
      "== 0x1\n"  // code segment
      "81 0/add/subop 3/mod/indirect 4/rm32/use-sib 1/imm32\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

void test_extra_modrm() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "59/pop-to-ECX  3/mod/direct 1/rm32/ECX 4/r32/ESP\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '59/pop-to-ECX 3/mod/direct 1/rm32/ECX 4/r32/ESP' (pop top of stack to ECX): unexpected modrm operand\n"
  );
}


void check_operands_0f(const line& inst) {
  assert(inst.words.at(0).data == "0f");
  if (SIZE(inst.words) == 1) {
    raise << "opcode '0f' requires a second opcode\n" << end();
    return;
  }
  word op = preprocess_op(inst.words.at(1));
  if (!contains_key(Name_0f, op.data)) {
    raise << "unknown 2-byte opcode '0f " << op.data << "'\n" << end();
    return;
  }
  check_operands_0f(inst, op);
}

void check_operands_f3(const line& /*unused*/) {
  raise << "no supported opcodes starting with f3\n" << end();
}

void test_check_missing_disp32_operand() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "  0f 84                                                                                                                                             # jmp if ZF to ??\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '0f 84' (jump disp32 bytes away if equal, if ZF is set): missing disp32 operand\n"
  );
}

void check_operands_0f(const line& inst, const word& op) {
  uint8_t expected_bitvector = get(Permitted_operands_0f, op.data);
  if (HAS(expected_bitvector, MODRM))
    check_operands_modrm(inst, op);
  compare_bitvector_0f(inst, CLEAR(expected_bitvector, MODRM), op);
}

void compare_bitvector_0f(const line& inst, uint8_t expected, const word& op) {
  if (all_hex_bytes(inst) && has_operands(inst)) return;  // deliberately programming in raw hex; we'll raise a warning elsewhere
  uint8_t bitvector = compute_expected_operand_bitvector(inst);
  if (trace_contains_errors()) return;  // duplicate operand type
  if (bitvector == expected) return;  // all good with this instruction
  for (int i = 0;  i < NUM_OPERAND_TYPES;  ++i, bitvector >>= 1, expected >>= 1) {
//?     cerr << "comparing " << HEXBYTE << NUM(bitvector) << " with " << NUM(expected) << '\n';
    if ((bitvector & 0x1) == (expected & 0x1)) continue;  // all good with this operand
    const string& optype = Operand_type_name.at(i);
    if ((bitvector & 0x1) > (expected & 0x1))
      raise << "'" << to_string(inst) << "'" << maybe_name_0f(op) << ": unexpected " << optype << " operand\n" << end();
    else
      raise << "'" << to_string(inst) << "'" << maybe_name_0f(op) << ": missing " << optype << " operand\n" << end();
    // continue giving all errors for a single instruction
  }
  // ignore settings in any unused bits
}

string maybe_name_0f(const word& op) {
  if (!is_hex_byte(op)) return "";
  if (!contains_key(Name_0f, op.data)) return "";
  // strip stuff in parens from the name
  const string& s = get(Name_0f, op.data);
  return " ("+s.substr(0, s.find(" ("))+')';
}

string tolower(const char* s) {
  ostringstream out;
  for (/*nada*/;  *s;  ++s)
    out << static_cast<char>(tolower(*s));
  return out.str();
}

#undef HAS
#undef SET
#undef CLEAR


void test_check_bitfield_sizes() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "01/add 4/mod 3/rm32 1/r32\n"  // add ECX to EBX
  );
  CHECK_TRACE_CONTENTS(
      "error: '4/mod' too large to fit in bitfield mod\n"
  );
}

void check_operand_bounds(const segment& code) {
  trace(3, "transform") << "-- check operand bounds" << end();
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    const line& inst = code.lines.at(i);
    for (int j = first_operand(inst);  j < SIZE(inst.words);  ++j)
      check_operand_bounds(inst.words.at(j));
    if (trace_contains_errors()) return;  // stop at the first mal-formed instruction
  }
}

void check_operand_bounds(const word& w) {
  for (map<string, uint32_t>::iterator p = Operand_bound.begin();  p != Operand_bound.end();  ++p) {
    if (!has_operand_metadata(w, p->first)) continue;
    if (!looks_like_hex_int(w.data)) continue;  // later transforms are on their own to do their own bounds checking
    int32_t x = parse_int(w.data);
    if (x >= 0) {
      if (p->first == "disp8" || p->first == "disp16") {
        if (static_cast<uint32_t>(x) >= p->second/2)
          raise << "'" << w.original << "' too large to fit in signed bitfield " << p->first << '\n' << end();
      }
      else {
        if (static_cast<uint32_t>(x) >= p->second)
          raise << "'" << w.original << "' too large to fit in bitfield " << p->first << '\n' << end();
      }
    }
    else {
      // hacky? assuming bound is a power of 2
      if (x < -1*static_cast<int32_t>(p->second/2))
        raise << "'" << w.original << "' too large to fit in bitfield " << p->first << '\n' << end();
    }
  }
}

void test_check_bitfield_sizes_for_imm8() {
  run(
      "== 0x1\n"  // code segment
      "c1/shift 4/subop/left 3/mod/direct 1/rm32/ECX 0xff/imm8"  // shift EBX left
  );
  CHECK(!trace_contains_errors());
}

void test_check_bitfield_sizes_for_imm8_error() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "c1/shift 4/subop/left 3/mod/direct 1/rm32/ECX 0x100/imm8"  // shift EBX left
  );
  CHECK_TRACE_CONTENTS(
      "error: '0x100/imm8' too large to fit in bitfield imm8\n"
  );
}

void test_check_bitfield_sizes_for_negative_imm8() {
  run(
      "== 0x1\n"  // code segment
      "c1/shift 4/subop/left 3/mod/direct 1/rm32/ECX -0x80/imm8"  // shift EBX left
  );
  CHECK(!trace_contains_errors());
}

void test_check_bitfield_sizes_for_negative_imm8_error() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "c1/shift 4/subop/left 3/mod/direct 1/rm32/ECX -0x81/imm8"  // shift EBX left
  );
  CHECK_TRACE_CONTENTS(
      "error: '-0x81/imm8' too large to fit in bitfield imm8\n"
  );
}

void test_check_bitfield_sizes_for_disp8() {
  // not bothering to run
  transform(
      "== 0x1\n"  // code segment
      "01/add 1/mod/*+disp8 3/rm32 1/r32 0x7f/disp8\n"  // add ECX to *(EBX+0x7f)
  );
  CHECK(!trace_contains_errors());
}

void test_check_bitfield_sizes_for_disp8_error() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "01/add 1/mod/*+disp8 3/rm32 1/r32 0x80/disp8\n"  // add ECX to *(EBX+0x80)
  );
  CHECK_TRACE_CONTENTS(
      "error: '0x80/disp8' too large to fit in signed bitfield disp8\n"
  );
}

void test_check_bitfield_sizes_for_negative_disp8() {
  // not bothering to run
  transform(
      "== 0x1\n"  // code segment
      "01/add 1/mod/*+disp8 3/rm32 1/r32 -0x80/disp8\n"  // add ECX to *(EBX-0x80)
  );
  CHECK(!trace_contains_errors());
}

void test_check_bitfield_sizes_for_negative_disp8_error() {
  Hide_errors = true;
  run(
      "== 0x1\n"  // code segment
      "01/add 1/mod/*+disp8 3/rm32 1/r32 -0x81/disp8\n"  // add ECX to *(EBX-0x81)
  );
  CHECK_TRACE_CONTENTS(
      "error: '-0x81/disp8' too large to fit in bitfield disp8\n"
  );
}


void test_segment_name() {
  run(
      "== code\n"
      "05/add-to-EAX  0x0d0c0b0a/imm32\n"
      // code starts at 0x08048000 + p_offset, which is 0x54 for a single-segment binary
  );
  CHECK_TRACE_CONTENTS(
      "load: 0x09000054 -> 05\n"
      "load: 0x09000055 -> 0a\n"
      "load: 0x09000056 -> 0b\n"
      "load: 0x09000057 -> 0c\n"
      "load: 0x09000058 -> 0d\n"
      "run: add imm32 0x0d0c0b0a to reg EAX\n"
      "run: storing 0x0d0c0b0a\n"
  );
}


void test_repeated_segment_merges_data() {
  run(
      "== code\n"
      "05/add-to-EAX  0x0d0c0b0a/imm32\n"
      "== code\n"  // again
      "2d/subtract-from-EAX  0xddccbbaa/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse: new segment 'code'\n"
      "parse: appending to segment 'code'\n"
      // first segment
      "load: 0x09000054 -> 05\n"
      "load: 0x09000055 -> 0a\n"
      "load: 0x09000056 -> 0b\n"
      "load: 0x09000057 -> 0c\n"
      "load: 0x09000058 -> 0d\n"
      // second segment
      "load: 0x09000059 -> 2d\n"
      "load: 0x0900005a -> aa\n"
      "load: 0x0900005b -> bb\n"
      "load: 0x0900005c -> cc\n"
      "load: 0x0900005d -> dd\n"
  );
}

void test_error_on_missing_segment_header() {
  Hide_errors = true;
  run(
      "05/add-to-EAX 0/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: input does not start with a '==' section header\n"
  );
}

void test_error_on_first_segment_not_code() {
  Hide_errors = true;
  run(
      "== data\n"
      "05 00 00 00 00\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: first segment must be 'code' but is 'data'\n"
  );
}

void test_error_on_second_segment_not_data() {
  Hide_errors = true;
  run(
      "== code\n"
      "05/add-to-EAX 0/imm32\n"
      "== bss\n"
      "05 00 00 00 00\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: second segment must be 'data' but is 'bss'\n"
  );
}


void compute_segment_starts(program& p) {
  trace(3, "transform") << "-- compute segment addresses" << end();
  uint32_t p_offset = /*size of ehdr*/0x34 + SIZE(p.segments)*0x20/*size of each phdr*/;
  for (size_t i = 0;  i < p.segments.size();  ++i) {
    segment& curr = p.segments.at(i);
    if (curr.start == 0) {
      curr.start = CODE_SEGMENT + i*SPACE_FOR_SEGMENT + p_offset;
      trace(99, "transform") << "segment " << i << " begins at address 0x" << HEXWORD << curr.start << end();
    }
    p_offset += size_of(curr);
    assert(p_offset < SEGMENT_ALIGNMENT);  // for now we get less and less available space in each successive segment
  }
}

uint32_t size_of(const segment& s) {
  uint32_t sum = 0;
  for (int i = 0;  i < SIZE(s.lines);  ++i)
    sum += num_bytes(s.lines.at(i));
  return sum;
}

// Assumes all bitfields are packed.
uint32_t num_bytes(const line& inst) {
  uint32_t sum = 0;
  for (int i = 0;  i < SIZE(inst.words);  ++i)
    sum += size_of(inst.words.at(i));
  return sum;
}

int size_of(const word& w) {
  if (has_operand_metadata(w, "disp32") || has_operand_metadata(w, "imm32"))
    return 4;
  else if (has_operand_metadata(w, "disp16"))
    return 2;
  else if (is_label(w))
    return 0;

  // End size_of(word w) Special-cases
  else
    return 1;
}





void test_entry_label() {
  run(
      "== 0x1\n"  // code segment
      "05 0x0d0c0b0a/imm32\n"
      "Entry:\n"
      "05 0x0d0c0b0a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "run: 0x00000006 opcode: 05\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("run: 0x00000001 opcode: 05");
}

void test_pack_immediate_ignores_single_byte_nondigit_operand() {
  Hide_errors = true;
  transform(
      "== 0x1\n"  // code segment
      "b9/copy  a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'b9/copy a/imm32'\n"
      // no change (we're just not printing metadata to the trace)
      "transform: instruction after packing: 'b9 a'\n"
  );
}

void test_pack_immediate_ignores_3_hex_digit_operand() {
  Hide_errors = true;
  transform(
      "== 0x1\n"  // code segment
      "b9/copy  aaa/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'b9/copy aaa/imm32'\n"
      // no change (we're just not printing metadata to the trace)
      "transform: instruction after packing: 'b9 aaa'\n"
  );
}

void test_pack_immediate_ignores_non_hex_operand() {
  Hide_errors = true;
  transform(
      "== 0x1\n"  // code segment
      "b9/copy xxx/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: packing instruction 'b9/copy xxx/imm32'\n"
      // no change (we're just not printing metadata to the trace)
      "transform: instruction after packing: 'b9 xxx'\n"
  );
}

void check_valid_name(const string& s) {
  if (s.empty()) {
    raise << "empty name!\n" << end();
    return;
  }
  if (s.at(0) == '-')
    raise << "'" << s << "' starts with '-', which can be confused with a negative number; use a different name\n" << end();
  if (s.substr(0, 2) == "0x") {
    raise << "'" << s << "' looks like a hex number; use a different name\n" << end();
    return;
  }
  if (isdigit(s.at(0)))
    raise << "'" << s << "' starts with a digit, and so can be confused with a negative number; use a different name.\n" << end();
  if (SIZE(s) == 2)
    raise << "'" << s << "' is two characters long which can look like raw hex bytes at a glance; use a different name\n" << end();
}


void test_map_label() {
  transform(
      "== 0x1\n"  // code segment
      "loop:\n"
      "  05  0x0d0c0b0a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: label 'loop' is at address 1\n"
  );
}

void rewrite_labels(program& p) {
  trace(3, "transform") << "-- rewrite labels" << end();
  if (p.segments.empty()) return;
  segment& code = p.segments.at(0);
  map<string, int32_t> byte_index;  // values are unsigned, but we're going to do subtractions on them so they need to fit in 31 bits
  compute_byte_indices_for_labels(code, byte_index);
  if (trace_contains_errors()) return;
  drop_labels(code);
  if (trace_contains_errors()) return;
  replace_labels_with_displacements(code, byte_index);
  if (contains_key(byte_index, "Entry"))
    Entry_address = code.start + get(byte_index, "Entry");
}

void compute_byte_indices_for_labels(const segment& code, map<string, int32_t>& byte_index) {
  int current_byte = 0;
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    const line& inst = code.lines.at(i);
    for (int j = 0;  j < SIZE(inst.words);  ++j) {
      const word& curr = inst.words.at(j);
      // hack: if we have any operand metadata left after previous transforms,
      // deduce its size
      // Maybe we should just move this transform to before instruction
      // packing, and deduce the size of *all* operands. But then we'll also
      // have to deal with bitfields.
      if (has_operand_metadata(curr, "disp32") || has_operand_metadata(curr, "imm32")) {
        if (*curr.data.rbegin() == ':')
          raise << "'" << to_string(inst) << "': don't use ':' when jumping to labels\n" << end();
        current_byte += 4;
      }
      else if (has_operand_metadata(curr, "disp16")) {
        if (*curr.data.rbegin() == ':')
          raise << "'" << to_string(inst) << "': don't use ':' when jumping to labels\n" << end();
        current_byte += 2;
      }
      // automatically handle /disp8 and /imm8 here
      else if (*curr.data.rbegin() != ':') {
        ++current_byte;
      }
      else {
        string label = drop_last(curr.data);
        // ensure labels look sufficiently different from raw hex
        check_valid_name(label);
        if (trace_contains_errors()) return;
        if (contains_any_operand_metadata(curr))
          raise << "'" << to_string(inst) << "': label definition (':') not allowed in operand\n" << end();
        if (j > 0)
          raise << "'" << to_string(inst) << "': labels can only be the first word in a line.\n" << end();
        if (Map_file.is_open())
          Map_file << "0x" << HEXWORD << (code.start + current_byte) << ' ' << label << '\n';
        if (contains_key(byte_index, label) && label != "Entry") {
          raise << "duplicate label '" << label << "'\n" << end();
          return;
        }
        put(byte_index, label, current_byte);
        trace(99, "transform") << "label '" << label << "' is at address " << (current_byte+code.start) << end();
        // no modifying current_byte; label definitions won't be in the final binary
      }
    }
  }
}

void drop_labels(segment& code) {
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    line& inst = code.lines.at(i);
    vector<word>::iterator new_end = remove_if(inst.words.begin(), inst.words.end(), is_label);
    inst.words.erase(new_end, inst.words.end());
  }
}

bool is_label(const word& w) {
  return *w.data.rbegin() == ':';
}

void replace_labels_with_displacements(segment& code, const map<string, int32_t>& byte_index) {
  int32_t byte_index_next_instruction_starts_at = 0;
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    line& inst = code.lines.at(i);
    byte_index_next_instruction_starts_at += num_bytes(inst);
    line new_inst;
    for (int j = 0;  j < SIZE(inst.words);  ++j) {
      const word& curr = inst.words.at(j);
      if (contains_key(byte_index, curr.data)) {
        int32_t displacement = static_cast<int32_t>(get(byte_index, curr.data)) - byte_index_next_instruction_starts_at;
        if (has_operand_metadata(curr, "disp8")) {
          if (displacement > 0x7f || displacement < -0x7f)
            raise << "'" << to_string(inst) << "': label too far away for displacement " << std::hex << displacement << " to fit in 8 signed bits\n" << end();
          else
            emit_hex_bytes(new_inst, displacement, 1);
        }
        else if (has_operand_metadata(curr, "disp16")) {
          if (displacement > 0x7fff || displacement < -0x7fff)
            raise << "'" << to_string(inst) << "': label too far away for displacement " << std::hex << displacement << " to fit in 16 signed bits\n" << end();
          else
            emit_hex_bytes(new_inst, displacement, 2);
        }
        else if (has_operand_metadata(curr, "disp32")) {
          emit_hex_bytes(new_inst, displacement, 4);
        }
      }
      else {
        new_inst.words.push_back(curr);
      }
    }
    inst.words.swap(new_inst.words);
    trace(99, "transform") << "instruction after transform: '" << data_to_string(inst) << "'" << end();
  }
}

string data_to_string(const line& inst) {
  ostringstream out;
  for (int i = 0;  i < SIZE(inst.words);  ++i) {
    if (i > 0) out << ' ';
    out << inst.words.at(i).data;
  }
  return out.str();
}

string drop_last(const string& s) {
  return string(s.begin(), --s.end());
}


void test_multiple_labels_at() {
  transform(
      "== 0x1\n"  // code segment
      // address 1
      "loop:\n"
      " $loop2:\n"
      // address 1 (labels take up no space)
      "    05  0x0d0c0b0a/imm32\n"
      // address 6
      "    eb  $loop2/disp8\n"
      // address 8
      "    eb  $loop3/disp8\n"
      // address 0xa
      " $loop3:\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: label 'loop' is at address 1\n"
      "transform: label '$loop2' is at address 1\n"
      "transform: label '$loop3' is at address a\n"
      // first jump is to -7
      "transform: instruction after transform: 'eb f9'\n"
      // second jump is to 0 (fall through)
      "transform: instruction after transform: 'eb 00'\n"
  );
}

void test_duplicate_label() {
  Hide_errors = true;
  transform(
      "== 0x1\n"
      "loop:\n"
      "loop:\n"
      "    05  0x0d0c0b0a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: duplicate label 'loop'\n"
  );
}

void test_label_too_short() {
  Hide_errors = true;
  transform(
      "== 0x1\n"
      "xz:\n"
      "  05  0x0d0c0b0a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: 'xz' is two characters long which can look like raw hex bytes at a glance; use a different name\n"
  );
}

void test_label_hex() {
  Hide_errors = true;
  transform(
      "== 0x1\n"
      "0xab:\n"
      "  05  0x0d0c0b0a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '0xab' looks like a hex number; use a different name\n"
  );
}

void test_label_negative_hex() {
  Hide_errors = true;
  transform(
      "== 0x1\n"
      "-a:\n"
      "    05  0x0d0c0b0a/imm32\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: '-a' starts with '-', which can be confused with a negative number; use a different name\n"
  );
}


void test_segment_size_ignores_labels() {
  transform(
      "== code\n"  // 0x09000074
      "  05/add  0x0d0c0b0a/imm32\n"  // 5 bytes
      "foo:\n"                        // 0 bytes
      "== data\n"  // 0x0a000079
      "bar:\n"
      "  00\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: segment 1 begins at address 0x0a000079\n"
  );
}


void test_global_variable() {
  run(
      "== code\n"
      "b9  x/imm32\n"
      "== data\n"
      "x:\n"
      "  00 00 00 00\n"
  );
  CHECK_TRACE_CONTENTS(
      "transform: global variable 'x' is at address 0x0a000079\n"
  );
}

void rewrite_global_variables(program& p) {
  trace(3, "transform") << "-- rewrite global variables" << end();
  insert_heap_global_variable(p);
  // Begin rewrite_global_variables
  map<string, uint32_t> address;
  compute_addresses_for_global_variables(p, address);
  if (trace_contains_errors()) return;
  drop_global_variables(p);
  replace_global_variables_with_addresses(p, address);
}

void compute_addresses_for_global_variables(const program& p, map<string, uint32_t>& address) {
  for (int i = /*skip code segment*/1;  i < SIZE(p.segments);  ++i)
    compute_addresses_for_global_variables(p.segments.at(i), address);
}

void compute_addresses_for_global_variables(const segment& s, map<string, uint32_t>& address) {
  int current_address = s.start;
  for (int i = 0;  i < SIZE(s.lines);  ++i) {
    const line& inst = s.lines.at(i);
    for (int j = 0;  j < SIZE(inst.words);  ++j) {
      const word& curr = inst.words.at(j);
      if (*curr.data.rbegin() != ':') {
        current_address += size_of(curr);
      }
      else {
        string variable = drop_last(curr.data);
        // ensure variables look sufficiently different from raw hex
        check_valid_name(variable);
        if (trace_contains_errors()) return;
        if (j > 0)
          raise << "'" << to_string(inst) << "': global variable names can only be the first word in a line.\n" << end();
        if (Map_file.is_open())
          Map_file << "0x" << HEXWORD << current_address << ' ' << variable << '\n';
        if (contains_key(address, variable)) {
          raise << "duplicate global '" << variable << "'\n" << end();
          return;
        }
        put(address, variable, current_address);
        trace(99, "transform") << "global variable '" << variable << "' is at address 0x" << HEXWORD << current_address << end();
        // no modifying current_address; global variable definitions won't be in the final binary
      }
    }
  }
}

void drop_global_variables(program& p) {
  for (int i = /*skip code segment*/1;  i < SIZE(p.segments);  ++i)
    drop_labels(p.segments.at(i));
}

void replace_global_variables_with_addresses(program& p, const map<string, uint32_t>& address) {
  if (p.segments.empty()) return;
  replace_global_variables_in_code_segment(p.segments.at(0), address);
  for (int i = /*skip code*/1;  i < SIZE(p.segments);  ++i)
    replace_global_variables_in_data_segment(p.segments.at(i), address);
}

void replace_global_variables_in_code_segment(segment& code, const map<string, uint32_t>& address) {
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    line& inst = code.lines.at(i);
    line new_inst;
    for (int j = 0;  j < SIZE(inst.words);  ++j) {
      const word& curr = inst.words.at(j);
      if (!contains_key(address, curr.data)) {
        if (!looks_like_hex_int(curr.data))
          raise << "missing reference to global '" << curr.data << "'\n" << end();
        new_inst.words.push_back(curr);
        continue;
      }
      if (!valid_use_of_global_variable(curr)) {
        raise << "'" << to_string(inst) << "': can't refer to global variable '" << curr.data << "'\n" << end();
        return;
      }
      emit_hex_bytes(new_inst, get(address, curr.data), 4);
    }
    inst.words.swap(new_inst.words);
    trace(99, "transform") << "instruction after transform: '" << data_to_string(inst) << "'" << end();
  }
}

void replace_global_variables_in_data_segment(segment& data, const map<string, uint32_t>& address) {
  for (int i = 0;  i < SIZE(data.lines);  ++i) {
    line& l = data.lines.at(i);
    line new_l;
    for (int j = 0;  j < SIZE(l.words);  ++j) {
      const word& curr = l.words.at(j);
      if (!contains_key(address, curr.data)) {
        if (looks_like_hex_int(curr.data)) {
          if (has_operand_metadata(curr, "imm32"))
            emit_hex_bytes(new_l, curr, 4);
          else if (has_operand_metadata(curr, "imm16"))
            emit_hex_bytes(new_l, curr, 2);
          else if (has_operand_metadata(curr, "imm8"))
            emit_hex_bytes(new_l, curr, 1);
          else if (has_operand_metadata(curr, "disp8"))
            raise << "can't use /disp8 in a non-code segment\n" << end();
          else if (has_operand_metadata(curr, "disp16"))
            raise << "can't use /disp16 in a non-code segment\n" << end();
          else if (has_operand_metadata(curr, "disp32"))
            raise << "can't use /disp32 in a non-code segment\n" << end();
          else
            new_l.words.push_back(curr);
        }
        else {
          raise << "missing reference to global '" << curr.data << "'\n" << end();
          new_l.words.push_back(curr);
        }
        continue;
      }
      trace(99, "transform") << curr.data << " maps to " << HEXWORD << get(address, curr.data) << end();
      emit_hex_bytes(new_l, get(address, curr.data), 4);
    }
    l.words.swap(new_l.words);
    trace(99, "transform") << "after transform: '" << data_to_string(l) << "'" << end();
  }
}

bool valid_use_of_global_variable(const word& curr) {
  if (has_operand_metadata(curr, "imm32")) return true;
  if (has_operand_metadata(curr, "disp32"))
    return has_metadata(curr, "has_mod");
  // todo: more sophisticated check, to ensure we don't use global variable
  // addresses as a real displacement added to other operands.

  // End Valid Uses Of Global Variable(curr)
  return false;
}


void correlate_disp32_with_mod(program& p) {
  if (p.segments.empty()) return;
  segment& code = p.segments.at(0);
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    line& inst = code.lines.at(i);
    for (int j = 0;  j < SIZE(inst.words);  ++j) {
      word& curr = inst.words.at(j);
      if (has_operand_metadata(curr, "disp32")
          && has_operand_metadata(inst, "mod"))
        curr.metadata.push_back("has_mod");
    }
  }
}

bool has_metadata(const word& w, const string& m) {
  for (int i = 0;  i < SIZE(w.metadata);  ++i)
    if (w.metadata.at(i) == m) return true;
  return false;
}

void test_global_variable_disallowed_in_jump() {
  Hide_errors = true;
  run(
      "== code\n"
      "eb/jump  x/disp8\n"
      "== data\n"
      "x:\n"
      "  00 00 00 00\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: 'eb/jump x/disp8': can't refer to global variable 'x'\n"
      // sub-optimal error message; should be
//?       "error: can't jump to data (variable 'x')\n"
  );
}

void test_global_variable_disallowed_in_call() {
  Hide_errors = true;
  run(
      "== code\n"
      "e8/call  x/disp32\n"
      "== data\n"
      "x:\n"
      "  00 00 00 00\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: 'e8/call x/disp32': can't refer to global variable 'x'\n"
      // sub-optimal error message; should be
//?       "error: can't call to the data segment ('x')\n"
  );
}

void test_global_variable_in_data_segment() {
  run(
      "== 0x1\n"
      "b9  x/imm32\n"
      "== 0x0a000000\n"
      "x:\n"
      "  y/imm32\n"
      "y:\n"
      "  00 00 00 00\n"
  );
  // check that we loaded 'x' with the address of 'y'
  CHECK_TRACE_CONTENTS(
      "load: 0x0a000000 -> 04\n"
      "load: 0x0a000001 -> 00\n"
      "load: 0x0a000002 -> 00\n"
      "load: 0x0a000003 -> 0a\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

void test_raw_number_with_imm32_in_data_segment() {
  run(
      "== 0x1\n"
      "b9  x/imm32\n"
      "== 0x0a000000\n"
      "x:\n"
      "  1/imm32\n"
  );
  // check that we loaded 'x' with the address of 1
  CHECK_TRACE_CONTENTS(
      "load: 0x0a000000 -> 01\n"
      "load: 0x0a000001 -> 00\n"
      "load: 0x0a000002 -> 00\n"
      "load: 0x0a000003 -> 00\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

void test_duplicate_global_variable() {
  Hide_errors = true;
  run(
      "== 0x1\n"
      "40/increment-EAX\n"
      "== 0x0a000000\n"
      "x:\n"
      "x:\n"
      "  00\n"
  );
  CHECK_TRACE_CONTENTS(
      "error: duplicate global 'x'\n"
  );
}

void test_global_variable_disp32_with_modrm() {
  run(
      "== code\n"
      "8b/copy 0/mod/indirect 5/rm32/.disp32 2/r32/EDX x/disp32\n"
      "== data\n"
      "x:\n"
      "  00 00 00 00\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

void test_global_variable_disp32_with_call() {
  transform(
      "== code\n"
      "foo:\n"
      "  e8/call bar/disp32\n"
      "bar:\n"
  );
  CHECK_TRACE_COUNT("error", 0);
}

string to_full_string(const line& in) {
  ostringstream out;
  for (int i = 0;  i < SIZE(in.words);  ++i) {
    if (i > 0) out << ' ';
    out << in.words.at(i).data;
    for (int j = 0;  j < SIZE(in.words.at(i).metadata);  ++j)
      out << '/' << in.words.at(i).metadata.at(j);
  }
  return out.str();
}


void insert_heap_global_variable(program& p) {
  if (SIZE(p.segments) < 2)
    return;  // no data segment defined
  // Start-of-heap:
  p.segments.at(1).lines.push_back(label("Start-of-heap"));
}

line label(string s) {
  line result;
  result.words.push_back(word());
  result.words.back().data = (s+":");
  return result;
}

line imm32(const string& s) {
  line result;
  result.words.push_back(word());
  result.words.back().data = s;
  result.words.back().metadata.push_back("imm32");
  return result;
}


void test_transform_literal_string() {
  run(
      "== code\n"
      "b8/copy  \"test\"/imm32\n"
      "== data\n"  // need to manually create the segment for now
  );
  CHECK_TRACE_CONTENTS(
      "transform: -- move literal strings to data segment\n"
      "transform: adding global variable '__subx_global_1' containing \"test\"\n"
      "transform: instruction after transform: 'b8 __subx_global_1'\n"
  );
}

void transform_literal_strings(program& p) {
  trace(3, "transform") << "-- move literal strings to data segment" << end();
  if (p.segments.empty()) return;
  segment& code = p.segments.at(0);
  segment data;
  for (int i = 0;  i < SIZE(code.lines);  ++i) {
    line& inst = code.lines.at(i);
    for (int j = 0;  j < SIZE(inst.words);  ++j) {
      word& curr = inst.words.at(j);
      if (curr.data.at(0) != '"') continue;
      ostringstream global_name;
      global_name << "__subx_global_" << Next_auto_global;
      ++Next_auto_global;
      add_global_to_data_segment(global_name.str(), curr, data);
      curr.data = global_name.str();
    }
    trace(99, "transform") << "instruction after transform: '" << data_to_string(inst) << "'" << end();
  }
  if (data.lines.empty()) return;
  if (SIZE(p.segments) < 2) {
    p.segments.resize(2);
    p.segments.at(1).lines.swap(data.lines);
  }
  vector<line>& existing_data = p.segments.at(1).lines;
  existing_data.insert(existing_data.end(), data.lines.begin(), data.lines.end());
}

void add_global_to_data_segment(const string& name, const word& value, segment& data) {
  trace(99, "transform") << "adding global variable '" << name << "' containing " << value.data << end();
  // emit label
  data.lines.push_back(label(name));
  // emit size for size-prefixed array
  data.lines.push_back(line());
  emit_hex_bytes(data.lines.back(), SIZE(value.data)-/*skip quotes*/2, 4/*bytes*/);
  // emit data byte by byte
  data.lines.push_back(line());
  line& curr = data.lines.back();
  for (int i = /*skip start quote*/1;  i < SIZE(value.data)-/*skip end quote*/1;  ++i) {
    char c = value.data.at(i);
    curr.words.push_back(word());
    curr.words.back().data = hex_byte_to_string(c);
    curr.words.back().metadata.push_back(string(1, c));
  }
}


void test_instruction_with_string_literal() {
  parse_instruction_character_by_character(
      "a \"abc  def\" z\n"  // two spaces inside string
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: \"abc  def\"\n"
      "parse2: word: z\n"
  );
  // no other words
  CHECK_TRACE_COUNT("parse2", 3);
}

void parse_instruction_character_by_character(const string& line_data, vector<line>& out) {
  if (line_data.find('\n') != string::npos  && line_data.find('\n') != line_data.size()-1) {
    raise << "parse_instruction_character_by_character: should receive only a single line\n" << end();
    return;
  }
  // parse literals
  istringstream in(line_data);
  in >> std::noskipws;
  line result;
  // add tokens (words or strings) one by one
  while (has_data(in)) {
    skip_whitespace(in);
    if (!has_data(in)) break;
    char c = in.get();
    if (c == '#') break;  // comment; drop rest of line
    if (c == ':') break;  // line metadata; skip for now
    if (c == '.') {
      if (!has_data(in)) break;  // comment token at end of line
      if (isspace(in.peek()))
        continue;  // '.' followed by space is comment token; skip
    }
    result.words.push_back(word());
    if (c == '"') {
      // slurp word data
      ostringstream d;
      d << c;
      while (has_data(in)) {
        in >> c;
        d << c;
        if (c == '"') break;
      }
      result.words.back().data = d.str();
      // slurp metadata
      ostringstream m;
      while (!isspace(in.peek()) && has_data(in)) {
        in >> c;
        if (c == '/') {
          if (!m.str().empty()) result.words.back().metadata.push_back(m.str());
          m.str("");
        }
        else {
          m << c;
        }
      }
      if (!m.str().empty()) result.words.back().metadata.push_back(m.str());
    }
    else {
      // slurp all characters until whitespace
      ostringstream w;
      w << c;
      while (!isspace(in.peek()) && has_data(in)) {  // peek can sometimes trigger eof(), so do it first
        in >> c;
        w << c;
      }
      parse_word(w.str(), result.words.back());
    }
    trace(99, "parse2") << "word: " << to_string(result.words.back()) << end();
  }
  if (!result.words.empty())
    out.push_back(result);
}

void skip_whitespace(istream& in) {
  while (true) {
    if (has_data(in) && isspace(in.peek())) in.get();
    else break;
  }
}

void skip_comment(istream& in) {
  if (has_data(in) && in.peek() == '#') {
    in.get();
    while (has_data(in) && in.peek() != '\n') in.get();
  }
}

// helper for tests
void parse_instruction_character_by_character(const string& line_data) {
  vector<line> out;
  parse_instruction_character_by_character(line_data, out);
}

void test_parse2_comment_token_in_middle() {
  parse_instruction_character_by_character(
      "a . z\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: z\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("parse2: word: .");
  // no other words
  CHECK_TRACE_COUNT("parse2", 2);
}

void test_parse2_word_starting_with_dot() {
  parse_instruction_character_by_character(
      "a .b c\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: .b\n"
      "parse2: word: c\n"
  );
}

void test_parse2_comment_token_at_start() {
  parse_instruction_character_by_character(
      ". a b\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: b\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("parse2: word: .");
}

void test_parse2_comment_token_at_end() {
  parse_instruction_character_by_character(
      "a b .\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: b\n"
  );
  CHECK_TRACE_DOESNT_CONTAIN("parse2: word: .");
}

void test_parse2_word_starting_with_dot_at_start() {
  parse_instruction_character_by_character(
      ".a b c\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: .a\n"
      "parse2: word: b\n"
      "parse2: word: c\n"
  );
}

void test_parse2_metadata() {
  parse_instruction_character_by_character(
      ".a b/c d\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: .a\n"
      "parse2: word: b /c\n"
      "parse2: word: d\n"
  );
}

void test_parse2_string_with_metadata() {
  parse_instruction_character_by_character(
      "a \"bc  def\"/disp32 g\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: \"bc  def\" /disp32\n"
      "parse2: word: g\n"
  );
}

void test_parse2_string_with_metadata_at_end() {
  parse_instruction_character_by_character(
      "a \"bc  def\"/disp32\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: a\n"
      "parse2: word: \"bc  def\" /disp32\n"
  );
}

void test_parse2_string_with_metadata_at_end_of_line_without_newline() {
  parse_instruction_character_by_character(
      "68/push \"test\"/f"  // no newline, which is how calls from parse() will look
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: 68 /push\n"
      "parse2: word: \"test\" /f\n"
  );
}


void test_parse2_string_containing_slashes() {
  parse_instruction_character_by_character(
      "a \"bc/def\"/disp32\n"
  );
  CHECK_TRACE_CONTENTS(
      "parse2: word: \"bc/def\" /disp32\n"
  );
}

