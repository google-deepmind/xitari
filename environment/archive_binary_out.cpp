#include "archive_binary_out.hpp"

template<>
ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const std::string& value) {
    size_t size = value.size();
    ar.m_sout.write(reinterpret_cast<char*>(&size),sizeof(size_t));
    ar.m_sout.write(reinterpret_cast<const char*>(&value[0]),static_cast<int>(value.size()));

    return ar;
}
