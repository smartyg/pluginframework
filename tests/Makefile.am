check_PROGRAMS = test-Controller
test_Controller_SOURCES = test-Controller.cpp
test_Controller_LDADD = @GTEST_LIBS@ @GMOCK_LIBS@ ../libpluginframework.la
test_Controller_LDFLAGS = -pthread
test_Controller_CPPFLAGS = @GTEST_CFLAGS@ @GMOCK_CFLAGS@ -pthread -I../include -I../src

TESTS = test-Controller
