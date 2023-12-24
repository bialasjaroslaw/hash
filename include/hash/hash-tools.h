#pragma once

#include <span>

namespace Hash{
	std::string to_string(const uint8_t* data, uint64_t size) {
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

	std::vector<uint8_t> from_string(const std::string& str) {
		std::vector<uint8_t> hash;
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
			if (idx++ & 1)
			{
				hash.push_back(val);
				val = 0;
			}
		}
		return hash;
	}
}