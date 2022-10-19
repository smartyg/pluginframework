## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

AM_CFLAGS =
AM_CXXFLAGS =
AM_LDFLAGS =
AM_LIBS =

## Only use the compiler flags when we build a debug version.
if DEBUG
COMMON_DEBUG_OPTIONS = -Wall -Wextra -W -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
	-Wmissing-declarations -Wno-missing-braces -Wno-missing-field-initializers \
	-Wformat=1 -Wswitch-default -Wswitch-enum -Wstrict-overflow=2 \
	-Wundef -Wunreachable-code -Wlogical-op -Wfloat-equal -Wno-float-equal \
	-Wstrict-aliasing=2 -Wredundant-decls -Werror \
	-fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing \
	-fvar-tracking -fvar-tracking-assignments -ggdb3 -O0
AM_CFLAGS += $(COMMON_DEBUG_OPTIONS) \
	-Wbad-function-cast -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs \
	-Wold-style-definition
AM_CXXFLAGS += $(COMMON_DEBUG_OPTIONS) \
	-Wconversion-null -Wctor-dtor-privacy -Wdelete-non-virtual-dtor \
	-Wnarrowing -Wnoexcept -Wnon-virtual-dtor -Wreorder -Wstrict-null-sentinel \
	-Wno-non-template-friend -Wold-style-cast -Woverloaded-virtual -Wno-pmf-conversions \
	-Wsign-promo
else
AM_CFLAGS += -flto
AM_CXXFLAGS += -flto
endif

if RELEASE
AM_CFLAGS += -O3
AM_CXXFLAGS += -O3
endif

if COVERAGE
AM_CFLAGS += -fprofile-arcs -ftest-coverage
AM_CXXFLAGS += -fprofile-arcs -ftest-coverage
AM_LDFLAGS += -fprofile-arcs -ftest-coverage
AM_LIBS += -lgcov
endif

export AM_CFLAGS
export AM_CXXFLAGS
export AM_LDFLAGS
export AM_LIBS

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libcpluginmanager.pc

lib_LTLIBRARIES = libcpluginmanager.la
libcpluginmanager_la_CPPFLAGS = -Iinclude -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\" @GPSDATA_UTILS_CFLAGS@

## Define the source file list for the "libexample-@EXAMPLE_API_VERSION@.la"
## target.  Note that @EXAMPLE_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.
libcpluginmanager_la_SOURCES = src/PluginManager.cpp \
	src/cpluginmanager.cpp \
	src/plugins.cpp

libcpluginmanager_la_LIBADD = @DL_LIBS@ @GPSDATA_UTILS_LIBS@

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libcpluginmanager_la_LDFLAGS = -version-info @LIBRARY_CURRENT_VERSION@:@LIBRARY_REVISION_VERSION@:@LIBRARY_AGE_VERSION@

noinst_HEADERS =

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
libcpluginmanager_includedir = $(includedir)
libcpluginmanager_include_HEADERS = include/cpluginmanager.h \
	include/PluginManager.hpp

libcpluginmanager_sub_includedir = $(includedir)/cpluginmanager
libcpluginmanager_sub_include_HEADERS = include/cpluginmanager/plugins.h

libcpluginmanager_sub_traits_includedir = $(includedir)/cpluginmanager/traits
libcpluginmanager_sub_traits_include_HEADERS = include/cpluginmanager/traits/common.hpp \
	include/cpluginmanager/traits/Manager.hpp

libcpluginmanager_sub_types_includedir = $(includedir)/cpluginmanager/types
libcpluginmanager_sub_types_include_HEADERS = include/cpluginmanager/types/PluginDetails.h

if EXAMPLE

#compile the calc plugin set
pkglib_LTLIBRARIES = calc_add.la
calc_add_la_CPPFLAGS = -Iinclude -DLOCALEDIR=\"$(localedir)\"
calc_add_la_LDFLAGS = -module -avoid-version -shared
calc_add_la_LIBADD = libcpluginmanager.la
calc_add_la_SOURCES = examples/calc_plugins/plugin-calc-add.c

pkglib_LTLIBRARIES += calc_multiply.la
calc_multiply_la_CPPFLAGS = -Iinclude -DLOCALEDIR=\"$(localedir)\"
calc_multiply_la_LDFLAGS = -module -avoid-version -shared
calc_multiply_la_LIBADD = libcpluginmanager.la
calc_multiply_la_SOURCES = examples/calc_plugins/plugin-calc-multiply.c

#compile the print plugin set

#example 1
noinst_PROGRAMS = c_calc_example
c_calc_example_LDADD = libcpluginmanager.la
c_calc_example_LDFLAGS = -Wl,-rpath=$(pkglibdir),--enable-new-dtags
c_calc_example_CPPFLAGS = -Iinclude -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"./.libs/\"
c_calc_example_SOURCES = examples/example_calc.c \
	examples/calc_plugins/calc_manager.c

noinst_HEADERS += examples/calc_plugins/calc_manager.h
endif

EXTRA_DIST = VERSION ChangeLog README AUTHORS LICENSE @PROGRAM_NAME@.spec

## Only try to update the ChangeLog file when git is availible to report all the changes.
if HAVE_GIT
## Force to recreate the ChangeLog file every time
.PHONY: ChangeLog VERSION

## Dynamicly create the changelog file when a distribution tarball is created.
ChangeLog:
	-$(GIT) log --decorate > ChangeLog

VERSION:
	-$(GIT) describe --tags --match=[0-9]* --long --dirty | $(SED) 's/^v//' > VERSION

distclean-local:
	@rm -f ChangeLog VERSION
endif

if COVERAGE
COVERAGE_INFO_FILE = $(top_builddir)/coverage.info
COVERAGE_REPORT_DIR = $(top_builddir)/coverage

.PHONY: coverage coverage-report clean-coverage-report clean-coverage

coverage: clean-coverage all-am check-am check coverage-report

coverage-report: check
	@echo "Start to create coverage reports..."
	$(LCOV) --capture \
		--directory "$(top_builddir)/include" \
		--directory "$(top_builddir)/src" \
		--directory "$(top_builddir)/tests" \
		--base-directory "$(abspath $(top_builddir))" \
		--output-file $(COVERAGE_INFO_FILE) \
		--gcov-tool $(GCOV) \
		--compat-libtool --checksum
	$(LCOV) --extract $(COVERAGE_INFO_FILE) "$(abspath $(top_srcdir))/include/*" \
		--extract $(COVERAGE_INFO_FILE) "$(abspath $(top_srcdir))/src/*" \
		--extract $(COVERAGE_INFO_FILE) "$(abspath $(top_srcdir))/test/*" \
		--gcov-tool $(GCOV) \
		--output-file $(COVERAGE_INFO_FILE)
	$(GENHTML) --prefix "$(abspath $(top_srcdir))" \
		--output-directory $(COVERAGE_REPORT_DIR) \
                --title $(PACKAGE_NAME) \
		--legend --show-details \
		$(GENHTML_OPTIONS) \
		$(COVERAGE_INFO_FILE)
	@echo "Successfully created coverage reports into $(COVERAGE_REPORT_DIR) directory."

clean-coverage-report:
	-rm -rf $(COVERAGE_INFO_FILE)
	-rm -rf $(COVERAGE_REPORT_DIR)

clean-coverage: clean-coverage-report
	-$(LCOV) --gcov-tool $(GCOV) --zerocounters --directory $(top_builddir)

clean-gcov-rundata: clean-coverage-report
	@if xargs --version 2>/dev/null; then \
		find $(top_builddir) -name "*.gcda" | xargs --no-run-if-empty rm; \
	else \
		find $(top_builddir) -name "*.gcda" | xargs rm; \
	fi

clean-gcov-data: clean-gcov-rundata clean-coverage-report
	@if xargs --version 2>/dev/null; then \
		find $(top_builddir) -name "*.gcno" | xargs --no-run-if-empty rm; \
	else \
		find $(top_builddir) -name "*.gcno" | xargs rm; \
	fi

clean-local: clean-gcov-data
endif

if GTEST
SUBDIRS = tests
endif
