#include <gtest/gtest.h>

#include "config.h"

#define PKGLIBDIR ""

#include "PluginManager.hpp"

using pluginmanager::Manager;

void dummy_register (const char*, void*);
void dummy_remove (const char*);

void dummy_register (const char*, void*) { }
void dummy_remove (const char*) { }

class DummyManager {
public:
	void* _pointer;
	const char* _string;

	constexpr static const std::string_view TAG = "DUMMYMANAGER";
	void registar (const char* string, void* pointer) { this->_string = string; this->_pointer = pointer; }
	void deregistar (const char* string) { this->_string = string; }
};

TEST(PluginManagerTest, emptyTags)
{
	const auto ret = Manager::getInstance ().getTags ();
	EXPECT_EQ (ret.empty (), true);
}

TEST(PluginManagerTest, addManager1)
{
	Manager::getInstance ().addManager ("MANAGER1", &dummy_register, &dummy_remove);
	EXPECT_EQ (Manager::getInstance ().hasManager ("MANAGER1"), true);
	EXPECT_EQ (Manager::getInstance ().hasManager (DummyManager::TAG), false);
	EXPECT_EQ (Manager::getInstance ().hasManager ("MANAGER2"), false);
}

TEST(PluginManagerTest, addManager2)
{
	DummyManager dummy;
	Manager::getInstance ().addManager (dummy);
	EXPECT_EQ (Manager::getInstance ().hasManager ("MANAGER1"), true);
	EXPECT_EQ (Manager::getInstance ().hasManager (DummyManager::TAG), true);
	EXPECT_EQ (Manager::getInstance ().hasManager ("MANAGER2"), false);
}

TEST(PluginManagerTest, addPlugin1)
{
	PluginDetails plugin;
	plugin.id = "P2";
	plugin.name = "Plugin 2";
	plugin.author = "You";
	plugin.license = "Restricted";
	plugin.major_version = 3;
	plugin.minor_version = 2;
	Manager::getInstance ().registerPlugin ("MANAGER1", plugin);
	EXPECT_EQ (Manager::getInstance ().getIds ("MANAGER1").size (), 1);
	EXPECT_STREQ (Manager::getInstance ().getName ("MANAGER1", "P2").data (), "Plugin 2");
	EXPECT_STREQ (Manager::getInstance ().getAuthor ("MANAGER1", "P2").data (), "You");
	EXPECT_STREQ (Manager::getInstance ().getLicense ("MANAGER1", "P2").data (), "Restricted");
	EXPECT_EQ (Manager::getInstance ().getMajorVersion ("MANAGER1", "P2"), 3);
	EXPECT_EQ (Manager::getInstance ().getMinorVersion ("MANAGER1", "P2"), 2);
}

TEST(PluginManagerTest, addPlugin2)
{
	PluginDetails plugin;
	plugin.id = "P1";
	plugin.name = "Plugin 1";
	plugin.author = "Me";
	plugin.license = "Free";
	plugin.major_version = 1;
	plugin.minor_version = 1;
	Manager::getInstance ().registerPlugin (DummyManager::TAG, plugin);
	EXPECT_EQ (Manager::getInstance ().getIds (DummyManager::TAG).size (), 1);
	EXPECT_STREQ (Manager::getInstance ().getName (DummyManager::TAG, "P1").data (), "Plugin 1");
	EXPECT_STREQ (Manager::getInstance ().getAuthor (DummyManager::TAG, "P1").data (), "Me");
	EXPECT_STREQ (Manager::getInstance ().getLicense (DummyManager::TAG, "P1").data (), "Free");
	EXPECT_EQ (Manager::getInstance ().getMajorVersion (DummyManager::TAG, "P1"), 1);
	EXPECT_EQ (Manager::getInstance ().getMinorVersion (DummyManager::TAG, "P1"), 1);
}

TEST(PluginManagerTest, numOfTags)
{
	EXPECT_EQ (Manager::getInstance ().getTags (false).size (), 2);
}

TEST(PluginManagerTest, nonExistingPlugin)
{
	ASSERT_EQ (Manager::getInstance ().getIds (DummyManager::TAG).size (), 1);
	EXPECT_STREQ (Manager::getInstance ().getName (DummyManager::TAG, "P2").data (), nullptr);
	EXPECT_STREQ (Manager::getInstance ().getAuthor (DummyManager::TAG, "P2").data (), nullptr);
	EXPECT_STREQ (Manager::getInstance ().getLicense (DummyManager::TAG, "P2").data (), nullptr);
	EXPECT_EQ (Manager::getInstance ().getMajorVersion (DummyManager::TAG, "P2"), 0);
	EXPECT_EQ (Manager::getInstance ().getMinorVersion (DummyManager::TAG, "P2"), 0);
}


TEST(PluginManagerTest, addUpdatedPlugin)
{
	PluginDetails plugin;
	plugin.id = "P1";
	plugin.name = "Improved Plugin 1";
	plugin.author = "Me++";
	plugin.license = "Still Free";
	plugin.major_version = 1;
	plugin.minor_version = 2;
	Manager::getInstance ().registerPlugin (DummyManager::TAG, plugin);
	EXPECT_EQ (Manager::getInstance ().getIds (DummyManager::TAG).size (), 1);
	EXPECT_STREQ (Manager::getInstance ().getName (DummyManager::TAG, "P1").data (), "Improved Plugin 1");
	EXPECT_STREQ (Manager::getInstance ().getAuthor (DummyManager::TAG, "P1").data (), "Me++");
	EXPECT_STREQ (Manager::getInstance ().getLicense (DummyManager::TAG, "P1").data (), "Still Free");
	EXPECT_EQ (Manager::getInstance ().getMajorVersion (DummyManager::TAG, "P1"), 1);
	EXPECT_EQ (Manager::getInstance ().getMinorVersion (DummyManager::TAG, "P1"), 2);
}

TEST(PluginManagerTest, addDowgradePlugin)
{
	PluginDetails plugin;
	plugin.id = "P1";
	plugin.name = "Worse Plugin 1";
	plugin.author = "Me--";
	plugin.license = "Not Free";
	plugin.major_version = 0;
	plugin.minor_version = 2;
	Manager::getInstance ().registerPlugin (DummyManager::TAG, plugin);
	EXPECT_EQ (Manager::getInstance ().getIds (DummyManager::TAG).size (), 1);
	EXPECT_STREQ (Manager::getInstance ().getName (DummyManager::TAG, "P1").data (), "Improved Plugin 1");
	EXPECT_STREQ (Manager::getInstance ().getAuthor (DummyManager::TAG, "P1").data (), "Me++");
	EXPECT_STREQ (Manager::getInstance ().getLicense (DummyManager::TAG, "P1").data (), "Still Free");
	EXPECT_EQ (Manager::getInstance ().getMajorVersion (DummyManager::TAG, "P1"), 1);
	EXPECT_EQ (Manager::getInstance ().getMinorVersion (DummyManager::TAG, "P1"), 2);
}