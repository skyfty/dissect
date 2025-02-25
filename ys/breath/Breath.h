#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include "..\3rd-party\findpeaks\findpeaks.h"

namespace ys {

template <typename InputType>
class Breath {
public:
    Breath(int fps = 20) {
        this->fps = fps;
    }

    template <typename FwIt>
    void FindPeakValley(
        FwIt begin, FwIt end,
        std::vector<size_t>& peaks,
        std::vector<size_t>& valleys) {

		std::vector<InputType> data(begin, end);
		SubMean(data);
		std::vector<InputType> data_inverse(data.size());
		std::transform(data.begin(), data.end(), data_inverse.begin(), [](auto v) {return -v; });

		peaks = FindPeak(data, 0.5);
		valleys = FindPeak(data_inverse, 0.5);
    }
    
private:
    Breath(const Breath& b) = delete;
    Breath(Breath&& b) = delete;
    Breath& operator=(const Breath& b) = delete;
    Breath& operator=(Breath&& b) = delete;

    int fps{ 1 };

	double FindGradThresholdWeight(
		const std::vector<InputType>& fg,
		const std::vector<InputType>& bg) {

		std::vector<InputType> tmp(fg.size());
		std::transform(fg.cbegin(), fg.cend(), bg.cbegin(), tmp.begin(), [](InputType t1, InputType t2) {
			return t1 - t2;
			});

		//用峰值的1半作为阈值
		auto maxit = std::max_element(tmp.begin(), tmp.end());
		auto maxindex = std::distance(tmp.begin(), maxit);
		return (*maxit * 0.5 + bg[maxindex]) / bg[maxindex];
	}
	void WindowAverage(
		const std::vector<InputType>& inputData,
		std::vector<InputType>& outputData,
		int kernelSize) {

		int num_samples = (int)inputData.size();
		int halfk = kernelSize / 2;
		int pos2 = num_samples - 1 - halfk;

		int end = std::min(num_samples - 1, halfk);
		InputType sum = std::accumulate(inputData.begin(), inputData.begin() + end, 0.0f);
		int count = halfk + 1;
		for (int i = 0; i < halfk; i++, count++) {
			sum += inputData[i + halfk];
			outputData[i] = sum / count;
		}
		for (int i = halfk; i <= pos2; i++) {
			sum += inputData[i + halfk];
			sum -= inputData[i - halfk];
			outputData[i] = sum / count;
		}

		for (int i = pos2 + 1; i < num_samples; i++) {
			count--;
			sum -= inputData[i - halfk];
			outputData[i] = sum / count;
		}
	}
public:
    std::vector<size_t> FindPeak(
        const std::vector<InputType>& signal,
        float smallwindow) {

        int small_kernel = (int)round(smallwindow * fps);
		int num_samples = (int)signal.size();

		std::vector<InputType> smoothgrad(num_samples);
		WindowAverage(signal, smoothgrad, small_kernel);

        std::vector<size_t> locmax = findPeaks(smoothgrad);
        std::vector<size_t> peaks(locmax.begin(), locmax.end());
		if (!peaks.empty() && peaks.back() == num_samples - 1)
			peaks.erase(peaks.end() - 1);
		return peaks;
    }

	void SubMean(std::vector<InputType>& vec) {
		if (vec.empty())
			return;

		InputType mean = (InputType)(std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size());
		std::transform(vec.begin(), vec.end(), vec.begin(), [mean](InputType v) {return v - mean; });
	}
};

}
