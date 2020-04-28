#ifndef FLUXIONS_FILEIO_HPP
#define FLUXIONS_FILEIO_HPP

#include <fluxions_stdcxx.hpp>
#include <filesystem>

namespace Fluxions {
	enum class PathType {
		None,
		DoesNotExist,
		Directory,
		File,
		Other
	};

	std::string ReadTextFile(const std::string& filename);
	byte_array ReadBinaryFile(const std::string& filename);

	using FileTimeValue = std::filesystem::file_time_type;

	struct FilePathInfo {
	public:
		FilePathInfo();
		FilePathInfo(const std::string& path);
		FilePathInfo(const std::string& path, const string_vector& paths);

		// returns true if path exists
		bool reset(const std::string& path);

		// returns true if path exists, searching through paths as necessary
		bool reset(const std::string& path, const string_vector& paths);

		const char* shortestPathC() const { return shortestPath().c_str(); }
		const std::string& shortestPath() const { return relative_path_.empty() ? absolute_path_ : relative_path_; }
		const std::string& relativePath() const { return relative_path_; }
		const std::string& absolutePath() const { return absolute_path_; }
		const std::string& parentPath() const { return parent_path_.empty() ? root_path_ : parent_path_; }
		const std::string& filename() const { return filename_; }
		const char* filenameC() const { return filename_.c_str(); }
		const std::string& stem() const { return stem_; }
		const std::string& extension() const { return extension_; }
		const FileTimeValue& lastWriteTime() const { return last_write_time_; }

		// returns true if this file is older than a reference time
		bool olderThan(FileTimeValue reftime) const { return last_write_time_ < reftime; }

		// returns true if this file is newer than a reference time
		bool newerThan(FileTimeValue reftime) const { return last_write_time_ > reftime; }

		// returns true if path was not found
		bool notFound() const { return !absolute_path_exists_; }

		// returns true if path exists
		bool exists() const { return absolute_path_exists_; }

		// returns true if path is a directory
		bool isDirectory() const { return pathType_ == PathType::Directory; }

		// returns true if path is not a directory or regular file
		bool isOther() const { return pathType_ == PathType::Other; }

		// returns true if path is a regular file
		bool isFile() const { return pathType_ == PathType::File; }

		// returns true if relative path exists
		bool isRelative() const { return !relative_path_.empty(); }

		// returns absolute path of file
		static std::string GetAbsolutePath(const std::string& path);

		// returns absolute path of current working directory
		static std::string GetCurrentPath();

		// returns true if path exists
		static bool TestIfPathExists(const std::string& path);

		// returns true if path exists, searching through paths as necessary
		static bool FindIfPathExists(std::string& path, const string_vector& paths);
	private:
		// The type of path, initially PathType::None
		PathType pathType_{ PathType::None };

		// time file last changed
		FileTimeValue last_write_time_;

		// error code for std::filesystem issues
		std::error_code fpi_ec_;
		// Original path string for this structure
		std::string origpath;
		// The directory for this file with final slash.
		std::string root_path_;
		// The directory for this file with the final slash.
		std::string parent_path_;
		// The full filename with extension.
		std::string filename_;
		// The filename minus the final extension.
		std::string stem_;
		// The extension from the filename.
		std::string extension_;
		// The full path name for the filename.
		std::string absolute_path_;
		// The relative path name
		std::string relative_path_;
		// The root name of the file (such as C: or //resource)
		std::string root_name_;

		bool absolute_path_exists_{ false };

		void _clear();
		bool _fill_stat_info();
	};
} // namespace Fluxions

#endif
