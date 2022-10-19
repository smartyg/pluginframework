#ifndef _LIBCPLUGINMANAGER_TRAIT_PLUGINMANAGERTRAIT_
#define _LIBCPLUGINMANAGER_TRAIT_PLUGINMANAGERTRAIT_

#include <type_traits>

#include <cpluginmanager/traits/common.hpp>

namespace pluginmanager {
	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN ManagerTrait = requires {
			T::TAG;
			requires std::is_same_v<const std::string_view, decltype(T::TAG)>;
		} && requires(T t, const char* str, void* ptr) {
			{ t.registar (str, ptr) } -> SAME_TYPE(void);
			{ t.deregistar (str) } -> SAME_TYPE(void);
		};
	}
}

#endif /* _LIBCPLUGINMANAGER_TRAIT_PLUGINMANAGERTRAIT_ */
