#ifndef _LIBCPLUGINMANAGER_TRAIT_PLUGINMANAGERTRAIT_
#define _LIBCPLUGINMANAGER_TRAIT_PLUGINMANAGERTRAIT_

#include <type_traits>

#include <cpluginmanager/traits/common.hpp>

namespace pluginmanager {
	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN ManagerTrait = requires(const T t) {
			typename T::TAG;
		};
	}
}

#endif /* _LIBCPLUGINMANAGER_TRAIT_PLUGINMANAGERTRAIT_ */
