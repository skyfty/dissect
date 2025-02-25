#include <QTest>

#include <channel/ChannelData.h>

#include <melt/MeltSurvey.h>

class TestMeltSurvey: public QObject
{
    Q_OBJECT
private slots:
    void test1();
};
void TestMeltSurvey::test1()
{

//    std::vector<ChannelData::DataType> consultDatas{100, 200, 2250,100,140, 2250,12, 2250,2250,2250, 90,0};
//    MeltSurvey consultSurvey(MeltElectrode::Max);
//    int consultDataIndex = consultSurvey(consultDatas);

//    std::vector<ChannelData::DataType> samplingDatas;
//    samplingDatas.resize(consultDatas.size());

//    MeltSurvey samplingSurvey(MeltElectrode::Max);
//    int samplingDataIndex = consultSurvey(consultDatas);

}
QTEST_MAIN(TestMeltSurvey)
#include "tst_melt_survey.moc"
