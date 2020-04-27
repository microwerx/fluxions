#include "fluxions_pch.hpp"
#include <fluxions_base.hpp>
#include <fluxions_file_path_info.hpp>

namespace Fluxions {
	namespace fs = std::filesystem;

	using FXubyte = unsigned char;
	using FXbyte = char;
	using FXshort = short;
	using FXushort = unsigned short;
	using FXint = int;
	using FXuint = unsigned int;
	using FXint64 = long long;
	using FXuint64 = unsigned long long;
	using FXfloat = float;
	using FXdouble = double;

	std::string& make_path_forward_slashes(std::string& s) {
		std::regex path_replace("[/\\\\]+");
		s = std::move(std::regex_replace(s, path_replace, "/"));
		return s;
	}

	FilePathInfo::FilePathInfo() {}

	FilePathInfo::FilePathInfo(const std::string& path) {
		reset(path);
	}

	FilePathInfo::FilePathInfo(const std::string& path, const string_vector& paths) {
		reset(path, paths);
	}

	void FilePathInfo::_clear() {
		pathType_ = PathType::None;
		//atime = 0;
		last_write_time_ = fs::file_time_type();
		root_path_.clear();
		origpath.clear();
		absolute_path_.clear();
		filename_.clear();
		stem_.clear();
		extension_.clear();
	}

	bool FilePathInfo::reset(const std::string& path) {
		_clear();

		std::regex path_replace("[/\\\\]+");
		origpath = std::move(std::regex_replace(path, path_replace, "/"));

		fs::path abs_path = fs::absolute(origpath, fpi_ec_);
		fs::path rel_path = fs::relative(abs_path, fs::current_path(), fpi_ec_);

		root_name_ = abs_path.root_name().string();
		absolute_path_ = abs_path.string();
		relative_path_ = rel_path.string();
		root_path_ = abs_path.parent_path().string();
		parent_path_ = rel_path.parent_path().string();
		extension_ = rel_path.extension().string();
		filename_ = rel_path.filename().string();
		stem_ = rel_path.stem().string();

		make_path_forward_slashes(absolute_path_);
		make_path_forward_slashes(relative_path_);
		make_path_forward_slashes(parent_path_);
		make_path_forward_slashes(root_path_);

		if (!parent_path_.empty()) {
			if (parent_path_.back() != '/') parent_path_.append("/");
		}
		if (!root_path_.empty()) {
			if (root_path_.back() != '/') root_path_.append("/");
		}

		return _fill_stat_info();
	}

	bool FilePathInfo::reset(const std::string& path, const string_vector& paths) {
		if (reset(path)) return true;
		for (auto& dir : paths) {
			if (reset(dir + path)) return true;
		}
		return false;
	}

	std::string FilePathInfo::GetAbsolutePath(const std::string& path) {
		std::error_code ec;
		fs::path absolute_path = fs::absolute(path, ec);
		std::string p = absolute_path.string();
		return make_path_forward_slashes(p);
	}

	std::string FilePathInfo::GetCurrentPath() {
		std::error_code ec;
		fs::path absolute_path = fs::current_path(ec);
		std::string p = absolute_path.string();
		return make_path_forward_slashes(p);
	}

	bool FilePathInfo::TestIfPathExists(const std::string& filename) {
		std::error_code ec;
		return fs::exists(filename, ec);
	}

	bool FilePathInfo::FindIfPathExists(std::string& path, const string_vector& paths) {
		// Is there a file name to test?
		if (path.empty())
			return false;

		std::error_code ec;
		if (fs::exists(path, ec))
			return true;

		for (const std::string& dir : paths) {
			if (dir.empty()) continue;
			char backChar = dir.back();
			std::string testPath = dir;
			if ((backChar != '/') && (backChar != '\\'))
				testPath += std::string("/") + path;
			else
				testPath += path;

			if (fs::exists(path, ec)) {
				path = testPath;
				return true;
			}
		}

		return false;
	}

	bool FilePathInfo::_fill_stat_info() {
		absolute_path_exists_ = fs::exists(absolute_path_);
		if (!absolute_path_exists_) {
			pathType_ = PathType::DoesNotExist;
			last_write_time_ = FileTimeValue();
			return false;
		}

		fs::file_status status = fs::status(absolute_path_);
		switch (status.type()) {
		case fs::file_type::directory:
			pathType_ = PathType::Directory;
			relative_path_.push_back('/');
			absolute_path_.push_back('/');
			break;
		case fs::file_type::regular:
			pathType_ = PathType::File;
			break;
		case fs::file_type::not_found:
			pathType_ = PathType::DoesNotExist;
			break;
		default:
			pathType_ = PathType::Other;
		}
		last_write_time_ = fs::last_write_time(absolute_path_);
		return exists();
	}

	std::string ReadTextFile(const std::string& filename) {
		std::ifstream fin(filename.c_str());

		if (!fin)
			return "";

		std::stringstream sstr;
		sstr << fin.rdbuf();
		fin.close();
		return sstr.str();
	}

	byte_array ReadBinaryFile(const std::string& filename) {
		byte_array buffer;
		std::ifstream fin(filename.c_str(), std::ios::binary);

		if (!fin)
			return buffer;

		std::string str;
		fin.seekg(0, std::ios::end);
		size_t size = (size_t)fin.tellg();
		buffer.resize(size);
		fin.seekg(0, std::ios::beg);
		fin.read((char*)&buffer[0], size);

		fin.close();
		return buffer;
	}

	//	std::string FindPathIfExists(const std::string& path, const std::vector<std::string> paths) {
	//		std::string output;
	//
	//		// Is there a file name to test?
	//		FilePathInfo fpi(path);
	//		if (fpi.filename().empty())
	//			return output;
	//
	//		if (TestIfFileExists(path)) {
	//			output = path;
	//		}
	//		else {
	//			for (auto testPathIt : paths) {
	//				std::string testPath = testPathIt;
	//				if (testPath.back() != '/' && testPath.back() != '\\')
	//					testPath += "/";
	//				testPath += fpi.filename();
	//				if (TestIfFileExists(testPath)) {
	//					output = testPath;
	//					break;
	//				}
	//			}
	//		}
	//
	//		return output;
	//	}
	//
	//	std::string NormalizePathName(const std::string& basepath, const std::string& path) {
	//		FpiBlankString = "";
	//
	//		FilePathInfo p1(basepath + "/" + path);
	//		FilePathInfo p2(path);
	//
	//		if (p1.pathType_ == PathType::DoesNotExist) {
	//			if (p2.pathType_ == PathType::DoesNotExist) {
	//				return FpiBlankString;
	//			}
	//			else {
	//				return p2.origpath;
	//			}
	//		}
	//		else {
	//			return p1.origpath;
	//		}
	//	}
	//
	//	PathType GetPathType(const std::string& path) {
	//#ifdef _WIN32
	//		struct _stat Stat;
	//#elif __unix__
	//		struct stat Stat;
	//#endif
	//		int retval = stat_with_errno(path, &Stat);
	//		if (retval != 0) {
	//			// do not print errors...
	//			return PathType::DoesNotExist;
	//		}
	//		if (Stat.st_mode & bitIsDirectory)
	//			return PathType::Directory;
	//		else if (Stat.st_mode & bitIsRegularFile)
	//			return PathType::File;
	//		return PathType::Other;
	//	}
	//
	//	TimeValue GetPathCreationTime(const std::string& path) {
	//#ifdef _WIN32
	//		struct _stat Stat;
	//#elif __unix__
	//		struct stat Stat;
	//#endif
	//
	//		int retval = stat_with_errno(path, &Stat);
	//
	//		if (retval != 0) {
	//			switch (errno) {
	//			case ENOENT:
	//				std::cerr << __func__ << ": file not found" << std::endl;
	//				break;
	//			case EINVAL:
	//				std::cerr << __func__ << ": invalid parameter to _stat()" << std::endl;
	//				break;
	//			default:
	//				std::cerr << __func__ << ": unknown error in _stat()" << std::endl;
	//			}
	//		}
	//		else {
	//			return Stat.st_ctime;
	//		}
	//		return 0;
	//	}
	//
	//	TimeValue GetPathAccessTime(const std::string& path) {
	//#ifdef _WIN32
	//		struct _stat Stat;
	//#elif __unix__
	//		struct stat Stat;
	//#endif
	//		int retval = stat_with_errno(path, &Stat);
	//
	//		if (retval != 0) {
	//			switch (errno) {
	//			case ENOENT:
	//				std::cerr << __func__ << ": file not found" << std::endl;
	//				break;
	//			case EINVAL:
	//				std::cerr << __func__ << ": invalid parameter to _stat()" << std::endl;
	//				break;
	//			default:
	//				std::cerr << __func__ << ": unknown error in _stat()" << std::endl;
	//			}
	//		}
	//		else {
	//			return Stat.st_atime;
	//		}
	//		return 0;
	//	}
} // namespace Fluxions
