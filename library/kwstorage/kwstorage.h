#pragma once

#include <string>
#include <memory>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <utils/unused.h>

namespace NKwStorage {

class TKwIteratorImpl {
public:
    virtual void Next() = 0;
    virtual bool End() = 0;
    virtual bool Reset() = 0;
    virtual void Seek(const std::string& key) {
    }
    virtual std::pair<std::string, std::string> Get() = 0;
};

typedef std::shared_ptr<TKwIteratorImpl> TKwIterator;

class TKwStorage: public boost::noncopyable {
public:
    virtual void Put(const std::string& key, const std::string& value) {
        UNUSED(key);
        UNUSED(value);
    }
    virtual boost::optional<std::string> Get(const std::string& key) {
        UNUSED(key);
        return boost::optional<std::string>();
    }
    virtual bool Exists(const std::string& key) {
        return false;
    }
    virtual void Flush() {
    }
    virtual TKwIterator Iterator() {
        assert(false && "not implemented");
    }
protected:
    TKwStorage() {
    }
};

} // NKwStorage
