#ifndef CHANNELTRACADAPTER_H
#define CHANNELTRACADAPTER_H

#include "ChannelTrackStorage.h"
#include "channel/ChannelTrackOrm.h"

struct ChannelTrackStorageAdapter {
    static auto make(const QString &filePath) {
        auto storage = make_storage(filePath.toStdString(),
            make_table("channeltrack",
               make_column("id",  &ChannelTrackData::m_id, unique()),
               make_column("time",&ChannelTrackData::m_time, default_value(0)),
               make_column("n0",&ChannelTrackData::m_n0),
               make_column("n1",&ChannelTrackData::m_n1),
               make_column("n2",&ChannelTrackData::m_n2),
               make_column("n3",&ChannelTrackData::m_n3),
               make_column("n4",&ChannelTrackData::m_n4),
               make_column("n5",&ChannelTrackData::m_n5),
               make_column("n6",&ChannelTrackData::m_n6),
               make_column("n7",&ChannelTrackData::m_n7),
               make_column("m0",&ChannelTrackData::m_m0),
               make_column("m1",&ChannelTrackData::m_m1),
               make_column("m2",&ChannelTrackData::m_m2),
               make_column("m3",&ChannelTrackData::m_m3),
               make_column("m4",&ChannelTrackData::m_m4),
               make_column("m5",&ChannelTrackData::m_m5),
               make_column("m6",&ChannelTrackData::m_m6),
               make_column("m7",&ChannelTrackData::m_m7),
               make_column("m8",&ChannelTrackData::m_m8),
               make_column("m9",&ChannelTrackData::m_m9),
               make_column("m10",&ChannelTrackData::m_m10),
               make_column("m11",&ChannelTrackData::m_m11),
               make_column("m12",&ChannelTrackData::m_m12),
               make_column("m13",&ChannelTrackData::m_m13),
               make_column("m14",&ChannelTrackData::m_m14),
               make_column("m15",&ChannelTrackData::m_m15),
               make_column("m16",&ChannelTrackData::m_m16),
               make_column("m17",&ChannelTrackData::m_m17),
               make_column("m18",&ChannelTrackData::m_m18),
               make_column("m19",&ChannelTrackData::m_m19),
               make_column("m20",&ChannelTrackData::m_m20),
               make_column("m21",&ChannelTrackData::m_m21),
               make_column("m22",&ChannelTrackData::m_m22),
               make_column("m23",&ChannelTrackData::m_m23),
               make_column("m24",&ChannelTrackData::m_m24),
               make_column("m25",&ChannelTrackData::m_m25),
               make_column("m26",&ChannelTrackData::m_m26),
               make_column("m27",&ChannelTrackData::m_m27),
               make_column("m28",&ChannelTrackData::m_m28),
               make_column("m29",&ChannelTrackData::m_m29),
               make_column("m30",&ChannelTrackData::m_m30),
               make_column("m31",&ChannelTrackData::m_m31),
               make_column("m32",&ChannelTrackData::m_m32),
               make_column("m33",&ChannelTrackData::m_m33),
               make_column("m34",&ChannelTrackData::m_m34),
               make_column("m35",&ChannelTrackData::m_m35),
               make_column("m36",&ChannelTrackData::m_m36),
               make_column("m37",&ChannelTrackData::m_m37),
               make_column("m38",&ChannelTrackData::m_m38),
               make_column("m39",&ChannelTrackData::m_m39),
               make_column("m40",&ChannelTrackData::m_m40),
               make_column("m41",&ChannelTrackData::m_m41),
               make_column("m42",&ChannelTrackData::m_m42),
               make_column("m43",&ChannelTrackData::m_m43),
               make_column("m44",&ChannelTrackData::m_m44),
               make_column("m45",&ChannelTrackData::m_m45),
               make_column("m46",&ChannelTrackData::m_m46),
               make_column("m47",&ChannelTrackData::m_m47)
            ));
        storage.sync_schema();
        storage.pragma.synchronous(0);
        return storage;
    }
};
typedef decltype(ChannelTrackStorageAdapter::make(QString())) ChannelTrackStorageSchema;

#endif // CHANNELTRACADAPTER_H
