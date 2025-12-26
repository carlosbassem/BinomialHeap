#ifndef VALUECONVERTER_H
#define VALUECONVERTER_H

#include <QString>
#include <stdexcept>

// Template class to convert between types and QString
template <typename T>
class ValueConverter {
public:
    static QString toString(T value);
    static T fromString(const QString& str, bool* ok = nullptr);
    static QString getTypeName();
    static QString getPlaceholder();
};

// Specialization for int
template <>
inline QString ValueConverter<int>::toString(int value) {
    return QString::number(value);
}

template <>
inline int ValueConverter<int>::fromString(const QString& str, bool* ok) {
    return str.toInt(ok);
}

template <>
inline QString ValueConverter<int>::getTypeName() {
    return "Integer";
}

template <>
inline QString ValueConverter<int>::getPlaceholder() {
    return "Enter integer value";
}

// Specialization for char
template <>
inline QString ValueConverter<char>::toString(char value) {
    return QString(QChar(value));
}

template <>
inline char ValueConverter<char>::fromString(const QString& str, bool* ok) {
    if (ok) {
        *ok = (str.length() == 1);
    }
    if (str.length() != 1) {
        if (ok) *ok = false;
        return '\0';
    }
    return str.at(0).toLatin1();
}

template <>
inline QString ValueConverter<char>::getTypeName() {
    return "Character";
}

template <>
inline QString ValueConverter<char>::getPlaceholder() {
    return "Enter single character";
}

#endif // VALUECONVERTER_H
