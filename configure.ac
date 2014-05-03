dnl Process this file with autoconf to create configure.

dnl ###########################################################################
dnl Setup
dnl ###########################################################################
AC_PREREQ(2.59)

AC_INIT([sparkengine], [0.00], [rob@eatenbyagrue.org])
dnl AC_CONFIG_HEADERS([config.h])

AC_DEFUN([MXE_SETUP], [
    AC_ARG_WITH([mxe], AS_HELP_STRING([--with-mxe], [Path to MXE MinGW environment for cross-building]), [MXE_BASE=$withval], [MXE_BASE=])
	MXE_CROSS=
	MXE_CMAKE_TOOLCHAIN_FILE=""
	if test "x$MXE_BASE" != "x" ; then
		if test "x$host" != "x" ; then
			AC_MSG_ERROR([Cannot specify --host with --with-mxe])
		fi
		AC_MSG_CHECKING([for MXE])
		MXE_CMAKE_TOOLCHAIN_FILE="$MXE_BASE/usr/i686-pc-mingw32/share/cmake/mxe-conf.cmake"
		if ! test -f "$MXE_CMAKE_TOOLCHAIN_FILE" ; then
			AC_MSG_RESULT([no])
			AC_MSG_ERROR([MXE not found in $MXE_BASE])
		fi
		AC_MSG_RESULT([$MXE_BASE])
		MXE_CROSS=yes
		MXE_PATH="$MXE_BASE/usr/bin"
		PATH="$MXE_PATH:$PATH"
		host_alias=i686-pc-mingw32
		ac_tool_prefix=$host_alias-
		cross_compiling=yes
		HAVE_WINDRES=yes
		MXE_WINDRES="$MXE_BASE/usr/bin/${ac_tool_prefix}windres"
	fi
	AM_CONDITIONAL([HAVE_WINDRES], [test "$HAVE_WINDRES" = yes])
	AC_SUBST(MXE_BASE)
	AC_SUBST(MXE_CROSS)
	AC_SUBST(MXE_PATH)
	AC_SUBST(MXE_CMAKE_TOOLCHAIN_FILE)
	AC_SUBST(MXE_WINDRES)
])

MXE_SETUP

AM_INIT_AUTOMAKE([1.9 foreign dist-bzip2 dist-xz dist-zip subdir-objects])

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CXX
AC_C_CONST
AC_PROG_RANLIB

AC_DEFUN([AC_PROG_STRIP], [AC_CHECK_TOOL(STRIP, strip, :)])
AC_PROG_STRIP

#AC_PROG_LIBTOOL

dnl Silent compilation by default
AM_SILENT_RULES([yes])

dnl ###########################################################################
dnl Flags and options
dnl ###########################################################################

STD_CXXFLAGS=

EXTRA_CFLAGS=
EXTRA_CXXFLAGS=
EXTRA_CPPFLAGS=
EXTRA_LIBS=

WARN_CFLAGS=
WARN_CXXFLAGS=
WARN_CPPFLAGS=

dnl Check for ccache if they want it
AC_ARG_WITH([ccache], AS_HELP_STRING([--with-ccache], [Compile using ccache]), [AC_PATH_PROG([CCACHE], [ccache])])
if test "x$with_ccache" = xyes; then
	if test "x$CCACHE" = x; then
		AC_MSG_WARN([ccache was requested but was not found. Build will proceed as normal])
	fi
fi

dnl Add a flag, if supported by the compiler or preprocessor
dnl Assumes that -Werror works
AC_DEFUN([_SPARKENGINE_COMPILER_FLAG], dnl Language, prefix (EXTRA), flags variable 'foo' (EXTRA_fooFLAGS), flag, $CC/$CXX/$CPP/similar, if yes, if no
    [AC_LANG_PUSH([$1])
     saved_flags="$][$3][FLAGS"
     ][$3][FLAGS="$saved_flags -Werror $4"
     AC_MSG_CHECKING([to see if $5 supports $4])
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[return 0;]])], [][$2][_][$3][FLAGS="$][$2][_][$3][FLAGS $4"; AC_MSG_RESULT([yes])]m4_default([; $6], []), [AC_MSG_RESULT([no])]m4_default([; $7], []))
     ][$3][FLAGS="$saved_flags"
     AC_LANG_POP([$1])
    ])

AC_DEFUN([SPARKENGINE_CFLAG], [_SPARKENGINE_COMPILER_FLAG([C], [EXTRA], [C], [$1], [$CC], [$2], [$3])])
AC_DEFUN([SPARKENGINE_CXXFLAG], [_SPARKENGINE_COMPILER_FLAG([C++], [EXTRA], [CXX], [$1], [$CXX], [$2], [$3])])
AC_DEFUN([SPARKENGINE_CPPFLAG], [_SPARKENGINE_COMPILER_FLAG([CPP], [EXTRA], [CPP], [$1], [$CPP], [$2], [$3])])

AC_DEFUN([SPARKENGINE_C_CXX_FLAG],
	[_SPARKENGINE_COMPILER_FLAG([C], [EXTRA], [C], [$1], [$CC/$CXX], [EXTRA_CXXFLAGS="$EXTRA_CXXFLAGS $1"])])

AC_DEFUN([SPARKENGINE_WARN_CFLAG], [_SPARKENGINE_COMPILER_FLAG([C], [WARN], [C], [$1], [$CC], [$2], [$3])])
AC_DEFUN([SPARKENGINE_WARN_CXXFLAG], [_SPARKENGINE_COMPILER_FLAG([C++], [WARN], [CXX], [$1], [$CXX], [$2], [$3])])
AC_DEFUN([SPARKENGINE_WARN_CPPFLAG], [_SPARKENGINE_COMPILER_FLAG([CPP], [WARN], [CPP], [$1], [$CPP], [$2], [$3])])

AC_DEFUN([SPARKENGINE_WARN_C_CXX_FLAG],
	[_SPARKENGINE_COMPILER_FLAG([C], [WARN], [C], [$1], [$CC/$CXX], [WARN_CXXFLAGS="$WARN_CXXFLAGS $1"])])

dnl Early check for C++11
dnl     -std=c++11 defines __STRICT_ANSI__, which causes MinGW some problems so use gnu++11 on it instead
case "$host" in
  i686-pc-mingw32|x86_64-pc-mingw64)
	_SPARKENGINE_COMPILER_FLAG([C++], [STD], [CXX], [-std=gnu++11], [$CXX], [], [AC_MSG_ERROR([No compiler support for C++11. Unable to build])])
	;;
  *)
	_SPARKENGINE_COMPILER_FLAG([C++], [STD], [CXX], [-std=c++11], [$CXX], [], [AC_MSG_ERROR([No compiler support for C++11. Unable to build])])
esac


dnl Always add -Wall, where supported
SPARKENGINE_WARN_C_CXX_FLAG([-Wall])

dnl Add -O0 if requested
OPTIMISE=-O3
AC_ARG_WITH([no-optimise], AS_HELP_STRING([--with-no-optimise], [Compile without optimisation]), [test "$withval" != no && OPTIMISE=-O0], [])
AC_SUBST([OPTIMISE])

AC_ARG_WITH([fno-inline],  AS_HELP_STRING([--with-fno-inline], [Compile without inlining. Helps debugging segaults that occur in STL code]), [SPARKENGINE_C_CXX_FLAG([-fno-inline])])
AC_ARG_WITH([gprof],       AS_HELP_STRING([--with-gprof], [Compile with gprof profiler]), [SPARKENGINE_C_CXX_FLAG([-pg])])

dnl Strip binary after build
DO_STRIP=no
AC_ARG_WITH([strip], AS_HELP_STRING([--with-strip], [Strip binary after build]), [test "$withval" != no && DO_STRIP=yes], [])
AM_CONDITIONAL([DO_STRIP], [test "$DO_STRIP" = yes])


dnl Compiler warning options
AC_ARG_WITH([extra-warnings], AS_HELP_STRING([--without-extra-warnings], [Disable lots of extra compiler warnings]), [], [with_extra_warnings=yes])
if test "x$with_extra_warnings" != xno; then
  for warn_opt in \
-Wformat				\
-Wstrict-aliasing=2			\
-Wmissing-format-attribute		\
-Wmissing-noreturn			\
-Wdisabled-optimization			\
-Wfloat-equal				\
-Wshadow				\
-Wcast-qual				\
-Wcast-align				\
  ; do
    SPARKENGINE_WARN_C_CXX_FLAG([$warn_opt])
  done
  dnl -Wformat-security requires -Wformat first, so it needs a slightly customised test
  dnl we assume that if this test passes then the test (above) for -Wformat passed
  dnl and therefore -Wformat should already be in WARN_CXXFLAGS
  _SPARKENGINE_COMPILER_FLAG([C], [WARN], [C], [-Wformat -Wformat-security], [$CC/$CXX], [WARN_CXXFLAGS="$WARN_CXXFLAGS -Wformat-security"])
  for warn_opt in \
-Wno-format-zero-length			\
-Werror-implicit-function-declaration	\
  ; do
    SPARKENGINE_WARN_CFLAG([$warn_opt])
  done
  for warn_opt in \
-Wstrict-null-sentinel			\
-Wold-style-cast			\
-Wsign-promo				\
  ; do
    SPARKENGINE_WARN_CXXFLAG([$warn_opt])
  done
fi

dnl Add some useful args for clang under ccache
dnl http://petereisentraut.blogspot.com.au/2011/05/ccache-and-clang.html
if test "x$CCACHE" != x; then
    SPARKENGINE_C_CXX_FLAG([-Qunused-arguments])
    SPARKENGINE_C_CXX_FLAG([-fcolor-diagnostics])
fi

dnl ###########################################################################
dnl Library checks
dnl ###########################################################################

dnl FIXME: should really check for headers (if no pkgconfig)
AC_DEFUN([SPARKENGINE_CHECK_LIB],
	[PKG_CHECK_MODULES([$1], [$2], [],
		[AC_CHECK_LIB([$3], [$4], [], [AC_MSG_ERROR([$1 is required])])
		][$1][_CFLAGS=
		][$1][_LIBS="-l$3"
		AC_SUBST([$1][_CFLAGS])
		AC_SUBST([$1][_LIBS])
		])
	])

dnl Check for OpenGL libs
if test "x$MXE_CROSS" = "xyes" ; then
	AC_SUBST([GL_CFLAGS], [])
	AC_SUBST([GL_LIBS], [-lopengl32])
else
	SPARKENGINE_CHECK_LIB([GL], [gl], [GL], [glBegin])
fi

dnl ###########################################################################
dnl Platform selection
dnl ###########################################################################

AC_MSG_CHECKING([platform type])
case "$host" in
  i686-pc-mingw32|x86_64-pc-mingw64)
	PLATFORM=win32
	;;
  *)
	PLATFORM=posix
	;;
esac
AC_SUBST([PLATFORM])
AC_MSG_RESULT($PLATFORM)

AM_CONDITIONAL([BUILD_WIN32], [test "x-$PLATFORM" = "x-win32"])
AM_CONDITIONAL([BUILD_POSIX], [test "x-$PLATFORM" = "x-posix"])

dnl ###########################################################################
dnl Finished :-)
dnl ###########################################################################

dnl if ccache was wanted, update the compiler vars to match
dnl this is done late so that configure tests aren't run under ccache
if test "x$CCACHE" != x; then
    CC="$CCACHE $CC"
    CXX="$CCACHE $CXX"
fi

AC_SUBST([STD_CXXFLAGS])

AC_SUBST([EXTRA_CFLAGS])
AC_SUBST([EXTRA_CXXFLAGS])
AC_SUBST([EXTRA_CPPFLAGS])
AC_SUBST([EXTRA_LIBS])

AC_SUBST([WARN_CFLAGS])
AC_SUBST([WARN_CXXFLAGS])
AC_SUBST([WARN_CPPFLAGS])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/Core/Makefile
])
AC_OUTPUT
