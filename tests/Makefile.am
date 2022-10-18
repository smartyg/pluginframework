check_PROGRAMS = test-PluginManager
test_PluginManager_SOURCES = test-PluginManager.cpp
test_PluginManager_LDADD = @GTEST_LIBS@ @GMOCK_LIBS@ ../libcpluginmanager.la
test_PluginManager_LDFLAGS = -pthread
test_PluginManager_CPPFLAGS = @GTEST_CFLAGS@ @GMOCK_CFLAGS@ -pthread -I../include -I../src
#
# check_PROGRAMS += test-Iostream-Iobuf
# test_Iostream_Iobuf_SOURCES = test-Iostream-Iobuf
# test_Iostream_Iobuf_LDADD = @GTEST_LIBS@ @GMOCK_LIBS@ ../libgpsfile2.la
# test_Iostream_Iobuf_LDFLAGS = -pthread
# test_Iostream_Iobuf_CPPFLAGS = @GTEST_CFLAGS@ @GMOCK_CFLAGS@ -pthread -I../include -I../src
#
TESTS = test-PluginManager
