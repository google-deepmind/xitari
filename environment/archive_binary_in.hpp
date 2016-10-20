#pragma once
#include <sstream>
#include <vector>
#include <map>
#include <set>

/**
 * Serialisation Archive for binary input.
 */
class ArchiveBinaryIn
{
public:
    /**
     * Constructor.  This opens the file.
     */
    ArchiveBinaryIn(const std::string& data);

    /**
     * The main archiving operator.
     */
    template<typename T>
    friend ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,T& value)
        { value.serializeIn(ar); return ar; }

    template<typename TValue>
    friend ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,std::vector<TValue>& vec)
        { ar.read(vec); return ar; }

    template<typename TValue>
    friend ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,std::set<TValue>& mset)
        { ar.read(mset); return ar; }

    template<typename TKey,typename TValue>
    friend ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,std::pair<TKey,TValue>& item)
        { ar.read(item); return ar; }

    template<typename TKey,typename TValue>
    friend ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,std::map<TKey,TValue>& map)
        { ar.read(map); return ar; }

private:
    /**
     * Helper function to read primitives.
     */
    template<typename T>
    void readPrimitive(T& value);

    /**
     * Helper functions to read stl containers.
     */
    template<typename T>
    void read(std::vector<T>& vec);

    template<typename T>
    void read(std::set<T>& mset);

    template<typename TKey,typename TValue>
    void read(std::pair<TKey,TValue>& item) { operator%(*this,item.first); operator%(*this,item.second); }

    template<typename TKey,typename TValue>
    void read(std::map<TKey,TValue>& map);

    /**
     * string stream.
     */
    std::stringstream m_sin;

};

template<typename T>
void ArchiveBinaryIn::readPrimitive(T& value)
{
    static char bytes[sizeof(T)];
    m_sin.read(bytes,sizeof(T));

    value = *reinterpret_cast<T*>(bytes);
}

// specialisation for efficiency
template<>
void ArchiveBinaryIn::read<unsigned char>(std::vector<unsigned char>& value);

// specialisation for efficiency
template<>
void ArchiveBinaryIn::read<char>(std::vector<char>& value);

template<typename T>
void ArchiveBinaryIn::read(std::vector<T>& vec) {
    assert(vec.empty());
    int size = 0;
    readPrimitive(size);

    vec.resize(size);

    for(int index = 0;index < size;++index)
        operator%(*this,vec[index]);
}

template<typename T>
void ArchiveBinaryIn::read(std::set<T>& mset) {
    assert(mset.empty());
    int size = 0;
    readPrimitive(size);

    for(int index = 0;index < size;++index) {
        T value;
        operator%(*this,value);
        mset.emplace(value);
    }
}

template<typename TKey,typename TValue>
void ArchiveBinaryIn::read(std::map<TKey,TValue>& map) {
    assert(map.empty());
    int size = 0;
    readPrimitive(size);

    for(int index = 0;index < size;++index) {
        TKey key;
        operator%(*this,key);
        TValue value;
        operator%(*this,value);
        map.emplace(key,value);
    }
}

template<typename T>
ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,T& value);

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,int& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,unsigned int& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,long& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,long long& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,unsigned long& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,unsigned long long& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,char& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,unsigned char& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,float& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,double& value) { ar.readPrimitive(value); return ar; }

template<>
inline ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,bool& value) { ar.readPrimitive(value); return ar; }

template<>
ArchiveBinaryIn& operator%(ArchiveBinaryIn& ar,std::string& value);

