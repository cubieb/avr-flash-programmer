/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: gperf --slot-name=cmd --output-file=shell_hash.h --compare-strncmp shell.gperf  */
/* Computed positions: -k'1' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "shell.gperf"

#include <string.h>
#include "shell.h"

#line 6 "shell.gperf"
struct cmd_config{
	const char *cmd;
	int (*func)(void *data);
};

#define TOTAL_KEYWORDS 9
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 21
/* maximum key range = 18, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
shell_hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 15, 10,
       5, 15, 22, 22, 10, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22,  0, 22,  5, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22
    };
  return len + asso_values[(unsigned char)str[0]];
}

struct stringpool_t
  {
    char stringpool_str4[sizeof("stat")];
    char stringpool_str6[sizeof("serase")];
    char stringpool_str9[sizeof("dump")];
    char stringpool_str11[sizeof("upload")];
    char stringpool_str13[sizeof("download")];
    char stringpool_str14[sizeof("help")];
    char stringpool_str16[sizeof("cerase")];
    char stringpool_str19[sizeof("echo")];
    char stringpool_str21[sizeof("berase")];
  };
static const struct stringpool_t stringpool_contents =
  {
    "stat",
    "serase",
    "dump",
    "upload",
    "download",
    "help",
    "cerase",
    "echo",
    "berase"
  };
#define stringpool ((const char *) &stringpool_contents)

static const struct cmd_config wordlist[] =
  {
    {-1}, {-1}, {-1}, {-1},
#line 24 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str4, flash_read_status},
    {-1},
#line 26 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str6, flash_serase},
    {-1}, {-1},
#line 25 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str9, flash_dump},
    {-1},
#line 22 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str11, file_upload},
    {-1},
#line 23 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str13, file_download},
#line 20 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str14, show_help},
    {-1},
#line 28 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str16, flash_cerase},
    {-1}, {-1},
#line 21 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str19, echo_string},
    {-1},
#line 27 "shell.gperf"
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str21, flash_berase}
  };

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct cmd_config *
shell_lookup (register const char *str, register unsigned int len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = shell_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int o = wordlist[key].cmd;
          if (o >= 0)
            {
              register const char *s = o + stringpool;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist[key];
            }
        }
    }
  return 0;
}
