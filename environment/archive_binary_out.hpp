#pragma once
#include <sstream>
#include <vector>
#include <map>
#include <set>

/**
 * Serialisation Archive for binary output.
 */
class ArchiveBinaryOut
{
public:
    /**
     * Constructor. This automatically opens the file.
     */
    ArchiveBinaryOut() {}

    /**
     * Grab the data.
     */
    std::string data() { return m_sout.str(); }

    /**
     * Archiving operator.
     */
    template<typename T>
    friend ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const T& value)
        { value.serializeOut(ar); return ar; }

    /**
     * Archiving operator.
     */
    template<typename TValue>
    friend ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const std::vector<TValue>& vec)
        { ar.write(vec); return ar; }

    template<typename TValue>
    friend ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const std::set<TValue>& mset)
        { ar.write(mset); return ar; }

    template<typename TKey,typename TValue>
    friend ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const std::pair<TKey,TValue>& value)
        { ar.write(value); return ar; }

    template<typename TKey,typename TValue>
    friend ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const std::map<TKey,TValue>& value)
        { ar.write(value); return ar; }

private:
    /**
     * Helper function to write primitives.
     */
    template<typename T>
    void writePrimitive(const T& value);

    /**
     * Helper function to write stl container primitives.
     */
    template<typename T>
    void write(const std::vector<T>& vec);

    template<typename T>
    void write(const std::set<T>& mset);

    template<typename TKey,typename TValue>
    void write(const std::pair<TKey,TValue>& item) { operator%(*this,item.first); operator%(*this,item.second); }

    template<typename TKey,typename TValue>
    void write(const std::map<TKey,TValue>& map);

    /**
     * String stream.
     */
    std::ostringstream m_sout;

};


/**
 * Generic write primitive function.
 */
template<typename T>
void ArchiveBinaryOut::writePrimitive(const T& value)
{
    m_sout.write(reinterpret_cast<const char*>(&value),sizeof(T));
}

/**
 * Generic write vector
 */
template<typename T>
void ArchiveBinaryOut::write(const std::vector<T>& value) {
    size_t size = value.size();
    m_sout.write(reinterpret_cast<char*>(&size),sizeof(size_t));
    for(typename std::vector<T>::const_iterator iter = value.begin(); iter != value.end(); ++iter)
        operator%(*this, *iter);
}

template<typename T>
void ArchiveBinaryOut::write(const std::set<T>& mset) {
    size_t size = mset.size();
    m_sout.write(reinterpret_cast<char*>(&size),sizeof(size_t));
    for(typename std::set<T>::const_iterator iter = mset.begin(); iter != mset.end(); ++iter)
        operator%(*this, *iter);
}

/**
 * Generic write maps
 */
template<typename TKey, typename TValue>
void ArchiveBinaryOut::write(const std::map<TKey,TValue>& map) {
    size_t size = map.size();
    m_sout.write(reinterpret_cast<char*>(&size),sizeof(size_t));
    for(typename std::map<TKey,TValue>::const_iterator iter = map.begin(); iter != map.end(); ++iter) {
        operator%(*this,iter->first);
        operator%(*this,iter->second);
    }
}

/**
 * Generic write primitive function.
 */
template<typename T>
ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const T& value);

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const int& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const unsigned int& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const long& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const long long& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const unsigned long& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const unsigned long long& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const char& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const unsigned char& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const float& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const double& value) { ar.writePrimitive(value); return ar; }

template<>
inline ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const bool& value) { ar.writePrimitive(value); return ar; }

template<>
ArchiveBinaryOut& operator%(ArchiveBinaryOut& ar,const std::string& value);

