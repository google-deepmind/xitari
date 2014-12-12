#include "archive_binary_in.hpp"

ArchiveBinaryIn::ArchiveBinaryIn(const std::string& data) {
    std::string uncompressed(data.size(),'\0');
    copy(data.data(), data.data() + data.size(), uncompressed.begin());

    // write to stream
    m_sin.str(uncompressed);
}

template<>
ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,std::string& value) {
    size_t size;
    ar.readPrimitive(size);

    value.resize(size);
    ar.m_sin.read(&value[0],static_cast<int>(size));

    return ar;
}

// specialised type to speed up reading of images
template<>
void ArchiveBinaryIn::read<unsigned char>(std::vector<unsigned char>& value) {
    size_t size;
    readPrimitive(size);

    value.resize(size);
    m_sin.read(reinterpret_cast<char*>(&value[0]),static_cast<int>(size));
}

template<>
void ArchiveBinaryIn::read<char>(std::vector<char>& value) {
    size_t size;
    readPrimitive(size);

    value.resize(size);
    m_sin.read(&value[0],static_cast<int>(size));
}

