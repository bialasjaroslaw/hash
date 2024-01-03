#pragma once

#include <string>
#include <vector>
#include <bit>
#include <cstring>

#include <algorithm>

namespace Hash{
	inline std::string to_string(const uint8_t* data, uint64_t size){
		std::string str;
		str.reserve(size * 2);
		auto to_hex = [](uint8_t c) {
			auto v = c & 0x0F;
			return static_cast<char>(v < 10 ? '0' + v : 'a' + v - 10);
		};

		for(auto it = data ; it < data + size ; ++it)
		{
			const auto& v = *it;
			str.push_back(to_hex(v >> 4));
			str.push_back(to_hex(v & 0x0F));
		}
		return str;
	}
	inline std::vector<uint8_t> from_string(const std::string& str) {
		if(str.size() % 2 != 0)
			return {1};
		auto in_range = [](char value, char low, char high){
			return value >= low && value <= high;
		};
		auto is_hex = [&](char maybe_hex){
			return in_range(maybe_hex, '0', '9') || in_range(maybe_hex, 'a', 'f') || in_range(maybe_hex, 'A', 'F');
		};
		if(!std::all_of(str.cbegin(), str.cend(), is_hex))
			return {2};
		std::vector<uint8_t> hash;
		hash.reserve(str.size() / 2);
		auto hex_to_value = [&](char val){
			return static_cast<uint8_t>(in_range(val, '0', '9') ? val - '0' :
				in_range(val, 'a', 'f') ? val - 'a' + 10 : val - 'A' + 10);
		};
		uint64_t idx = 0;
		uint8_t val = 0;
		for(auto c : str){
			val = static_cast<uint8_t>(val << 4);
			val |= static_cast<uint8_t>(hex_to_value(c) & 0x0F);
			if (idx & 1)
				hash.push_back(val);
			++idx;
		}
		return hash;
	}

	constexpr uint32_t rotate_left(uint32_t x, uint32_t n) {
		return (x << n) | (x >> (32 - n));
	}

	bool constexpr is_positive_power_two(uint64_t value){
	if (value == 0)
		return false;
	return !(value & (value - 1));
	}

	template <typename D>
	void constexpr byteswap(D *dst) {
	constexpr auto size = sizeof(D);
	static_assert(is_positive_power_two(size));
	auto* ptr = reinterpret_cast<char*>(dst);
	auto* end = ptr + size - 1;
	while(ptr < end){
		std::swap(*ptr, *end);
		++ptr;
		--end;
	}
	}

	template <typename D, typename S>
	void constexpr byteswap_to_le(D *dst, const S *src) {
		memcpy(dst, src, 4);
		if constexpr (std::endian::native == std::endian::big) {
			byteswap(dst);
		}
	}
}