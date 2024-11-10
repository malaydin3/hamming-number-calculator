#include <iostream>
#include <set>
#include <queue>
#include <stack>
#include <math.h>

using namespace std::chrono;
using namespace std;
// define a typedef for 64-bit integer and use it in the rest of the code
typedef unsigned long long int _intType;

class HammingNumber{
    public:
        // exponents for the humming number
        _intType p, q, r;

        // pre-calculated natural logarithms of 2,3, and 5
        double ln2 = 0.69314718055994530941;
        double ln3 = 1.09861228866810969139;
        double ln5 = 1.60943791243410037460;

        // default constructor which initializes the Hamming 
        HammingNumber() : p(0), q(0), r(0) {}

        // take the log of the hamming number
        inline double log() const {
            return ln2 * p + ln3 * q + ln5 * r;
        }

        // operator overloads - multiplication with the factors - increment the exponent 
        // works only with the factor of 2,3 and 5 
        friend HammingNumber operator*(HammingNumber u, int fact) {
            if (fact == 2) u.p++;
            if (fact == 3) u.q++;
            if (fact == 5) u.r++;

            return u;
        }

        // operator overload for comparison in the log space for positive numbers
        friend bool operator<(const HammingNumber& l, const HammingNumber& r) {
            return l.log() < r.log();
        }

        // operator overload for equality - exponents must be identical for l and r
        friend bool operator==(const HammingNumber& l, const HammingNumber& r) {
            return l.p == r.p && l.q == r.q && l.r == r.r;
        }
        // operator overload for printing Hamming number - also print out the floating point representation
        friend std::ostream& operator<<(std::ostream& os, const HammingNumber& HN)
        {
            double result = pow(2, HN.p) * pow(3, HN.q) * pow(5, HN.r);
            os << "2**" << HN.p << " * 3**" << HN.q << " * 5**" << HN.r << " = " << result;
            return os;
        }

};


class HammingCalculator    {
    private:
        // target value to reach 
        const _intType _targetNumber;
        // define the fixed multipliers for the problem
        const std::vector<int> _multipliers{2, 3, 5};
        // limiter for the naive solver
        const _intType naiveLimit = 10000;
    public:
        // store the results publicly
        HammingNumber _resultHamming;
        _intType _resultInt;

        // define a constructor
        HammingCalculator(_intType t): _targetNumber(t), _resultHamming(HammingNumber()), _resultInt(1)  {}

        // First solution to the problem using a min-heap based priority queue and a set to keep track of visited entries in the array 
        // Time complexity: O (N log N )
        // Memory complexity: O ( N )
        void Solve_Naive()
        {
            // check if the target number is larger than 1500, if refuse to solve because we can overflow!!! 
            if (_targetNumber > naiveLimit)
            {
                std::cout << "ERR: Solve_Naive is not compatible for target numbers larger than " << naiveLimit << " due to potential overflows!!! Please USE Hamming solver \n";
                return;
            } 
            auto a = high_resolution_clock::now();
            // this is a min-heap version of the pq 
            std::priority_queue<_intType, vector<_intType>, greater<_intType> > _PQ;
            // push the first element 
            _PQ.push(1);
            // this set keeps track of integers that have been already seen so to eleminate duplicates because PQ implementation in STL allows duplicates...
            std::set<_intType> _tracer;
            // start with adding 1 to the tracer
            _tracer.insert(1);
            // a counter for halting the loop
            _intType cntr = 0;
            //loop until we reach to the target number in size
            while(cntr < _targetNumber)
            {
                // get the minimum element in the PQ
                _resultInt = _PQ.top();
                _PQ.pop();
                cntr++;
                // push the element to the results vector and factorize it with the given multipliers
                for (std::vector<int>::const_iterator it = _multipliers.begin(); it != _multipliers.end(); it++)
                {
                    _intType candidate = (*it) * _resultInt;
                    const bool isSeen = _tracer.find(candidate) != _tracer.end();
                    // not seen before, then add it to the PQ
                    if (!isSeen){
                        _PQ.push(candidate);
                        _tracer.insert(candidate);
                    }
                }
            }

            // store the result
            auto b = high_resolution_clock::now();
            cout << "Hamming number at the position " << _targetNumber << " is " << _resultInt << " and took " << duration_cast<nanoseconds>(b - a).count() << " nanoseconds with Solve_Naive" <<  endl;

            return;
        }

        // Time complexity: O ( N )
        // Memory complexity: O ( N )
        void Solve_Neat_Hamming() {
            auto a = high_resolution_clock::now();
            // this is the vector where we collect Hamming numbers
            std::vector<HammingNumber> res;
            // pre-allocate some already-known space for the vector to reduce memory allocation costs
            res.reserve(_targetNumber);

            // initialize multipliers
            _intType p = 0;
            _intType q = 0;
            _intType r = 0;
            
            // multipliers of 2,3 and 5 in HammingNumber representation
            HammingNumber next, next2, next3, next5;

            // this is the container where we keep track of things
            res.push_back(HammingNumber());

            while (res.size() < size_t(_targetNumber) + 1) {
                // this will automatically work because we have overloaded comparison operators
                next = std::min(next2, std::min(next3, next5));

                res.push_back(next);

                if (next == next2) next2 = res[++p] * 2;
                if (next == next3) next3 = res[++q] * 3;
                if (next == next5) next5 = res[++r] * 5;
            }
            // Store the result
            _resultHamming = res[_targetNumber];
            auto b = high_resolution_clock::now();
            cout << "Hamming number at the position " << _targetNumber << " is " << res[_targetNumber] << " and took " << duration_cast<seconds>(b - a).count() << " seconds with Solve_Neat_Hamming" <<  endl;
            return;
        }

        // Similar approach but use a sliding window for not running out of memory for big numbers
        // Time complexity: O ( N )
        // Memory complexity:  < O ( N )
        void Solve_Neater_Hamming() {
            auto a = high_resolution_clock::now();
            // double ended queue 
            std::deque<HammingNumber> res;

            // window size for removing elements at the front of the dequeue
            _intType offset = 0;

            // initialize multipliers
            _intType p = 0;
            _intType q = 0;
            _intType r = 0;

            // initialize Hamming numbers
            HammingNumber next, next2, next3, next5;

            // start with 1
            res.push_back(HammingNumber());

            // to account for removed elements from the list we substract the offset amount
            while (res.size() < size_t(_targetNumber) - offset + 1) {
                next = std::min(next2, std::min(next3, next5));

                res.push_back(next);

                if (next == next2) next2 = res[++p - offset] * 2;
                if (next == next3) next3 = res[++q - offset] * 3;
                if (next == next5) next5 = res[++r - offset] * 5;

                size_t min = std::min(p, std::min(q, r));

                // sliding window has moved past some Hamming numbers that are no longer needed
                // offset = number of popped elements from the original deque determined by the minimum of p,q, and r
                if (offset < min) {
                    res.pop_front();
                    offset++;
                }

            }
            _resultHamming = res[_targetNumber - offset];
            auto b = high_resolution_clock::now();
            cout << "Hamming number at the position " << _targetNumber << " is " << res[_targetNumber - offset] << " and took " << duration_cast<seconds>(b - a).count() << " seconds with Solve_Neater_Hamming" <<  endl;
            return;
    }

};


// main function
int main()
{
    // solution to the first problem
    // solve the first problem = 1500 th 
    HammingCalculator* problem1 = new HammingCalculator(1500);
    problem1->Solve_Naive();
    problem1->Solve_Neat_Hamming();
    problem1->Solve_Neater_Hamming();
    cout << "***************" <<  endl;
            

    // solve the second problem = 100000 th 
    HammingCalculator* problem2 = new HammingCalculator(100000);
    problem2->Solve_Neat_Hamming();
    problem2->Solve_Neater_Hamming();
    cout << "***************" <<  endl;

    // solve the last problem = 4 000 000 000 th 
    HammingCalculator* problem3 = new HammingCalculator(4000000000);
    problem3->Solve_Neater_Hamming();

    // free up the memory 
    delete problem1;
    delete problem2;
    delete problem3;

    return 0;
}