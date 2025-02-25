#pragma once

#include "FilterCoefficients.h"
#include "EnumFilterType.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <type_traits>
#include <algorithm>
#include <numeric>

namespace ys
{

    typedef std::vector<FilterCoefficients> ComplexCoefficients;

    template <typename InputType>
    class Filter
    {
        static_assert(std::is_arithmetic<InputType>::value, "input value type must be numeric!");

    public:
        Filter() { }
        Filter(const FilterCoefficients& co)
        {
            assert(co._CoefficientsA.size() > 0);
            assert(co._CoefficientsB.size() > 0);
            assert(co._CoefficientsA.size() == co._CoefficientsB.size());

            _FilterCoefficients = co;

            _Delay.resize(_FilterCoefficients._CoefficientsA.size());
        }
        Filter(const Filter<InputType>& f)
        {
            *this = f;
        }

        const Filter<InputType>& operator=(const Filter<InputType>& f)
        {
            assert(f._FilterCoefficients._CoefficientsA.size() > 0);
            assert(f._FilterCoefficients._CoefficientsB.size() > 0);
            assert(f._FilterCoefficients._CoefficientsA.size() == f._FilterCoefficients._CoefficientsB.size());
            assert(f._FilterCoefficients._CoefficientsA.size() == f._Delay.size());

            _FilterCoefficients = f._FilterCoefficients;
            _Delay = f._Delay;

            return *this;
        }

        const std::vector<InputType>& Process(const InputType* p, size_t len)
        {
            assert(p != nullptr);
            assert(len > 0);

            _Output.resize(len);

            for (size_t i = 0; i < len; ++i)
            {
                _Output.at(i) = CalcOneStep(p[i]);
            }

            return _Output;
        }
        const std::vector<InputType>& Process(const InputType* p, size_t len, const InputType* pOldState, size_t stateLen)
        {
            assert(p != nullptr);
            assert(len > 0);
            assert(pOldState != nullptr);
            assert(stateLen > 0);
            assert(stateLen == _FilterCoefficients._CoefficientsA.size());

            _Delay.clear();
            _Delay.assign(& pOldState[0], &pOldState[stateLen]);

            return Process(p, len);
        }
        const std::vector<InputType>& Process(const std::vector<InputType>& input)
        {
            assert(input.size() > 0);
            return Process(& input[0], input.size());
        }
        const std::vector<InputType>& Process(const std::vector<InputType>& input, const std::vector<InputType>& oldState)
        {
            assert(input.size() > 0);
            assert(oldState.size() > 0);
            return Process(& input[0], input.size(), &oldState[0], oldState.size());
        }

        InputType CalcOneStep(const InputType input)
        {
            InputType output = (InputType)(_FilterCoefficients._CoefficientsB.at(0) * input + _Delay.at(0));
            for (size_t j = 1; j < _FilterCoefficients._CoefficientsA.size(); ++j)
            {
                _Delay.at(j - 1) = _FilterCoefficients._CoefficientsB.at(j) * input
                                   + _Delay.at(j)
                                   - _FilterCoefficients._CoefficientsA.at(j) * output;
            }
            return output;
        }

        void ClearState()
        {
            for (int i = 0; i < _Delay.size(); ++i)
            {
                _Delay[i] = 0;
            }
        }
    protected:
        FilterCoefficients _FilterCoefficients;
        std::vector<InputType> _Delay;
        std::vector<InputType> _Output;
    };

}
