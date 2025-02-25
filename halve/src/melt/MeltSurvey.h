#ifndef MELTPOSITION_H
#define MELTPOSITION_H


#include <HalveType.h>
#include <channel/ChannelData.h>

class ElectrodeNode;
class MeltSetting;

typedef std::tuple<qint32, qint32,double, qint32, double> ElectrodeNodeFruit;

class MeltSurvey
{
public:
    enum  Germ{
        FIRST,
        MIDDLE,
        LAST
    };
public:
    explicit MeltSurvey(const ChannelData::List &samplingDatas);
    int operator()(Halve::Perception perception, int b , int e, Germ germ) const;
    std::vector<int> operator()(Halve::Perception perception, int b , int e) const;
    std::vector<int> operator()(Halve::Perception perception) const;

    static int germValue(const std::vector<int> &datas,int b, Germ germ);
    static int germValue(const std::vector<int> &datas,Germ germ);


    quint16 samplingRate() const;
    void setSamplingRate(quint16 newSamplingRate);

    ElectrodeNodeFruit calculateSamplingData(const ElectrodeNode* electrodeNode, qint32 consultDataIndex, MeltSetting* meltSetting, Halve::Perception perc);


private:
    std::vector<int> trans(Halve::Perception perception,ChannelData::List::const_iterator begin, ChannelData::List::const_iterator end) const;


private:
    quint16 m_samplingRate = 2048;
    const ChannelData::List &m_samplingDatas;
};

#endif // MELTPOSITION_H
