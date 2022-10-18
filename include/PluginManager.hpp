#ifndef _LIBCPLUGINMANAGER_MANAGER_
#define _LIBCPLUGINMANAGER_MANAGER_

#include <string>
#include <string_view>
#include <map>
#include <list>
#include <utility>
#include <functional>

#include <cpluginmanager/types/PluginDetails.h>
#include <cpluginmanager/traits/Manager.hpp>

namespace pluginmanager {
	typedef std::function<void(const char*, void*)> registar_function;
	typedef std::function<void(const char*)> deregistar_function;

	class Manager {
	private:
		std::map<const std::string, void*> _loaded_files;
		std::map<const std::string, std::pair<registar_function, deregistar_function>> _managers;
		std::list<std::pair<const std::string, PluginDetails>> _registered_plugins;

		friend void cpluginmanager_addManager (const char* tag, void (*register_function)(void*), void (*deregister_function)(const char* id));
		friend void cpluginmanager_register_plugin (const PluginDetails details);
		friend void cpluginmanager_remove_plugin (const char* id);

		Manager (void) {
			this->scanDirectory (PKGLIBDIR);
		}

		Manager (const Manager&)            = delete;
		Manager (Manager&&) noexcept        = delete;
		void operator= (const Manager&)     = delete;
		void operator= (Manager&&) noexcept = delete;

		~Manager (void);

	public:
		template <ManagerTrait M>
		void addManager (M& manager_instance) {
			registar_function f1 = std::bind (&M::registar, manager_instance, std::placeholders::_1, std::placeholders::_2);
			deregistar_function f2 = std::bind (&M::deregistar, manager_instance, std::placeholders::_1);
			this->addManager (M::TAG, f1, f2);
		}

		void registerPluginToManager (const std::string&, const PluginDetails& plugin) const;
		void scanDirectory (const std::string_view directory);

		const std::vector<std::string> getTags (const bool& registered = false) const noexcept;
		const std::vector<std::string> getIds (const std::string_view tag) const noexcept;
		bool hasManager (const std::string_view tag) const noexcept;
		const std::string_view getName (const std::string_view tag, const std::string_view id) const;
		const std::string_view getAuthor (const std::string_view tag, const std::string_view id) const;
		const std::string_view getLicense (const std::string_view tag, const std::string_view id) const;
		uint8_t getMajorVersion (const std::string_view tag, const std::string_view id) const;
		uint8_t getMinorVersion (const std::string_view tag, const std::string_view id) const;

		void addManager (const std::string_view tag, registar_function registar, deregistar_function deregistar);
		void registerPlugin (const std::string_view tag, const PluginDetails details);
		void removePlugin (const std::string_view tag, const std::string& id);

		[[nodiscard]] static Manager& getInstance (void);

	private:
		bool hasPluginRegistered (const std::string_view tag, const std::string_view id);
		const PluginDetails getPluginDetails (const std::string_view tag, const std::string_view id);
		static bool isNewerVersion (const PluginDetails& old_plugin, const PluginDetails& new_plugin);
	};
}

#endif /* _LIBCPLUGINMANAGER_MANAGER_ */