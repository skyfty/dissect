#ifndef SQLITEQSTRING_H
#define SQLITEQSTRING_H

#include <QColor>
#include <QDateTime>
#include <QString>
#include <sqlite_orm.h>

namespace sqlite_orm {

namespace qstring_helper{

inline std::string GenderToString(const QString &str) {
    return str.toStdString();
}
inline std::unique_ptr<QString> GenderFromString(const std::string& s) {
    return std::make_unique<QString>(QString::fromStdString(s));
}
}

template<>
struct type_printer<QString> : public text_printer {};

template<>
struct statement_binder<QString> {
    int bind(sqlite3_stmt* stmt, int index, const QString& value) {
        return statement_binder<std::string>().bind(stmt, index, qstring_helper::GenderToString(value));
    }
};
template<>
struct field_printer<QString> {
    std::string operator()(const QString& t) const {
        return qstring_helper::GenderToString(t);
    }
};

template<>
struct row_extractor<QString> {
    QString extract(const char* row_value) {
        if(auto val = qstring_helper::GenderFromString(row_value)) {
            return *val;
        } else {
            throw std::runtime_error("incorrect gender string (" + std::string(row_value) + ")");
        }
    }
    QString extract(sqlite3_stmt* stmt, int columnIndex) {
        auto str = sqlite3_column_text(stmt, columnIndex);
        return this->extract((const char*)str);
    }
};
}


namespace sqlite_orm {

namespace qcolor_helper{

inline std::string GenderToString(const QColor &v) {
    return v.name().toStdString();
}
inline std::unique_ptr<QColor> GenderFromString(const std::string& s) {
    return std::make_unique<QColor>(QColor::fromString(s));
}
}

template<>
struct type_printer<QColor> : public text_printer {};

template<>
struct statement_binder<QColor> {
    int bind(sqlite3_stmt* stmt, int index, const QColor& value) {
        return statement_binder<std::string>().bind(stmt, index, qcolor_helper::GenderToString(value));
    }
};
template<>
struct field_printer<QColor> {
    std::string operator()(const QColor& t) const {
        return qcolor_helper::GenderToString(t);
    }
};

template<>
struct row_extractor<QColor> {
    QColor extract(const char* row_value) {
        if(auto val = qcolor_helper::GenderFromString(row_value)) {
            return *val;
        } else {
            throw std::runtime_error("incorrect gender string (" + std::string(row_value) + ")");
        }
    }
    QColor extract(sqlite3_stmt* stmt, int columnIndex) {
        auto str = sqlite3_column_text(stmt, columnIndex);
        return this->extract((const char*)str);
    }
};
}



namespace sqlite_orm {

namespace qdatetime_helper{

inline qint64 GenderToInteger(const QDateTime &v) {
    return v.toMSecsSinceEpoch();
}
inline std::unique_ptr<QDateTime> GenderFromInteger(qint64 s) {
    return std::make_unique<QDateTime>(QDateTime::fromMSecsSinceEpoch(s));
}
}

template<>
struct type_printer<QDateTime> : public integer_printer {};

template<>
struct statement_binder<QDateTime> {
    int bind(sqlite3_stmt* stmt, int index, const QDateTime& value) {
        return statement_binder<qint64>().bind(stmt, index, qdatetime_helper::GenderToInteger(value));
    }
};
template<>
struct field_printer<QDateTime> {
    qint64 operator()(const QDateTime& t) const {
        return qdatetime_helper::GenderToInteger(t);
    }
};

template<>
struct row_extractor<QDateTime> {
    QDateTime extract(qint64 row_value) {
        if(auto val = qdatetime_helper::GenderFromInteger(row_value)) {
            return *val;
        } else {
            throw std::runtime_error("incorrect gender string ()");
        }
    }
    QDateTime extract(sqlite3_stmt* stmt, int columnIndex) {
        auto str = sqlite3_column_int64(stmt, columnIndex);
        return this->extract(str);
    }
};
}

#endif // SQLITEQSTRING_H
