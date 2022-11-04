#ifndef _PLUGINFRAMEWORK_TRAIT_PLUGINMANAGERTRAIT_
#define _PLUGINFRAMEWORK_TRAIT_PLUGINMANAGERTRAIT_

#include <type_traits>

#include <pluginframework/traits/common.hpp>

namespace pluginframework {
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

#endif /* _PLUGINFRAMEWORK_TRAIT_PLUGINMANAGERTRAIT_ */
