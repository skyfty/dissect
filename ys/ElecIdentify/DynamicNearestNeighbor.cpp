#include "DynamicNearestNeighbor.h"

namespace ys
{
void DynamicNearestNeighbor::AddPoint(const float *point, const KNNCell &attribute_value)
{
    std::lock_guard<std::mutex> lock(indexMutex);           // 确保线程安全
    if (currentCount >= maxSize) {
        // 如果超过最大元素数量，则扩展容量
        maxSize *= 2;
        index->resizeIndex(maxSize);
        attributeCells.resize(maxSize);
    }
    index->addPoint(point, currentCount);                 // 插入点
    attributeCells[currentCount] = attribute_value;       // 更新属性映射
    currentCount++;
}

size_t DynamicNearestNeighbor::GetPointsCount() const
{
    return currentCount;
}

DynamicNearestNeighbor::DynamicNearestNeighbor(size_t maxSize, int dim)
    : maxSize(maxSize), dimension(dim)
{
    space = std::make_unique<hnswlib::L2Space>(dimension);                      // 使用欧氏距离
    index = new hnswlib::HierarchicalNSW<float>(space.get(), maxSize);         // 初始化HNSW索引
    index->ef_construction_ = 200;                                               // 构建效率参数
    index->setEf(100);
    attributeCells.resize(maxSize);
}

DynamicNearestNeighbor::~DynamicNearestNeighbor()
{
    delete index;
}

void DynamicNearestNeighbor::AddPoint(const std::vector<float> &point, const KNNCell &attribute_value)
{
    AddPoint(point.data(), attribute_value);
}

void DynamicNearestNeighbor::AddPoint(const vtkeigen::Vector3f &point, const KNNCell &attribute_value)
{
    AddPoint(point.data(), attribute_value);
}

const DynamicNearestNeighbor::KNNCell* DynamicNearestNeighbor::Query(const vtkeigen::Vector3f &query_point, int k)
{
    return Query(query_point.data(), k);
}

const DynamicNearestNeighbor::KNNCell* DynamicNearestNeighbor::Query(const std::vector<float> &query_point, int k)
{
    return Query(query_point.data(), k);
}

const DynamicNearestNeighbor::KNNCell* DynamicNearestNeighbor::Query(const float *query_point, int k)
{
    std::lock_guard<std::mutex> lock(indexMutex);                              // 确保线程安全
    std::priority_queue<std::pair<float, hnswlib::labeltype>> result =
        index->searchKnn(query_point, k);                                      // 最近邻搜索
    if (!result.empty()) {
        auto neighbor = result.top();                                           // 最近邻
        size_t neighbor_id = neighbor.second;                                   // 最近邻ID
        auto& cell = attributeCells[neighbor_id];
        return &cell;
    }
    return nullptr;
}

}
