#pragma once
#include <string>

using fnv1a_t = std::uint32_t;

namespace fnv1a
{
	inline constexpr std::uint32_t basis = 0x811C9DC5;
	inline constexpr std::uint32_t prime = 0x1000193;

	// compile-time hashes
	constexpr fnv1a_t hash_const(const char* str, const fnv1a_t value = basis) noexcept
	{
		return (str[0] == '\0')
			? value
			: hash_const(&str[1], (value ^ fnv1a_t(str[0])) * prime);
	}

	// runtime hashes
	inline fnv1a_t hash(const char* str)
	{
		fnv1a_t hashed = basis;

		for (std::size_t i = 0U; i < strlen(str); ++i)
		{
			hashed ^= str[i];
			hashed *= prime;
		}

		return hashed;
	}
}
