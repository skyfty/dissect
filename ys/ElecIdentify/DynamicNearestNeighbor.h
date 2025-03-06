#ifndef DYNAMICNEARESTNEIGHBOR_H
#define DYNAMICNEARESTNEIGHBOR_H

#include <hnswlib/hnswlib.h>
#include <vector>
#include <mutex>  // 用于多线程并发支持
#include <Eigen/Eigen>

namespace ys
{
    struct KNNCell
    {
        Eigen::Vector3f ep, mp;    // 电坐标，磁坐标
        Eigen::Vector3f k;
    };

    class DynamicNearestNeighbor
    {
    private:
        int dimension;                              // 数据维度
        std::unique_ptr<hnswlib::L2Space> space;    // HNSW空间（欧氏距离）
        hnswlib::HierarchicalNSW<float>* index;     // HNSW索引
        std::mutex indexMutex;                     // 用于线程安全
        size_t maxSize;                            // 最大点数
        size_t currentCount = 0;                   // 当前点数
        std::vector<KNNCell> attributeCells;
    public:
        // 构造函数
        DynamicNearestNeighbor(size_t maxSize = 144000*2, int dim = 3);

        // 析构函数
        ~DynamicNearestNeighbor();

        // 插入新点及其属性
        void AddPoint(const std::vector<float>& point, const KNNCell& attribute_value);
        void AddPoint(const Eigen::Vector3f& point, const KNNCell& attribute_value);
        void AddPoint(const float* point, const KNNCell& attribute_value);

        // 查询最近邻点的属性值
        const KNNCell* Query(const std::vector<float>& query_point, int k = 1);
        const KNNCell* Query(const Eigen::Vector3f& query_point, int k = 1);
        const KNNCell* Query(const float* query_point, int k = 1);

        size_t GetPointsCount() const;
    };
}

#endif // DYNAMICNEARESTNEIGHBOR_H
