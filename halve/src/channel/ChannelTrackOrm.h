#ifndef CHANNELTRACKORM_H
#define CHANNELTRACKORM_H

#include <QDataStream>
#include <sqlite_orm.h>

class ChannelTrackN;
class ChannelTrackM;

namespace sqlite_orm {

namespace channel_track_helper{

template<typename T>
inline std::vector<char> genderToBuffer(const T &v) {
    std::vector<char> buffer;
    buffer.resize(sizeof(T));
    std::memcpy(&buffer[0], &v, sizeof(T));
    return buffer;
}

template<typename T>
inline std::unique_ptr<T> genderFromBuffer(const void *v) {
    auto channelTrack = std::make_unique<T>();
    std::memcpy(channelTrack.get(), v, sizeof(T));
    return std::move(channelTrack);
}
}

template<>
struct type_printer<ChannelTrackN> : public blob_printer {};

template<>
struct statement_binder<ChannelTrackN> {
    int bind(sqlite3_stmt* stmt, int index, const ChannelTrackN& value) {
        return statement_binder<std::vector<char>>().bind(stmt, index, channel_track_helper::genderToBuffer(value));
    }
};
template<>
struct field_printer<ChannelTrackN> {
    std::vector<char> operator()(const ChannelTrackN& t) const {
        return channel_track_helper::genderToBuffer(t);
    }
};

template<>
struct row_extractor<ChannelTrackN> {
    ChannelTrackN extract(const void *str) {
        if(auto val = channel_track_helper::genderFromBuffer<ChannelTrackN>(str)) {
            return *val;
        } else {
            throw std::runtime_error("incorrect gender string ()");
        }
    }
    ChannelTrackN extract(sqlite3_stmt* stmt, int columnIndex) {
        const void *str = sqlite3_column_blob(stmt, columnIndex);
        return this->extract(str);
    }
};

template<>
struct type_printer<ChannelTrackM> : public blob_printer {};

template<>
struct statement_binder<ChannelTrackM> {
    int bind(sqlite3_stmt* stmt, int index, const ChannelTrackM& value) {
        return statement_binder<std::vector<char>>().bind(stmt, index, channel_track_helper::genderToBuffer(value));
    }
};
template<>
struct field_printer<ChannelTrackM> {
    std::vector<char> operator()(const ChannelTrackM& t) const {
        return channel_track_helper::genderToBuffer(t);
    }
};

template<>
struct row_extractor<ChannelTrackM> {
    ChannelTrackM extract(const void *str) {
        if(auto val = channel_track_helper::genderFromBuffer<ChannelTrackM>(str)) {
            return *val;
        } else {
            throw std::runtime_error("incorrect gender string ()");
        }
    }
    ChannelTrackM extract(sqlite3_stmt* stmt, int columnIndex) {
        const void *str = sqlite3_column_blob(stmt, columnIndex);
        return this->extract(str);
    }
};


}

#endif // CHANNELTRACKORM_H
