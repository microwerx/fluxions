#ifndef FLUXIONS_FILE_READER_WRITER_HPP
#define FLUXIONS_FILE_READER_WRITER_HPP

#include <fluxions_stdcxx.hpp>

namespace Fluxions
{

// Interface for File Writer
template <typename T>
struct TFileWriter {
	virtual ~TFileWriter() { close(); }
	virtual bool open(const char* path) = 0;
	virtual bool open(const std::string& path) { return open(path.c_str()); }
	virtual void close() {};
	virtual bool write(const T* obj) = 0;
};

// Interface for File Reader
template <typename T>
struct TFileReader {
	virtual ~TFileReader() { close(); }
	virtual bool open(const char* path) = 0;
	virtual bool open(const std::string& path) { return open(path.c_str()); }
	virtual void close() {}
	virtual bool read(T* ssg) = 0;
};

}

#endif
