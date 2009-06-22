/**
(The MIT License)

Copyright © 2009 Evan Phoenix

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ‘Software’), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ‘AS IS’, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "ruby.h"

#ifndef RSTRING_PTR(obj)
#define RSTRING_PTR(obj) RSTRING(obj)->ptr
#endif

#ifndef RSTRING_LEN(obj)
#define RSTRING_LEN(obj) RSTRING(obj)->len
#endif

const char space[] = { ' ' };
const char plus[] =  { '+' };

static inline char hex(char digit) {
  if(digit >= '0' && digit <= '9') {
    return digit - '0';
  } else if(digit >= 'a' && digit <= 'f') {
    return 10 + digit - 'a';
  } else if(digit >= 'A' && digit <= 'F') {
    return 10 + digit - 'A';
  }

  return -1;
}

static VALUE unescape(VALUE self, VALUE str) {
  StringValue(str);

  const char* buf = RSTRING_PTR(str);
  const char* bufend = buf + RSTRING_LEN(str);

  VALUE outstr = rb_str_buf_new(RSTRING_LEN(str));

  while(buf < bufend) {
    if(buf[0] == '%' && buf + 2 <= bufend) {
      char high = hex(buf[1]);
      char low =  hex(buf[2]);

      if(high >= 0 && low >= 0) {
        const char byte = low + (high << 4);
        rb_str_buf_cat(outstr, &byte, 1);
        buf += 3;
        continue;
      }
    }

    if(buf[0] == '+') {
      rb_str_buf_cat(outstr, space, 1);
    } else {
      rb_str_buf_cat(outstr, buf, 1);
    }
    buf++;
  }

  return outstr;
}

const char hex_table[][4] = {
  "%00", "%01", "%02", "%03", "%04", "%05", "%06", "%07",
  "%08", "%09", "%0a", "%0b", "%0c", "%0d", "%0e", "%0f",
  "%10", "%11", "%12", "%13", "%14", "%15", "%16", "%17",
  "%18", "%19", "%1a", "%1b", "%1c", "%1d", "%1e", "%1f",
  "%20", "%21", "%22", "%23", "%24", "%25", "%26", "%27",
  "%28", "%29", "%2a", "%2b", "%2c", "%2d", "%2e", "%2f",
  "%30", "%31", "%32", "%33", "%34", "%35", "%36", "%37",
  "%38", "%39", "%3a", "%3b", "%3c", "%3d", "%3e", "%3f",
  "%40", "%41", "%42", "%43", "%44", "%45", "%46", "%47",
  "%48", "%49", "%4a", "%4b", "%4c", "%4d", "%4e", "%4f",
  "%50", "%51", "%52", "%53", "%54", "%55", "%56", "%57",
  "%58", "%59", "%5a", "%5b", "%5c", "%5d", "%5e", "%5f",
  "%60", "%61", "%62", "%63", "%64", "%65", "%66", "%67",
  "%68", "%69", "%6a", "%6b", "%6c", "%6d", "%6e", "%6f",
  "%70", "%71", "%72", "%73", "%74", "%75", "%76", "%77",
  "%78", "%79", "%7a", "%7b", "%7c", "%7d", "%7e", "%7f",
  "%80", "%81", "%82", "%83", "%84", "%85", "%86", "%87",
  "%88", "%89", "%8a", "%8b", "%8c", "%8d", "%8e", "%8f",
  "%90", "%91", "%92", "%93", "%94", "%95", "%96", "%97",
  "%98", "%99", "%9a", "%9b", "%9c", "%9d", "%9e", "%9f",
  "%a0", "%a1", "%a2", "%a3", "%a4", "%a5", "%a6", "%a7",
  "%a8", "%a9", "%aa", "%ab", "%ac", "%ad", "%ae", "%af",
  "%b0", "%b1", "%b2", "%b3", "%b4", "%b5", "%b6", "%b7",
  "%b8", "%b9", "%ba", "%bb", "%bc", "%bd", "%be", "%bf",
  "%c0", "%c1", "%c2", "%c3", "%c4", "%c5", "%c6", "%c7",
  "%c8", "%c9", "%ca", "%cb", "%cc", "%cd", "%ce", "%cf",
  "%d0", "%d1", "%d2", "%d3", "%d4", "%d5", "%d6", "%d7",
  "%d8", "%d9", "%da", "%db", "%dc", "%dd", "%de", "%df",
  "%e0", "%e1", "%e2", "%e3", "%e4", "%e5", "%e6", "%e7",
  "%e8", "%e9", "%ea", "%eb", "%ec", "%ed", "%ee", "%ef",
  "%f0", "%f1", "%f2", "%f3", "%f4", "%f5", "%f6", "%f7",
  "%f8", "%f9", "%fa", "%fb", "%fc", "%fd", "%fe", "%ff"
};

static VALUE escape(VALUE self, VALUE str)
{
  StringValue(str);

  const char* buf = RSTRING_PTR(str);
  VALUE outstr = rb_str_buf_new(RSTRING_LEN(str));
  int len = RSTRING_LEN(str);
  int i;
  char lx, mx, nx;
  char l, m;

  for(i = 0; i < len; i++) {
    lx = *buf;
    l = (lx >> 4) & 0xf;
    /* (UTF-8 escape, 0x0000-0x007F) */
    if(0x0 <= l && l <= 0x7) {
      if(('A' <= lx && lx <= 'Z') || ('a' <= lx && lx <= 'z')) {
        rb_str_buf_cat(outstr, (char *) &lx, 1);
      } else if('0' <= lx && lx <= '9') {
        rb_str_buf_cat(outstr, (char *) &lx, 1);
      } else if(lx == ' ') {
        rb_str_buf_cat(outstr, plus, 1); // TODO should be a + or %20 replacement
      } else if(lx == '-' || lx == '_'
          || lx == '.' || lx == '!'
          || lx == '~' || lx == '*'
          || lx == '\'' || lx == '('
          || lx == ')') {
        rb_str_buf_cat(outstr, (char *) &lx, 1);
      }
    }
    else {
      buf++;
      i++;
      mx = *buf;
      m = (mx >> 4) & 0xf;

      /* (UTF-8 escape, 0x0080-0x07FF) */
      if(0xc <= l && l <= 0xd && 0x8 <= m && m <= 0xb) {
        rb_str_buf_cat(outstr, (char *) &hex_table[lx & 0xff], 3);
        rb_str_buf_cat(outstr, (char *) &hex_table[mx & 0xff], 3);
      }
      else {
        buf++;
        i++;
        nx = *buf;

        /* (UTF-8 escape, 0x0800-0xFFFF) */
        if(0xe == l && 0x8 <= m && m <= 0xb) {
          rb_str_buf_cat(outstr, (char *) &hex_table[lx & 0xff], 3);
          rb_str_buf_cat(outstr, (char *) &hex_table[mx & 0xff], 3);
          rb_str_buf_cat(outstr, (char *) &hex_table[nx & 0xff], 3);
        }
        /* (ISO Latin-1/2/? escape, 0x0080 - x00FF) */
        else if(0x8 <= l && l <= 0xf) {
          rb_str_buf_cat(outstr, (char *) &hex_table[lx & 0xff], 3);
          buf -= 2;
          i -= 2;
        }
      }
    }

    buf++;
  }

  return outstr;
}

void Init_url_escape() {
  VALUE mod = rb_define_module("URLEscape");
  rb_define_singleton_method(mod, "unescape", unescape, 1);
  rb_define_singleton_method(mod, "escape",   escape,   1);
}
