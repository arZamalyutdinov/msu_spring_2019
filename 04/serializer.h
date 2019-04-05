#pragma once
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer {
    private:
        static constexpr char separator = ' ';
        std::ostream &out_;

        template<typename T, typename... Args>
        Error process(T&&, Args&&...);

        Error process();

        template<typename T> 
        Error write(const T&);

        Error write(const uint64_t);
        Error write(const bool);
        Error write();

    public:
        explicit Serializer(std::ostream&);
        
        template<typename T>
        Error save(T&);

        template<typename... Args>
        Error operator()(Args&&...);

};

class Deserializer {
    private:
        std::istream &in_;

        template<typename T, typename... Args>
        Error process(T&&, Args&&...);

        Error process();

        template<typename T>
        Error read(const T&);

        Error read(bool&);
        Error read(uint64_t&);
        Error read();

    public:
        explicit Deserializer(std::istream&);

        template<typename T>
        Error load(T&);

        template<typename... Args>
        Error operator()(Args&&...);
};

//Serializer class

Serializer::Serializer(std::ostream &out) : out_(out) {};

template<typename T>
Error Serializer::save(T &obj) 
{
    return obj.serialize(*this);
}

template<typename... Args>
Error Serializer::operator()(Args&&... args) 
{
    return process(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Error Serializer::process(T &&value, Args&&... args)
{
    if (write(value) == Error::NoError && process(std::forward<Args>(args)...) == Error::NoError) {
        return Error::NoError;
    }
    return Error::CorruptedArchive;
}

Error Serializer::process()
{
    return Error::NoError;
}

template<typename T>
Error Serializer::write(const T& obj)
{
    return Error::CorruptedArchive;
}

Error Serializer::write(const uint64_t value) 
{
    out_ << value << separator;
    return Error::NoError;
}

Error Serializer::write(const bool value) 
{
    out_ << (value ? "true" : "false") << separator;
    return Error::NoError;
}

Error Serializer::write()
{
    return Error::NoError;
}


// Deserializer class

Deserializer::Deserializer(std::istream &in) : in_(in) {};

template<typename T>
Error Deserializer::load(T &obj)
{
    return obj.serialize(*this);
}

template<typename... Args>
Error Deserializer::operator()(Args&&... args)
{
    return process(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Error Deserializer::process(T &&value, Args&&... args)
{
    if (read(value) == Error::NoError && process(std::forward<Args>(args)...) == Error::NoError) {
        return Error::NoError;
    }
    return Error::CorruptedArchive;
}

Error Deserializer::process()
{
    return Error::NoError;
}

template<typename T>
Error Deserializer::read(const T &value)
{
    return Error::CorruptedArchive;
}

Error Deserializer::read(uint64_t &value)
{
    std::string str;
    in_ >> str;
    if (str[0] == '-') {
        return Error::CorruptedArchive;
    }
    try {
        value = std::stoul(str);
    } catch (const std::invalid_argument &ex) {
        return Error::CorruptedArchive;
    }
    return Error::NoError;
}

Error Deserializer::read(bool &value)
{
    std::string str;
    in_ >> str;
    if (str == "false") {
        value = false;
    } else if (str == "true") {
        value = true;
    } else {
        return Error::CorruptedArchive;
    }
    return Error::NoError;
}

Error Deserializer::read() 
{
    return Error::NoError;
}