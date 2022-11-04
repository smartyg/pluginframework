#include "config.h"

#include "pluginframework.h"

#include <string>
#include <string_view>
#include <functional>
#include <filesystem>
#include <utility>
#include <dlfcn.h>
#include <Logger.hpp>

#include "pluginframework/plugins.h"

using pluginframework::Controller;

bool is_shared_library (const std::filesystem::directory_entry& e);

bool is_shared_library (const std::filesystem::directory_entry& e) {
	DEBUG_2_MSG (1, "Check if file for extension: {:s}\n", e.path ().string ());
	const auto& path = e.path ();
	if (!path.has_extension ()) return false;
	return (path.extension ().string ().compare (".so") == 0);
}

Controller& Controller::getInstance (void) {
	static Controller instance; // Guaranteed to be destroyed.
	// Instantiated on first use.
	return instance;
}

Controller::~Controller (void) {
	DEBUG_MSG ("Controller::{:s} ()\n", __func__);

	this->_destructor_called = true;

	for (const auto& entry : this->_registered_plugins) {
		try {
			const auto& manager = this->_managers.at (std::string (entry.first));
			const auto& deregister_func = std::get<1>(manager);
			deregister_func (entry.second.id);
		} catch (std::out_of_range& e) { (void)e; }
	}
	this->_registered_plugins.clear ();

	this->_managers.clear ();

	int dl_ret;
	//for (const auto& [file, handle] : this->_loaded_files) {
	for (auto it = this->_loaded_files.begin (); it != this->_loaded_files.end (); ++it) {
		DEBUG_2_MSG (1, "Try to close module: {:s}\n", it->first);
		if ((dl_ret = dlclose (it->second)) != 0)
			WARNING_MSG ("Failed to close module {:s}: {:d}\n", it->first, dl_ret);
	}
}

void Controller::addManager (const std::string_view tag, std::function<void(const char*, void*)> registar, std::function<void(const char*)> deregistar) {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:p}, {:p})\n", __func__, tag.data (), fmt::ptr (&registar), fmt::ptr (&deregistar));

	bool already_present = false;
	try {
		this->_managers.at (std::string (tag));
		already_present = true;
	} catch (std::out_of_range& e) { (void)e; }

	if (already_present) return;

	this->_managers.insert (std::make_pair(tag, std::make_pair (registar, deregistar)));

	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first)) this->registerPluginToManager (entry.first, entry.second);
	}
}

void Controller::registerPluginToManager (const std::string& tag, const PluginDetails& plugin) const {
	DEBUG_MSG ("Controller::{:s} ({:s}, ...)\n", __func__, tag);

	try {
		const auto& manager = this->_managers.at (tag);
		const auto& register_func = std::get<0>(manager);
		register_func (plugin.id, plugin.data);
	} catch (std::out_of_range& e) { (void)e; }
}

void Controller::scanDirectory (std::string_view directory) {
	const std::filesystem::path directory_path (directory);
	if (!std::filesystem::is_directory (directory_path))
		return;

	DEBUG_MSG ("Checking files in directory: {:s}\n", directory_path.string ());
	for (const auto& dir_entry : std::filesystem::directory_iterator{directory_path}) {
		DEBUG_2_MSG (1, "Check file: {:s}\n", dir_entry.path ().string ());
		if (is_regular_file (dir_entry) && is_shared_library (dir_entry)) {
			const std::filesystem::path entry_abs = std::filesystem::canonical (dir_entry.path ());
			// Check if the file is already open
			bool already_loaded = false;
			try {
				this->_loaded_files.at (entry_abs.string ());
				already_loaded = true;
			} catch (std::out_of_range& e) { (void)e; }
			if (!already_loaded) {
				const std::string path_to_load = entry_abs.string ();
				void *plugin = dlopen (path_to_load.c_str (), RTLD_NOW);
				if (!plugin) {
					WARNING_MSG ("Cannot load module {:s}: {:s}\n", path_to_load, dlerror ());
				} else {
					DEBUG_2_MSG (1, "Loaded module: {:s}\n", path_to_load);
					this->_loaded_files.insert (std::make_pair (std::move (path_to_load), plugin));
				}
			}
		}
	}
}

bool Controller::hasPluginRegistered (const std::string_view tag, const std::string_view id) {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0) return true;
	}
	return false;
}

const PluginDetails Controller::getPluginDetails (const std::string_view tag, const std::string_view id) {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0) return entry.second;
	}
	return {};
}

void Controller::registerPlugin (const std::string_view tag, const PluginDetails details) {
	DEBUG_MSG ("Controller::{:s} ({:s}, ...)\n", __func__, tag.data ());
	if (this->hasPluginRegistered (tag, details.id)) {
		if (!Controller::isNewerVersion (this->getPluginDetails (tag, details.id), details)) return;
		this->removePlugin (tag, details.id);
	}
	const std::string tag_str (tag);
	this->_registered_plugins.push_back (std::make_pair (tag_str, details));
	this->registerPluginToManager (tag_str, details);
}
void Controller::removePlugin (const std::string_view tag, const std::string& id) {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), id);
	if (this->_destructor_called) return;

	for (auto it = this->_registered_plugins.cbegin (); it != this->_registered_plugins.cend (); ++it) {
		if (tag.compare (it->first) == 0 && id.compare (it->second.id) == 0) {
			try {
				const auto& manager = this->_managers.at (std::string (tag));
				const auto& deregister_func = std::get<1>(manager);
				deregister_func (id.c_str ());
			} catch (std::out_of_range& e) { (void)e; }
			it = this->_registered_plugins.erase (it);
		}
	}
}

bool Controller::isNewerVersion (const PluginDetails& old_plugin, const PluginDetails& new_plugin) {
	DEBUG_MSG ("Controller::{:s} (.., ...)\n", __func__);
	if (old_plugin.major_version < new_plugin.major_version) return true;
	else if (old_plugin.major_version == new_plugin.major_version &&
		old_plugin.minor_version < new_plugin.minor_version) return true;
	else return false;
}

const std::vector<std::string> Controller::getTags (const bool& registered) const noexcept {
	DEBUG_MSG ("Controller::{:s} ({:d})\n", __func__, registered);

	std::vector<std::string> ret;
	const auto contains = [] (const std::vector<std::string>& vector, const std::string& string) -> bool {
		const auto comparison = [&string] (const std::string& test) -> bool {
			if (string.compare (test) == 0) return true;
			else return false;
		};
		const auto res = std::find_if (vector.cbegin (), vector.cend (), comparison);
		if (res == vector.cend ()) return false;
		else return true;
	};
	for (const auto& entry : this->_registered_plugins) {
		if (!contains (ret, entry.first)) {
			if (!registered || (registered && this->_managers.contains (entry.first)))
				ret.push_back (entry.first);
		}
	}
	return ret;
}

const std::vector<std::string> Controller::getIds (const std::string_view tag) const noexcept {
	DEBUG_MSG ("Controller::{:s} ({:s})\n", __func__, tag.data ());
	std::vector<std::string> ret;
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0)
			ret.push_back (entry.second.id);
	}
	return ret;
}

bool Controller::hasManager (const std::string_view tag) const noexcept {
	DEBUG_MSG ("Controller::{:s} ({:s})\n", __func__, tag.data ());
	return this->_managers.contains (std::string (tag));
}

const std::string_view Controller::getName (const std::string_view tag, const std::string_view id) const {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0)
			return std::string_view (entry.second.name);
	}
	return {};
}

const std::string_view Controller::getAuthor (const std::string_view tag, const std::string_view id) const {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0)
			return std::string_view (entry.second.author);
	}
	return {};
}

const std::string_view Controller::getLicense (const std::string_view tag, const std::string_view id) const {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0)
			return std::string_view (entry.second.license);
	}
	return {};
}

uint8_t Controller::getMajorVersion (const std::string_view tag, const std::string_view id) const {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0)
			return entry.second.major_version;
	}
	return 0;
}

uint8_t Controller::getMinorVersion (const std::string_view tag, const std::string_view id) const {
	DEBUG_MSG ("Controller::{:s} ({:s}, {:s})\n", __func__, tag.data (), tag.data ());
	for (const auto& entry : this->_registered_plugins) {
		if (tag.compare (entry.first) == 0 && id.compare (entry.second.id) == 0)
			return entry.second.minor_version;
	}
	return 0;
}
