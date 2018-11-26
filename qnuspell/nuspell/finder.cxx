/* Copyright 2016-2018 Dimitrij Mijoski
 *
 * This file is part of Nuspell.
 *
 * Nuspell is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nuspell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Nuspell.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file finder.cxx
 * Finding dictionaries.
 */

#include "finder.hxx"
#include "string_utils.hxx"

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_set>
#include <utility>

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) ||               \
                         (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#ifdef _POSIX_VERSION
#include <dirent.h>
#include <glob.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#elif defined(_WIN32)

#include <io.h>
#include <windows.h>

#ifdef __MINGW32__
#include <dirent.h>
//#include <glob.h> //not present in mingw-w64. present in vanilla mingw
#include <sys/stat.h>
#include <sys/types.h>
#endif //__MINGW32__

#endif

using namespace std;

namespace nuspell {

#ifdef _WIN32
const auto PATHSEP = ';';
const auto DIRSEP = '\\';
const auto SEPARATORS = "\\/";
#else
const auto PATHSEP = ':';
const auto DIRSEP = '/';
const auto SEPARATORS = '/';
#endif

/**
 * Gets the default search paths.
 *
 * @param out Output iterator, begin of the output range.
 * @return End of the output range.
 */
template <class OutIt>
auto get_default_search_paths(OutIt out) -> OutIt
{
	*out++ = ".";
	char* dicpath = getenv("DICPATH");
	if (dicpath) {
		out = split(string(dicpath), PATHSEP, out);
	}
#ifdef _POSIX_VERSION
	char* home = getenv("HOME");
	array<string, 3> prefixes = {home ? string(home) + "/.local" : "",
	                             "/usr/local", "/usr"};
	array<const char*, 3> dirs = {"/share/hunspell", "/share/myspell",
	                              "/share/myspell/dicts"};
	for (auto& dir : dirs) {
		for (auto& prefix : prefixes) {
			*out = prefix + dir;
			++out;
		}
	}
#if defined(__APPLE__) && defined(__MACH__)
	string osx = "/Library/Spelling";
	if (home) {
		*out++ = home + osx;
	}
	*out++ = osx;
#endif
#endif
#ifdef _WIN32
	array<char*, 2> winpaths = {getenv("LOCALAPPDATA"),
	                            getenv("PROGRAMDATA")};
	for (auto& p : winpaths) {
		if (p) {
			*out++ = string(p) + "\\hunspell";
		}
	}
#endif
	return out;
}

/**
 * Adds the default directory paths which may containt dictionaries.
 */
auto Finder::add_default_dir_paths() -> void
{
	get_default_search_paths(back_inserter(paths));
}

#ifdef _WIN32
class FileListerWindows {
	struct _finddata_t data = {};
	intptr_t handle = -1;
	bool goodbit = false;

      public:
	FileListerWindows() {}
	FileListerWindows(const char* pattern) { first(pattern); }
	FileListerWindows(const string& pattern) { first(pattern); }

	FileListerWindows(const FileListerWindows& d) = delete;
	void operator=(const FileListerWindows& d) = delete;

	auto first(const char* pattern) -> bool
	{
		close();
		handle = _findfirst(pattern, &data);
		goodbit = handle != -1;
		return goodbit;
	}
	auto first(const string& pattern) -> bool
	{
		return first(pattern.c_str());
	}

	auto name() const -> const char* { return data.name; }
	auto good() const -> bool { return goodbit; }
	auto next() -> bool
	{
		goodbit = _findnext(handle, &data) == 0;
		return goodbit;
	}
	auto close() -> void
	{
		if (handle == -1)
			return;
		_findclose(handle);
		handle = -1;
		goodbit = false;
	}
	auto list_all() -> vector<string>
	{
		vector<string> ret;
		for (; good(); next()) {
			ret.emplace_back(name());
		}
		return ret;
	}
};
#endif

#if defined(_POSIX_VERSION) || defined(__MINGW32__)
class Directory {
	DIR* dp = nullptr;
	struct dirent* ent_p = nullptr;

      public:
	Directory() = default;
	Directory(const Directory& d) = delete;
	void operator=(const Directory& d) = delete;
	auto open(const string& dirname) -> bool
	{
		close();
		dp = opendir(dirname.c_str());
		return dp;
	}
	auto next() -> bool { return (ent_p = readdir(dp)); }
	auto entry_name() const -> const char* { return ent_p->d_name; }
	auto close() -> void
	{
		if (dp) {
			(void)closedir(dp);
			dp = nullptr;
		}
	}
	~Directory() { close(); }
};
#elif defined(_WIN32)
class Directory {
	FileListerWindows fl;
	bool first = true;

      public:
	Directory() {}
	Directory(const Directory& d) = delete;
	void operator=(const Directory& d) = delete;
	auto open(const string& dirname) -> bool
	{
		fl.first(dirname + "\\*");
		first = true;
		return fl.good();
	}
	auto next() -> bool
	{
		if (first)
			first = false;
		else
			fl.next();
		return fl.good();
	}
	auto entry_name() const -> const char* { return fl.name(); }
	auto close() -> void { fl.close(); }
};
#else
struct Directory {
	Directory() {}
	Directory(const Directory& d) = delete;
	void operator=(const Directory& d) = delete;
	auto open(const string& dirname) -> bool { return false; }
	auto next() -> bool { return false; }
	auto entry_name() const -> const char* { return nullptr; }
	auto close() -> void {}
};
#endif

#ifdef _POSIX_VERSION
class Globber {
      private:
	glob_t g = {};
	int ret = 1;

      public:
	Globber(const char* pattern) { ret = ::glob(pattern, 0, nullptr, &g); }
	Globber(const string& pattern) : Globber(pattern.c_str()) {}
	auto glob(const char* pattern) -> bool
	{
		globfree(&g);
		ret = ::glob(pattern, 0, nullptr, &g);
		return ret == 0;
	}
	auto glob(const string& pattern) -> bool
	{
		return glob(pattern.c_str());
	}
	auto begin() -> const char* const* { return g.gl_pathv; }
	auto end() -> const char* const* { return begin() + g.gl_pathc; }
	template <class OutIt>
	auto copy_glob_paths(OutIt out) -> OutIt
	{
		if (ret == 0) {
			out = copy(begin(), end(), out);
		}
		return out;
	}
	~Globber() { globfree(&g); }
};
#elif defined(_WIN32)
class Globber {
	vector<string> data;

      public:
	Globber(const char* pattern) { glob(pattern); }
	Globber(const string& pattern) { glob(pattern); }
	auto glob(const char* pattern) -> bool { return glob(string(pattern)); }
	auto glob(const string& pattern) -> bool
	{
		data.clear();

		if (pattern.empty())
			return false;
		auto first_two = pattern.substr(0, 2);
		if (first_two == "\\\\" || first_two == "//" ||
		    first_two == "\\/" || first_two == "//")
			return false;

		auto q1 = vector<string>();
		auto q2 = q1;
		auto v = q1;

		split_on_any_of(pattern, "\\/", back_inserter(v));
		auto i = v.begin();
		if (i == v.end())
			return false;

		FileListerWindows fl;

		if (i->find(':') != i->npos) {
			// absolute path
			q1.push_back(*i++);
		}
		else if (pattern[0] == '\\' || pattern[0] == '/') {
			// relative to drive
			q1.push_back("");
		}
		else {
			// relative
			q1.push_back(".");
		}
		for (; i != v.end(); ++i) {
			if (i->empty())
				continue;
			for (auto& q1e : q1) {
				auto p = q1e + DIRSEP + *i;
				// cout << "P " << p << endl;
				fl.first(p.c_str());
				for (; fl.good(); fl.next()) {

					if (fl.name() == string(".") ||
					    fl.name() == string(".."))
						continue;
					auto n = q1e + DIRSEP + fl.name();
					q2.push_back(n);
					// cout << "Q2 " << n << endl;
				}
			}
			q1.clear();
			q1.swap(q2);
		}
		data.insert(data.end(), q1.begin(), q1.end());
		return true;
	}
	auto begin() -> vector<string>::iterator { return data.begin(); }
	auto end() -> vector<string>::iterator { return data.end(); }
	template <class OutIt>
	auto copy_glob_paths(OutIt out) -> OutIt
	{
		out = copy(begin(), end(), out);
		return out;
	}
};
#else
// unimplemented
struct Globber {
	Globber(const char* pattern) {}
	Globber(const string& pattern) {}
	auto glob(const char* pattern) -> bool { return false; }
	auto glob(const string& pattern) -> bool { return false; }
	auto begin() -> char** { return nullptr; }
	auto end() -> char** { return nullptr; }
	template <class OutIt>
	auto copy_glob_paths(OutIt out) -> OutIt
	{
		return out;
	}
};
#endif

/**
 * Gets the Mozilla search paths.
 *
 * @param out Output iterator, begin of the output range.
 * @return End of the output range.
 */
template <class OutIt>
auto get_mozilla_paths(OutIt out) -> OutIt
{
#ifdef _POSIX_VERSION
	// add Mozilla Linux global path
	array<const char*, 4> dirs = {"/usr/local/lib/firefox/dictionaries",
	                              "/usr/lib/firefox/dictionaries",
	                              "/usr/local/lib/thunderbird/dictionaries",
	                              "/usr/lib/thunderbird/dictionaries"};
	struct stat dir_stat;
	for (auto& dir : dirs) {
		if (lstat(dir, &dir_stat) == 0) {
			if (S_ISDIR(dir_stat.st_mode)) {
				*out++ = dir;
			}
			// if SYMLINK do not add
		}
	}

	// add Mozilla Linux user path
	char* home = getenv("HOME");
	if (home == nullptr) {
		return out;
	}
	string moz = home;
	moz += "/.mozilla/firefox/*/extensions/*/dictionaries";
	Globber gm(moz);
	out = gm.copy_glob_paths(out);

	moz = home;
	moz += "/.thunderbird/*/extensions/*/dictionaries";
	Globber gt(moz);
	out = gt.copy_glob_paths(out);

#elif defined(_WIN32)
	// add Mozilla Windows global path
	array<char*, 2> winpaths = {getenv("PROGRAMFILES"),
	                            getenv("PROGRAMFILES(x86)")};
	for (auto& p : winpaths) {
		if (p) {
			*out++ = string(p) + "\\Mozilla Firefox\\dictionaries";
			*out++ =
			    string(p) + "\\Mozilla Thunderbird\\dictionaries";
		}
	}
	// add Mozilla Windows local path
	char* home = getenv("APPDATA");
	if (home == nullptr) {
		return out;
	}
	string moz = home;
	moz += "\\Mozilla\\Firefox\\Profiles\\*\\extensions\\*\\dictionaries";
	Globber gm(moz);
	out = gm.copy_glob_paths(out);

	moz = home;
	moz +=
	    "\\Mozilla\\Thunderbird\\Profiles\\*\\extensions\\*\\dictionaries";
	Globber gt(moz);
	out = gt.copy_glob_paths(out);
#endif
	return out;
}

/**
 * Adds the Mozilla directory paths which may containt dictionaries.
 */
auto Finder::add_mozilla_dir_paths() -> void
{
	get_mozilla_paths(back_inserter(paths));
}

/**
 * Gets the LibreOffice search paths.
 *
 * @param out Output iterator, begin of the output range.
 * @return End of the output range.
 */
template <class OutIt>
auto get_libreoffice_paths(OutIt out) -> OutIt
{
	string lo_user_glob;
#ifdef _POSIX_VERSION
	// add LibreOffice Linux global paths
	array<const char*, 3> prefixes = {"/usr/local/lib/libreoffice",
	                                  "/usr/lib/libreoffice",
	                                  "/opt/libreoffice*"};
	for (auto& prefix : prefixes) {
		Globber g(string(prefix) + "/share/extensions/dict-*");
		out = g.copy_glob_paths(out);
	}

	// add LibreOffice Linux local

	char* home = getenv("HOME");
	if (home == nullptr) {
		return out;
	}
	lo_user_glob = home;
	lo_user_glob += "/.config/libreoffice/?/user/uno_packages/cache"
	                "/uno_packages/*/*.oxt/";
#elif defined(_WIN32)
	// add Libreoffice Windows global paths
	array<char*, 2> prefixes = {getenv("PROGRAMFILES"),
	                            getenv("PROGRAMFILES(x86)")};
	for (auto& prefix : prefixes) {
		if (prefix == nullptr) {
			continue;
		}
		Globber g(string(prefix) +
		          "\\LibreOffice ?\\share\\extensions\\dict-*");
		out = g.copy_glob_paths(out);
	}

	char* home = getenv("APPDATA");
	if (home == nullptr) {
		return out;
	}
	lo_user_glob = home;
	lo_user_glob += "\\libreoffice\\?\\user\\uno_packages\\cache"
	                "\\uno_packages\\*\\*.oxt\\";
#else
	return out;
#endif
	// finish adding LibreOffice user path dicts (Linux and Windows)
	Globber g(lo_user_glob + "dict*");
	out = g.copy_glob_paths(out);

	g.glob(lo_user_glob + "*.aff");
	string path_str;
	for (auto& path : g) {
		path_str = path;
		path_str.erase(path_str.rfind(DIRSEP));
		*out = path_str;
		++out;
	}
	return out;
}

/**
 * Adds the LibreOffice directory paths which may containt dictionaries.
 */
auto Finder::add_libreoffice_dir_paths() -> void
{
	get_libreoffice_paths(back_inserter(paths));
}

/**
 * Gets the Apache OpenOffice search paths.
 *
 * @param out Output iterator, begin of the output range.
 * @return End of the output range.
 */
template <class OutIt>
auto get_openoffice_paths(OutIt out) -> OutIt
{
	// Note that Apache OpenOffice is no longer available on Debian and
	// Ubuntu. For legacy reasons, all paths are still supported.
	string aoo_user_glob;
#ifdef _POSIX_VERSION
	// add Apache OpenOffice Linux global paths
	array<const char*, 3> prefixes = {"/usr/local/lib/openoffice",
	                                  "/usr/lib/openoffice",
	                                  "/opt/openoffice*"};
	for (auto& prefix : prefixes) {
		Globber g(string(prefix) + "/share/extensions/dict-*");
		out = g.copy_glob_paths(out);
	}

	// add Apache OpenOffice Linux local

	char* home = getenv("HOME");
	if (home == nullptr) {
		return out;
	}
	aoo_user_glob = home;
	aoo_user_glob += "/.config/openoffice/?/user/uno_packages/cache"
	                 "/uno_packages/*/*.oxt/";
#elif defined(_WIN32)
	// add Apache OpenOffice Windows global paths
	array<char*, 2> prefixes = {getenv("PROGRAMFILES"),
	                            getenv("PROGRAMFILES(x86)")};
	for (auto& prefix : prefixes) {
		if (prefix == nullptr) {
			continue;
		}
		Globber g(string(prefix) +
		          "\\OpenOffice ?\\share\\extensions\\dict-*");
		out = g.copy_glob_paths(out);
	}

	char* home = getenv("APPDATA");
	if (home == nullptr) {
		return out;
	}
	aoo_user_glob = home;
	aoo_user_glob += "\\openoffice\\?\\user\\uno_packages\\cache"
	                 "\\uno_packages\\*\\*.oxt\\";
#else
	return out;
#endif
	// finish adding Apache OpenOffice user path dicts (Linux and Windows)
	Globber g(aoo_user_glob + "dict*");
	out = g.copy_glob_paths(out);

	g.glob(aoo_user_glob + "*.aff");
	string path_str;
	for (auto& path : g) {
		path_str = path;
		path_str.erase(path_str.rfind(DIRSEP));
		*out = path_str;
		++out;
	}
	return out;
}

/**
 * Adds the OpenOffice directory paths which may containt dictionaries.
 */
auto Finder::add_openoffice_dir_paths() -> void
{
	get_openoffice_paths(back_inserter(paths));
}

/**
 * Searches directory for dictionaries.
 *
 * @param dir directory path.
 * @param out output iter where to append the found dictionary names.
 * @return end of the output range
 */
template <class OutIt>
auto search_path_for_dicts(const string& dir, OutIt out) -> OutIt
{
	Directory d;
	if (d.open(dir) == false) {
		return out;
	}
	unordered_set<string> dics;
	string file_name;
	while (d.next()) { // Ideally process file names in alphabetical
		           // order, output is now:
		// nl_BE	/usr/share/hunspell/nl_BE
		// en_US	/usr/share/hunspell/en_US
		// nl_NL	/usr/share/hunspell/nl_NL
		// en_GB	/usr/share/hunspell/en_GB
		file_name = d.entry_name();
		auto sz = file_name.size();
		if (sz < 4) {
			continue;
		}
		if (file_name.compare(sz - 4, 4, ".dic") == 0) {
			dics.insert(file_name);
			file_name.replace(sz - 4, 4, ".aff");
			if (dics.count(file_name)) {
				file_name.erase(sz - 4);
				auto full_path = dir + DIRSEP + file_name;
				*out = make_pair(file_name, full_path);
				out++;
			}
		}
		else if (file_name.compare(sz - 4, 4, ".aff") == 0) {
			dics.insert(file_name);
			file_name.replace(sz - 4, 4, ".dic");
			if (dics.count(file_name)) {
				file_name.erase(sz - 4);
				auto full_path = dir + DIRSEP + file_name;
				*out = make_pair(file_name, full_path);
				out++;
			}
		}
	}
	return out;
}

/**
 * Searches the added directories for dictionaries.
 */
auto Finder::search_for_dictionaries() -> void
{
	dictionaries.clear();
	for (auto& path : paths) {
		search_path_for_dicts(path, back_inserter(dictionaries));
	}
	stable_sort(dictionaries.begin(), dictionaries.end(),
	            [](auto& a, auto& b) { return a.first < b.first; });
}

/**
 * Creates Finder object with all possible dictionaries found.
 * @return Finder object
 */
auto Finder::search_all_dirs_for_dicts() -> Finder
{
	auto ret = Finder();
	ret.add_default_dir_paths();
	ret.add_mozilla_dir_paths();
	ret.add_libreoffice_dir_paths();
	ret.add_openoffice_dir_paths();
	ret.search_for_dictionaries();
	return ret;
}

auto Finder::find(const std::string& dict) const -> const_iterator
{
	return find_if(begin(), end(),
	               [&](auto& e) { return e.first == dict; });
}

auto Finder::equal_range(const string& dict) const
    -> std::pair<const_iterator, const_iterator>
{
	auto eq = [&](auto& e) { return e.first == dict; };
	auto a = find_if(begin(), end(), eq);
	auto b = find_if_not(a, end(), eq);
	return {a, b};
}

/**
 * Gets the dictionary.
 *
 * @param dict name or path of dictionary without the trailing .aff/.dic.
 * @return the full path to dictionary or empty if does not exists.
 */
auto Finder::get_dictionary(const std::string& dict) const -> string
{
	// first check if it is a path
	if (dict.find_first_of(SEPARATORS) != dict.npos) {
		// a path
		return dict;
	}
	else {
		// search list
		auto x = find(dict);
		if (x != end())
			return x->second;
	}
	return "";
}
} // namespace nuspell
