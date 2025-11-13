#include "file.hpp"

using namespace LINI;

#include <format>

std::string StringData::to_string(void)
{
    const auto size = data.size();
    if(data[0] == data[size - 1] && data[0] == '\"')
        data = data.substr(1, size - 2);
    return data;
}

void Section::dump(std::stringstream& out)
{
    for(auto entry : entries)
        out << entry.first << " = " << entry.second.data << "\n";
}

void File::dump(std::stringstream& out)
{
    for(auto section : sections)
    {
        out << std::format("[{}]", section.first) << "\n";
        section.second.dump(out);
        out << "\n";
    }
}

File& File::parse(std::string_view in)
{
    size_t start = 0, end = 0;
    std::string section_name;

    auto trim_view = [](std::string_view s) -> std::string_view {
        size_t first = s.find_first_not_of(" \t\r\n");
        if (first == std::string_view::npos) return {};
        size_t last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, last - first + 1);
    };

    while((end = in.find('\n', start)) != std::string_view::npos)
    {
        std::string_view line = trim_view(in.substr(start, end - start)); start = end + 1;

        // Skip comments and empty lines.
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        // New active section.
        if (line.front() == '[' && line.back() == ']')
        {
            section_name = line.substr(1, line.size() - 2);
            continue;
        }

        // Entry.
        auto pos = line.find('=');
        if (pos == std::string::npos) continue; // TODO: Has no '=', maybe malformed, ignore for now.

        // Save the <key, value> pair.
        std::string_view key = trim_view(line.substr(0, pos)), value = trim_view(line.substr(pos + 1));
        sections.try_emplace(section_name, Section{}).first->second
        .entries.try_emplace(std::string(key), StringData{std::string(value)});
    }
    return *this;
}