%{
#include "boole.h"
#include "y.tab.h"
%}


%%


[ \t\f] {
  ;
}

\n {
  ++line_number;
  prompt();
}

"--".*\n {
  ++line_number;
  prompt();
}

";" {
  return (';');
}

"(" {
  return ('(');
}

")" {
  return (')');
}

":=" {
  return (GETS);
}

"[=" {
  return (AGETS);
}

"=" {
  return ('=');
}

"&" {
  return ('&');
}

"*" {
  return ('&');
}

"|" {
  return ('|');
}

"+" {
  return ('|');
}

"^" {
  return ('^');
}

"!" {
  return ('!');
}

"~" {
  return ('!');
}

"[" {
  return ('[');
}

"]" {
  return (']');
}

"0" {
  return ('0');
}

"1" {
  return ('1');
}

"?" {
  return ('?');
}

":" {
  return (':');
}

[0-9]+ {
  sscanf(yytext, "%ld", &yylval.i);
  return (NUM);
}

[-.A-Za-z0-9_$%#@/\<>]* {
  st_entry entry;

  entry=(st_entry)hash_lookup(symbol_table, (pointer)yytext);
  if (!entry)
    {
      entry=(st_entry)mem_get_block((long)sizeof(struct st_entry_));
      entry->type=NEW;
      entry->id=copy_str(yytext);
      hash_insert(symbol_table, (pointer)entry->id, (pointer)entry);
      yylval.e=entry;
      return (NEWID);
    }
  if (entry->type == KEYWORD)
    return (entry->info.keyword.code);
  yylval.e=entry;
  if (entry->type == NEW)
    /* Entry exists, but NEW tag means we're supposed to pretend it */
    /* doesn't. */
    return (NEWID);
  return (ID);
}

. {
  return (UNKNOWNCHAR);
}
