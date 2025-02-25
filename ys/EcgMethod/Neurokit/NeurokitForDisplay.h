#ifndef NEUROKITFORDISPLAY_H
#define NEUROKITFORDISPLAY_H

#include "FilterPipe.h"
#include "AlgVector.h"
#include "ViewerAppConfig.h"
#include "findpeaks.h"

namespace ys
{
    enum EcgDetectMethod
    {
        MaxPeak = 1,
        MinPeak,
        MaxAbsPeak,
        MaxDvDt,
        MinDvDt,
        MaxAbsDvDt
    };

    template <typename DataType>
    class NeurokitForDisplay
    {
    public:
        NeurokitForDisplay(int sampleRate)
        {
            this->sampleRate_ = sampleRate;

            this->lastAvgRInterval = sampleRate * 10;//初始值设置为极大

            filterPipe_.SetBufferSize(1);
            filterPipe_.AddBandPass(5, 15, 4, sampleRate);
        }

        template <typename Iter>
        void PreProcess(
            ViewerAppConfig& config,
            Iter begin, Iter end,
            std::vector<DataType>& filtered,
            std::vector<DataType>& position,
            std::vector<DataType>& different,
            std::vector<DataType>& avgShort,
            std::vector<DataType>& avgLong,
            std::vector<DataType>& bkg)
        {
            std::vector<DataType> input(begin, end);

            int expSize = std::min((int)input.size(), (int)round(lastAvgRInterval * 5));
            if (expSize > 0 && input.size() > expSize)
            {
                input.erase(input.begin(), input.end() - expSize);
            }

            qDebug() << "++++有效数据长度：" << input.size();

            //先滤波
            //filtered = FilterData(input.begin(), input.end());
            filtered = input;

            //再输出位置波形
            if (config.getShowPosition())
            {
                position.assign(filtered.begin(), filtered.end());
                PositionData(position, config.getDetectPosition());
            }

            //差分
            different = config.getDifferentSignal() ? DifferentData(filtered) : filtered;
            //绝对值
            for (auto& v : different)
            {
                v = (DataType)std::abs(v);
            }

            //平均
            avgShort = Smooth(different, config.getSmallWindowSizeRate());
            avgLong = Smooth(different, config.getBigWindowSizeRate());
            if (avgShort.empty() || avgLong.empty())
            {
                return;
            }

            //阈值
            double threshold = config.getAutoBgScale() ?
                               GetBkgThreshold(avgShort, avgLong) :
                               config.getBgScaleFactor();
            if (config.getAutoBgScale())
            {
                if (threshold < 1.25)
                {
                    threshold = 1.25;
                }
                if (threshold > 2.5)
                {
                    threshold = 2.5;
                }
            }
            config.setBgScaleFactor(threshold);

            //背景
            bkg.assign(avgLong.begin(), avgLong.end());
            for (auto& v : bkg)
            {
                v *= threshold;
            }
        }

        std::vector<int> FindPeak(
            ViewerAppConfig& config,
            const std::vector<DataType>& signal,
            const std::vector<DataType>& avgShort,
            const std::vector<DataType>& bkg) const
        {
            double qrsMinAvgQRS = 0.4;                   //qrs间隔最小占用平均qrs间隔比例
            double peakMinSample = 0.3;
            int offsetX = config.MaxDispPointsCount - (int)signal.size();

            std::vector<int> peaks;//空的

            if (avgShort.size() != signal.size() ||
                bkg.size() != signal.size())
            {
                return peaks;
            }

            const int num_samples = (int)signal.size();
            int mindelay_samples = (int)std::round(sampleRate_* peakMinSample);

            std::vector<int> qrs(num_samples, false);
            for (int i = 0; i < num_samples; i++)
            {
                qrs[i] = avgShort[i] > bkg[i];
            }

            //清空
            config.peakSectionLock.lock();
            config.dispPeakSection.clear();
            config.peakSectionLock.unlock();

            config.peakLock.lock();
            config.dispRPeaks.clear();
            config.peakLock.unlock();

            std::vector<int> beg_qrs;
            std::vector<int> end_qrs, end_qrs2;

            for (int i = 0; i < num_samples - 1; i++)
            {
                if (!qrs[i] && qrs[i + 1])
                {
                    beg_qrs.push_back(i);
                }
                if (qrs[i] && !qrs[i + 1])
                {
                    end_qrs.push_back(i);
                }
            }
            if (beg_qrs.size() == 0)
            {
                return peaks;
            }
            int temp = beg_qrs[0];
            for (int i = 0; i < end_qrs.size(); i++)
            {
                if (end_qrs[i] > temp)
                {
                    end_qrs2.push_back(end_qrs[i]);
                }
            }
            if (end_qrs2.size() == 0)
            {
                return peaks;
            }
            end_qrs = end_qrs2;
            int num_qrs = std::min(beg_qrs.size(), end_qrs.size());

            //复制section
            config.peakSectionLock.lock();
            for (int i = 0; i < num_qrs; ++i)
            {
                config.dispPeakSection.push_back(QPointF(offsetX + beg_qrs[i], 0));
                config.dispPeakSection.push_back(QPointF(offsetX + beg_qrs[i] + 0.05, 1000));
                config.dispPeakSection.push_back(QPointF(offsetX + end_qrs[i] - 0.05, 1000));
                config.dispPeakSection.push_back(QPointF(offsetX + end_qrs[i], 0));
            }
            config.peakSectionLock.unlock();

            double min_len = (std::accumulate(end_qrs.begin(), end_qrs.begin() + num_qrs, 0) - std::accumulate(beg_qrs.begin(), beg_qrs.begin() + num_qrs, 0)) / num_qrs;
            min_len *= qrsMinAvgQRS;
            min_len = round(min_len);

            for (int i = 0; i < num_qrs; i++)
            {
                int beg = beg_qrs[i];
                int end = end_qrs[i];
                int len_qrs = end - beg;

                //过窄的R波峰删除
                if (len_qrs < (int)min_len)
                {
                    continue;
                }

                std::vector<DataType> data(signal.begin() + beg, signal.begin() + end);
                std::vector<DataType> props;

                auto locmax = findPeaks(data);
                for (int i = 0; i < locmax.size(); i++)
                {
                    props.push_back(data[locmax[i]]);
                }

                if (!locmax.empty())
                {
                    auto max_it = std::max_element(props.begin(), props.end());
                    int max_idx = std::distance(props.begin(), max_it);
                    int peak_idx = beg + locmax[max_idx];

                    bool valid_peak = true;
                    for (int j = peaks.size() - 1; j >= 0 && valid_peak; j--)
                    {
                        if (peak_idx - peaks[j] < mindelay_samples)
                        {
                            valid_peak = false;//峰峰宽度小于0.x秒，则删除
                        }
                    }

                    if (valid_peak)
                    {
                        peaks.push_back(peak_idx);
                    }
                }
            }

            //添加peak
            config.peakLock.lock();
            for (int i = 0; i < peaks.size(); ++i)
            {
                config.dispRPeaks.push_back(QPointF(offsetX + peaks[i], signal[peaks[i]]));
            }
            config.peakLock.unlock();

            return peaks;
        }

        const int CalcHeartRate(const std::vector<int>& peaks) const
        {
            //计算心率
            //从实验结果分析来看，开头波峰不受影响，结尾波峰偶尔受滑动平均影响。
            //方法：删除最后波峰，用前n-1个波峰计算。
            if (peaks.size() <= 3)
            {
                return 0;
            }

            if (peaks.size() == 4)
            {
                return (int)std::round(60.0 * sampleRate_ / (peaks[peaks.size() - 2] - peaks[peaks.size() - 3]));
            }

            return (int)std::round(60.0 * sampleRate_ * 2 / (peaks[peaks.size() - 2] - peaks[peaks.size() - 4]));
        }

        void UpdateInterval(const std::vector<int>& peaks)
        {
            if (peaks.size() < 4)
            {
                lastAvgRInterval = sampleRate_ * 10;
            }
            else
            {
                lastAvgRInterval = (double)(peaks[peaks.size() - 2] - peaks[1]) / (peaks.size() - 3);
            }
        }
    private:
        int sampleRate_;
        FilterPipe<DataType> filterPipe_;

        //上一次R峰宽度，检波时保证5个波峰宽度即可。
        //如果失效，没检测出波峰，则用最大长度检测。
        double lastAvgRInterval { 0 };

        ///
        /// \brief FilterData
        /// 输入到滤波，带阻滤波。
        /// \param begin
        /// \param end
        /// \return
        ///
        template <typename FwIt>
        std::vector<DataType> FilterData(FwIt begin, FwIt end)
        {
            return filterPipe_.ProcessNoState(begin, end);
        }

        ///
        /// \brief PositionData
        /// 根据选择的检测类型，只处理检测位置数据。
        /// 预处理成：最小波谷，最大dv/dt等。
        /// \param method
        /// \param inout
        ///
        void PositionData(std::vector<DataType>& inout, int method = MaxPeak)
        {
            switch (method)
            {
                case ys::EcgDetectMethod::MinPeak:
                    for (auto& v : inout)
                    {
                        v = -v;
                    }
                    return;
                case ys::EcgDetectMethod::MaxAbsPeak:
                    for (auto& v : inout)
                    {
                        v = std::abs(v);
                    }
                    return;
                case ys::EcgDetectMethod::MaxDvDt:
                    for (int i = 1; i < inout.size(); ++i)
                    {
                        inout[i] = inout[i] - inout[i - 1];
                    }
                    inout[0] = inout[1];
                    break;
                case ys::EcgDetectMethod::MinDvDt:
                    for (int i = 1; i < inout.size(); ++i)
                    {
                        inout[i] = -(inout[i] - inout[i - 1]);
                    }
                    inout[0] = inout[1];
                    break;
                case ys::EcgDetectMethod::MaxAbsDvDt:
                    for (int i = 1; i < inout.size(); ++i)
                    {
                        inout[i] = std::abs(inout[i] - inout[i - 1]);
                    }
                    inout[0] = inout[1];
                    break;
                default:
                    //不处理
                    return;
            }
        }

        ///
        /// \brief DifferentData
        /// 差分数据，防止T波干扰。
        /// \param input
        /// \return
        ///
        std::vector<DataType> DifferentData(const std::vector<DataType>& input)
        {
            return AlgVector::Gradient(input);
        }

        ///
        /// \brief 滑动窗口平均
        /// \param begin
        /// \param end
        /// \param kernelFactor
        /// \return
        ///
        std::vector<DataType> Smooth(const std::vector<DataType>& input, double kernelFactor)
        {
            int kernelSize = (int)round(sampleRate_* kernelFactor);
            int bufferSize = (int)input.size();
            if (bufferSize <= kernelSize)
            {
                return std::vector<DataType>();
            }
            int halfSize = kernelSize / 2;

            std::vector<DataType> output(bufferSize);
            //先累加窗口内和
            double sum = 0.0;
            for (int i = 0; i < kernelSize; ++i)
            {
                sum += input[i];
            }

            //前面补齐效果不好，改成滑动平均
            // {
            //     double sum1 = sum;
            //     for (int i = 0; i < halfSize; ++i) {
            //         sum1 -= input[i];
            //         output[halfSize - 1 - i] = sum1 / (kernelSize - 1 - i);
            //     }
            // }

            //中间部分产生输出
            for (int i = 0; i < bufferSize - kernelSize; ++i)
            {
                output[i + halfSize] = sum / kernelSize;
                sum -= input[i];
                sum += input[i + kernelSize];
            }

            //补齐前面
            for (int i = halfSize - 1; i >= 0; --i)
            {
                output[i] = output[i + 1];
            }

            //补齐后面
            for (int i = bufferSize - kernelSize + halfSize; i < bufferSize; ++i)
            {
                output[i] = output[i - 1];
            }
            return output;
        }

        double GetBkgThreshold(const std::vector<DataType>& fg,
                               const std::vector<DataType>& bg)
        {
            std::vector<DataType> tmp(fg.size());
            std::transform(fg.cbegin(), fg.cend(), bg.cbegin(), tmp.begin(),
                           [](DataType t1, DataType t2)
                           {
                               return t1 - t2;
                           });

            //用峰值的1半作为阈值
            auto maxit = std::max_element(tmp.begin(), tmp.end());
            DataType v = *maxit;
            int maxindex = std::distance(tmp.begin(), maxit);

            return (v * 0.5 + bg[maxindex]) / bg[maxindex];

        }
    };
}

#endif // NEUROKITFORDISPLAY_H
