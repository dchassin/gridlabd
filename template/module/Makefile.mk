pkglib_LTLIBRARIES += {{{module}}}/{{{module}}}.la

{{{module}}}_{{{module}}}_la_CPPFLAGS =
{{{module}}}_{{{module}}}_la_CPPFLAGS += $(AM_CPPFLAGS)

{{{module}}}_{{{module}}}_la_LDFLAGS =
{{{module}}}_{{{module}}}_la_LDFLAGS += $(AM_LDFLAGS)

{{{module}}}_{{{module}}}_la_LIBADD = 

{{{module}}}_{{{module}}}_la_SOURCES =
{{{module}}}_{{{module}}}_la_SOURCES += {{{module}}}/main.cpp {{{module}}}/{{{module}}}.h
# {{{module}}}_{{{module}}}_la_SOURCES += {{{module}}}/{{{class1}}}.cpp {{{module}}}/{{{class1}}}.h
# {{{module}}}_{{{module}}}_la_SOURCES += {{{module}}}/{{{class2}}}.cpp {{{module}}}/{{{class2}}}.h
