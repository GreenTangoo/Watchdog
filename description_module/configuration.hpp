#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <memory>
#include <mutex>
#include <regex>
#include <cstdlib>

#include "types_resolver.hpp"
#include "../utility_module/json.hpp"
#include "../utility_module/regex_siem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define EMPTY_REGEX_GROUP -1

namespace description_space
{
	class Configuration
	{
	private:
		JsonObject _configurationFileParser;
	public:
		Configuration(JsonObject configJson);
		Configuration(Configuration const &other);
		Configuration(Configuration &&other);
		Configuration const& operator=(Configuration const &other);
		Configuration const& operator=(Configuration &&other);
		JsonObject getConfiguration(std::string const &nameNode) const;
	};
}

#endif // CONFIGURATION_HPP
