# Doxyfile 1.8.14

DOXYFILE_ENCODING      = UTF-8
PROJECT_NAME           = @PROJECTNAME@
PROJECT_NUMBER         = @ANTIQUACRM_VERSION_STRING@
PROJECT_BRIEF          = "@DESCRIPTION@"
PROJECT_LOGO           = @CMAKE_CURRENT_SOURCE_DIR@/icons/database.png
OUTPUT_DIRECTORY       = @CMAKE_CURRENT_BINARY_DIR@/doc
CREATE_SUBDIRS         = Yes
ALLOW_UNICODE_NAMES    = NO
OUTPUT_LANGUAGE        = English
BRIEF_MEMBER_DESC      = YES
REPEAT_BRIEF           = YES

# This tag implements a quasi-intelligent brief description abbreviator that is
# used to form the text in various listings. Each string in this list, if found
# as the leading text of the brief description, will be stripped from the text
# and the result, after processing the whole list, is used as the annotated
# text. Otherwise, the brief description is used as-is. If left blank, the
# following values are used ($name is automatically replaced with the name of
# the entity):The $name class, The $name widget, The $name file, is, provides,
# specifies, contains, represents, a, an and the.

ABBREVIATE_BRIEF       = "The $name class" \
  "The $name widget" \
  "The $name file" \
  is \
  provides \
  specifies \
  contains \
  represents \
  a \
  an \
  the

ALWAYS_DETAILED_SEC    = YES
INLINE_INHERITED_MEMB  = NO
FULL_PATH_NAMES        = NO
STRIP_FROM_PATH        =
STRIP_FROM_INC_PATH    =
SHORT_NAMES            = NO
JAVADOC_AUTOBRIEF      = NO
MULTILINE_CPP_IS_BRIEF = NO
INHERIT_DOCS           = YES
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 2

ALIASES                = \
  "obsolete=@deprecated" \
  "FIXME=\xrefitem fixme \"Fixme\" \"Fixme\"" \
  "qpl=<a href=\"http://www.trolltech.com/products/qt/licenses\">QPL</a>" \
  "homepage=<a href=\"https://www.hjcms.de\">HJCMS</a>"

OPTIMIZE_OUTPUT_FOR_C  = NO
OPTIMIZE_OUTPUT_JAVA   = NO
BUILTIN_STL_SUPPORT    = NO
DISTRIBUTE_GROUP_DOC   = NO
SUBGROUPING            = NO

EXTENSION_MAPPING      =

MARKDOWN_SUPPORT       = YES
TOC_INCLUDE_HEADINGS   = 0
AUTOLINK_SUPPORT       = NO
BUILTIN_STL_SUPPORT    = NO
CPP_CLI_SUPPORT        = NO
SIP_SUPPORT            = NO
IDL_PROPERTY_SUPPORT   = YES
DISTRIBUTE_GROUP_DOC   = YES
GROUP_NESTED_COMPOUNDS = NO
SUBGROUPING            = YES
INLINE_GROUPED_CLASSES = YES
INLINE_SIMPLE_STRUCTS  = NO
TYPEDEF_HIDES_STRUCT   = NO
LOOKUP_CACHE_SIZE      = 0

#---------------------------------------------------------------------------
# Build related configuration options
#---------------------------------------------------------------------------

EXTRACT_ALL            = NO
EXTRACT_PRIVATE        = NO
EXTRACT_PACKAGE        = NO
EXTRACT_STATIC         = NO
EXTRACT_LOCAL_CLASSES  = YES
EXTRACT_LOCAL_METHODS  = YES
EXTRACT_ANON_NSPACES   = NO
HIDE_UNDOC_MEMBERS     = YES
HIDE_UNDOC_CLASSES     = NO
HIDE_FRIEND_COMPOUNDS  = NO
HIDE_IN_BODY_DOCS      = YES
INTERNAL_DOCS          = NO
CASE_SENSE_NAMES       = YES
HIDE_SCOPE_NAMES       = NO
HIDE_COMPOUND_REFERENCE= NO
SHOW_INCLUDE_FILES     = YES
SHOW_GROUPED_MEMB_INC  = NO
FORCE_LOCAL_INCLUDES   = NO
INLINE_INFO            = NO
SORT_MEMBER_DOCS       = NO
SORT_BRIEF_DOCS        = NO
SORT_MEMBERS_CTORS_1ST = NO
SORT_GROUP_NAMES       = YES
SORT_BY_SCOPE_NAME     = YES
STRICT_PROTO_MATCHING  = NO
GENERATE_TODOLIST      = NO
GENERATE_TESTLIST      = NO
GENERATE_BUGLIST       = NO
GENERATE_DEPRECATEDLIST= NO
MAX_INITIALIZER_LINES  = 30
SHOW_USED_FILES        = YES
SHOW_FILES             = YES
SHOW_NAMESPACES        = YES

#---------------------------------------------------------------------------
# Configuration options related to warning and progress messages
#---------------------------------------------------------------------------

QUIET                  = NO
WARNINGS               = YES
WARN_IF_UNDOCUMENTED   = NO
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = NO
WARN_AS_ERROR          = NO
WARN_FORMAT            = "$file:$line: $text"
WARN_LOGFILE           = @CMAKE_CURRENT_BINARY_DIR@/doc/doxygen_warnings.log

#---------------------------------------------------------------------------
# Configuration options related to the input files
#---------------------------------------------------------------------------

INPUT                  = @CMAKE_CURRENT_SOURCE_DIR@ \
 @CMAKE_CURRENT_SOURCE_DIR@/lib \
 @CMAKE_CURRENT_BINARY_DIR@ \
 @CMAKE_CURRENT_SOURCE_DIR@/widgets \
 @CMAKE_CURRENT_SOURCE_DIR@/plugins \
 @CMAKE_CURRENT_SOURCE_DIR@/tabs \
 @CMAKE_CURRENT_SOURCE_DIR@/utils

INPUT_ENCODING         = UTF-8

FILE_PATTERNS          = *.cpp \
                         *.h \
                         *.dox

EXCLUDE_PATTERNS       = moc_*.cpp \
 ui_*.cpp \
 qrc_*.cpp

RECURSIVE              = YES
EXCLUDE                = @CMAKE_CURRENT_SOURCE_DIR@/plugins/antiquacrm \
 @CMAKE_CURRENT_SOURCE_DIR@/widgets/templates \
 @CMAKE_CURRENT_SOURCE_DIR@/assistant

EXCLUDE_SYMLINKS       = YES
EXCLUDE_SYMBOLS        =
EXAMPLE_PATH           =
EXAMPLE_PATTERNS       = *
EXAMPLE_RECURSIVE      = NO
IMAGE_PATH             = @CMAKE_CURRENT_SOURCE_DIR@/icons
INPUT_FILTER           =
FILTER_PATTERNS        =
FILTER_SOURCE_FILES    = NO
FILTER_SOURCE_PATTERNS =
USE_MDFILE_AS_MAINPAGE =

#---------------------------------------------------------------------------
# Configuration options related to source browsing
#---------------------------------------------------------------------------

SOURCE_BROWSER         = NO
INLINE_SOURCES         = NO
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = NO
REFERENCES_RELATION    = NO
REFERENCES_LINK_SOURCE = NO
SOURCE_TOOLTIPS        = NO
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES

#---------------------------------------------------------------------------
# Configuration options related to the alphabetical class index
#---------------------------------------------------------------------------

ALPHABETICAL_INDEX     = YES
COLS_IN_ALPHA_INDEX    = 5
IGNORE_PREFIX          =

#---------------------------------------------------------------------------
# Configuration output options
#---------------------------------------------------------------------------

GENERATE_TODOLIST      = NO
GENERATE_TESTLIST      = NO
GENERATE_BUGLIST       = NO
GENERATE_DOCSET        = NO
GENERATE_HTMLHELP      = NO
GENERATE_CHI           = NO
GENERATE_LATEX         = NO
GENERATE_RTF           = NO
GENERATE_MAN           = NO
GENERATE_XML           = NO
GENERATE_AUTOGEN_DEF   = NO
GENERATE_PERLMOD       = NO
GENERATE_LEGEND        = NO

## NOTE: Required by QHP
GENERATE_HTML          = YES
HTML_OUTPUT            =
HTML_FILE_EXTENSION    = .html
HTML_COLORSTYLE_HUE    = 240
HTML_COLORSTYLE_SAT    = 0
HTML_COLORSTYLE_GAMMA  = 100

GENERATE_QHP           = YES
QCH_FILE               = @PROJECTNAME@.qch
QHP_NAMESPACE          = @ANTIQUACRM_DOMAIN_SCOPE@
QHG_LOCATION           = @ANTIQUA_QHG_LOCATION@

#---------------------------------------------------------------------------
# Configuration options related to the preprocessor
#---------------------------------------------------------------------------

# If the ENABLE_PREPROCESSING tag is set to YES, doxygen will evaluate all
# C-preprocessor directives found in the sources and include files.
# The default value is: YES.

ENABLE_PREPROCESSING   = YES

# If the MACRO_EXPANSION tag is set to YES, doxygen will expand all macro names
# in the source code. If set to NO, only conditional compilation will be
# performed. Macro expansion can be done in a controlled way by setting
# EXPAND_ONLY_PREDEF to YES.
# The default value is: NO.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

MACRO_EXPANSION        = NO

# If the EXPAND_ONLY_PREDEF and MACRO_EXPANSION tags are both set to YES then
# the macro expansion is limited to the macros specified with the PREDEFINED and
# EXPAND_AS_DEFINED tags.
# The default value is: NO.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

EXPAND_ONLY_PREDEF     = NO

# If the SEARCH_INCLUDES tag is set to YES, the include files in the
# INCLUDE_PATH will be searched if a #include is found.
# The default value is: YES.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

SEARCH_INCLUDES        = YES

# The INCLUDE_PATH tag can be used to specify one or more directories that
# contain include files that are not input files but should be processed by the
# preprocessor.
# This tag requires that the tag SEARCH_INCLUDES is set to YES.

INCLUDE_PATH           = .

# You can use the INCLUDE_FILE_PATTERNS tag to specify one or more wildcard
# patterns (like *.h and *.hpp) to filter out the header-files in the
# directories. If left blank, the patterns specified with FILE_PATTERNS will be
# used.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

INCLUDE_FILE_PATTERNS  =

# The PREDEFINED tag can be used to specify one or more macro names that are
# defined before the preprocessor is started (similar to the -D option of e.g.
# gcc). The argument of the tag is a list of macros of the form: name or
# name=definition (no spaces). If the definition and the "=" are omitted, "=1"
# is assumed. To prevent a macro definition from being undefined via #undef or
# recursively expanded use the := operator instead of the = operator.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

PREDEFINED             =

# If the MACRO_EXPANSION and EXPAND_ONLY_PREDEF tags are set to YES then this
# tag can be used to specify a list of macro names that should be expanded. The
# macro definition that is found in the sources will be used. Use the PREDEFINED
# tag if you want to use a different macro definition that overrules the
# definition found in the source code.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

EXPAND_AS_DEFINED      =

# If the SKIP_FUNCTION_MACROS tag is set to YES then doxygen's preprocessor will
# remove all references to function-like macros that are alone on a line, have
# an all uppercase name, and do not end with a semicolon. Such function macros
# are typically used for boiler-plate code, and will confuse the parser if not
# removed.
# The default value is: YES.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

SKIP_FUNCTION_MACROS   = YES

#---------------------------------------------------------------------------
# Configuration options related to external references
#---------------------------------------------------------------------------

# The TAGFILES tag can be used to specify one or more tag files. For each tag
# file the location of the external documentation should be added. The format of
# a tag file without this location is as follows:
# TAGFILES = file1 file2 ...
# Adding location for the tag files is done as follows:
# TAGFILES = file1=loc1 "file2 = loc2" ...
# where loc1 and loc2 can be relative or absolute paths or URLs. See the
# section "Linking to external documentation" for more information about the use
# of tag files.
# Note: Each tag file must have a unique name (where the name does NOT include
# the path). If a tag file is not located in the directory in which doxygen is
# run, you must also specify the path to the tagfile here.

TAGFILES               =

# When a file name is specified after GENERATE_TAGFILE, doxygen will create a
# tag file that is based on the input files it reads. See section "Linking to
# external documentation" for more information about the usage of tag files.

GENERATE_TAGFILE       = @CMAKE_CURRENT_BINARY_DIR@/doc/@PROJECTNAME@.tags

# If the ALLEXTERNALS tag is set to YES, all external class will be listed in
# the class index. If set to NO, only the inherited external classes will be
# listed.
# The default value is: NO.

ALLEXTERNALS           = NO

# If the EXTERNAL_GROUPS tag is set to YES, all external groups will be listed
# in the modules index. If set to NO, only the current project's groups will be
# listed.
# The default value is: YES.

EXTERNAL_GROUPS        = NO

# If the EXTERNAL_PAGES tag is set to YES, all external pages will be listed in
# the related pages index. If set to NO, only the current project's pages will
# be listed.
# The default value is: YES.

EXTERNAL_PAGES         = NO

# The PERL_PATH should be the absolute path and name of the perl script
# interpreter (i.e. the result of 'which perl').
# The default file (with absolute path) is: /usr/bin/perl.

PERL_PATH              = /usr/bin/perl

#---------------------------------------------------------------------------
# Configuration options related to the dot tool
#---------------------------------------------------------------------------

HAVE_DOT               = NO
CLASS_DIAGRAMS         = NO
HIDE_UNDOC_RELATIONS   = YES

##EOF
