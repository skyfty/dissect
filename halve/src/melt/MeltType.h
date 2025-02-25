#ifndef MELTTYPE_H
#define MELTTYPE_H

#include <QString>
#include <channel/ChannelData.h>

class ElectrodeNode;
class QAbstractSeries;

typedef QHash<const ElectrodeNode*, ChannelData::List> ElectrodeNodeHash;
typedef QHash<QString, std::pair<QAbstractSeries*, const ElectrodeNode*>> SeriesData;
typedef std::tuple<qint32, qint32> SeriesIndex;
typedef std::tuple<qint32,std::vector<qint32>> ConsultDataIndex;

#endif // MELTTYPE_H
