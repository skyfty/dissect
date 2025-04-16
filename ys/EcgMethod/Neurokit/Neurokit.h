#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include "findpeaks.h"

namespace ys
{
    template <typename InputType>
    class Neurokit
    {
    public:
        Neurokit() = default;
        Neurokit(int sampleRate)
        {
            _sampleRate = sampleRate;
        }

        const int GetSampleRate() const
        {
            return _sampleRate;
        }
        void SetSampleRate(const int rate)
        {
            _sampleRate = rate;
        }

        std::vector<int> FindPeak(const InputType *p, size_t len)
        {

            std::vector<InputType> signal( &p[0], &p[len]);
            return FindPeak(signal);
        }
        std::vector<int> FindPeak(const std::vector<InputType> &signal)
        {
            return FilteredToPeak(signal);
        }
    protected:
        int _sampleRate;

        double FindBkgThresholdWeight(
            const std::vector<InputType> &fg,
            const std::vector<InputType> &bg)
        {

            std::vector<InputType> tmp(fg.size());
            std::transform(fg.cbegin(), fg.cend(), bg.cbegin(), tmp.begin(), [](InputType t1, InputType t2)
                           {
                               return t1 - t2;
                           });

            //用峰值的1半作为阈值
            auto maxit = std::max_element(tmp.begin(), tmp.end());
            auto maxindex = std::distance(tmp.begin(), maxit);
            return ( *maxit * 0.5 + bg[maxindex]) / bg[maxindex];
        }

        bool WindowAverage(
            std::vector<InputType> &inputData,
            std::vector<InputType> &outputData,
            int kernelSize)
        {
            int bufferSize = (int)inputData.size();
            if (bufferSize <= kernelSize)
            {
                return false;
            }
            int halfSize = kernelSize / 2;

            outputData.resize(bufferSize);

            //中间部分正常滑动平均。
            //开头、结尾部分直接复制首尾点。
            //防止头尾数据不足，波形上翘。

            //先累加窗口内和
            double sum = 0.0;
            for (int i = 0; i < kernelSize; ++i)
            {
                sum += inputData[i];
            }

            //中间部分产生输出
            for (int i = 0; i < bufferSize - kernelSize; ++i)
            {
                outputData[i + halfSize] = sum / kernelSize;
                sum -= inputData[i];
                sum += inputData[i + kernelSize];
            }

            //补齐前面
            for (int i = halfSize - 1; i >= 0; --i)
            {
                outputData[i] = outputData[i + 1];
            }

            //补齐后面
            for (int i = bufferSize - kernelSize + halfSize; i < bufferSize; ++i)
            {
                outputData[i] = outputData[i - 1];
            }

            return true;
        }
        std::vector<int> FilteredToPeak(
            const std::vector<InputType> &filtered,
            float windowShort = 0.05f,  //短平均窗口（占采样率的比例）
            float windowLong = 0.75f,   //长平均窗口（占采样率的比例）
            float minRWeight = 0.4f,    //R峰宽度最小值（占R峰均值的比例）
            float minQrsWeight = 0.2f)  //R峰之间的最小值（占采样率的比例）最高能检测心率300/分。
        {
            auto &signal = filtered;

            std::vector<int> peaks;
            int inputSize = (int)signal.size();

            std::vector<InputType> diff, diffAbs(inputSize);
            gradient(signal, diff, diffAbs);

            int kernelShort = round(windowShort *_sampleRate);
            int kernelLong = round(windowLong *_sampleRate);

            std::vector<InputType> avgShort(inputSize);
            std::vector<InputType> avgLong(inputSize);
            if (!WindowAverage(diffAbs, avgShort, kernelShort))
            {
                //数据过短，不处理
                return peaks;
            }
            if (!WindowAverage(diffAbs, avgLong, kernelLong))
            {
                //数据过短，不处理
                return peaks;
            }

            //自动找背景放大系数
            double bkgWeight = FindBkgThresholdWeight(avgShort, avgLong);
            if (bkgWeight < 1.25)
            {
                bkgWeight = 1.25;
            }
            if (bkgWeight > 2.5)
            {
                bkgWeight = 2.5;
            }

            std::vector<InputType> bkg(inputSize);
            std::transform(avgLong.begin(), avgLong.end(), bkg.begin(),
                           [bkgWeight] (InputType val)
                           {
                               return bkgWeight *val;
                           });

            int minQrsWidth = std::round(_sampleRate *minQrsWeight);

            std::vector<int> qrs(inputSize, false);
            for (int i = 0; i < inputSize; i++)
            {
                qrs[i] = avgShort[i] > bkg[i];
            }

            std::vector<int> beg_qrs;
            std::vector<int> end_qrs, end_qrs2;

            for (int i = 0; i < inputSize - 1; i++)
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
            InputType minAvgRWidth = (std::accumulate(end_qrs.begin(), end_qrs.begin() + num_qrs, 0) - std::accumulate(beg_qrs.begin(), beg_qrs.begin() + num_qrs, 0)) / num_qrs;
            minAvgRWidth *= minRWeight;

            for (int i = 0; i < num_qrs; i++)
            {
                int beg = beg_qrs[i];
                int end = end_qrs[i];
                int len_qrs = end - beg;

                //过窄的R波峰删除
                if (len_qrs < minAvgRWidth)
                {
                    continue;
                }

                std::vector<InputType> data(signal.begin() + beg, signal.begin() + end);
                std::vector<InputType> props;

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
                        if (peak_idx - peaks[j] < minQrsWidth)
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

            return peaks;
        }

        std::vector<InputType> gradient(const std::vector<InputType> &input)
        {
            if (input.size() <= 2)
            {
                return input;
            }

            std::vector<InputType> res(input.size());
            for (int j = 1; j < (int)input.size() - 1; j++)
            {
                res[j] = (input[j + 1] - input[j - 1]) / 2.0f;
            }
            res.front() = input[1] - input.front();
            res.back() = input.back() - input[input.size() - 2];

            return res;
        }
        void gradient(
            const std::vector<InputType> &input,
            std::vector<InputType> &diff,
            std::vector<InputType> &diffAbs)
        {

            if (input.size() <= 2)
            {
                return;
            }

            diff.resize(input.size());
            diffAbs.resize(input.size());
            for (int j = 1; j < (int)input.size() - 1; j++)
            {
                diff[j] = (input[j + 1] - input[j - 1]) / 2.0f;
                diffAbs[j] = diff[j] * diff[j];
            }
            diff.front() = input[1] - input.front();
            diff.back() = input.back() - input[input.size() - 2];

            diffAbs.front() = diff.front() * diff.front();
            diffAbs.back() = diff.back() * diff.back();
        }
    };

}
