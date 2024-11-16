#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>
#include <stdexcept>
#include <iomanip>

// Класс для модуляции QAM
class QAMModulator {
public:
    QAMModulator(int m) : M(m) {
        if (M != 4 && M != 16 && M != 64) {
            throw std::invalid_argument("Supported modulations are QPSK (4), QAM16 (16), and QAM64 (64)");
        }
        generateConstellation();
    }

    std::vector<std::complex<double>> modulate(const std::vector<int>& bits) {
        int bitsPerSymbol = std::log2(M);
        if (bits.size() % bitsPerSymbol != 0) {
            throw std::invalid_argument("Bitstream length must be a multiple of log2(M)");
        }

        std::vector<std::complex<double>> symbols;
        for (size_t i = 0; i < bits.size(); i += bitsPerSymbol) {
            int index = 0;
            for (int j = 0; j < bitsPerSymbol; j++) {
                index = (index << 1) | bits[i + j];
            }
            symbols.push_back(constellation[index]);
        }
        return symbols;
    }

private:
    int M;
    std::vector<std::complex<double>> constellation;

    void generateConstellation() {
        int dim = std::sqrt(M);
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                constellation.emplace_back(2 * i - (dim - 1), 2 * j - (dim - 1));
            }
        }
    }
};

// Класс для добавления гауссовского шума
class GaussianNoise {
public:
    GaussianNoise(double noiseVariance) : distribution(0.0, std::sqrt(noiseVariance)) {}

    std::vector<std::complex<double>> addNoise(const std::vector<std::complex<double>>& symbols) {
        std::vector<std::complex<double>> noisySymbols;
        for (const auto& symbol : symbols) {
            noisySymbols.emplace_back(
                symbol.real() + distribution(generator),
                symbol.imag() + distribution(generator)
            );
        }
        return noisySymbols;
    }

private:
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
};

// Класс для демодуляции QAM
class QAMDemodulator {
public:
    QAMDemodulator(int m) : M(m) {
        if (M != 4 && M != 16 && M != 64) {
            throw std::invalid_argument("Supported demodulations are QPSK (4), QAM16 (16), and QAM64 (64)");
        }
        generateConstellation();
    }

    std::vector<int> demodulate(const std::vector<std::complex<double>>& symbols) {
        std::vector<int> bits;
        int bitsPerSymbol = std::log2(M);
        for (const auto& symbol : symbols) {
            int index = findNearestSymbol(symbol);
            for (int j = bitsPerSymbol - 1; j >= 0; j--) {
                bits.push_back((index >> j) & 1);
            }
        }
        return bits;
    }

private:
    int M;
    std::vector<std::complex<double>> constellation;

    void generateConstellation() {
        int dim = std::sqrt(M);
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                constellation.emplace_back(2 * i - (dim - 1), 2 * j - (dim - 1));
            }
        }
    }

    int findNearestSymbol(const std::complex<double>& symbol) {
        double minDistance = std::numeric_limits<double>::max();
        int nearestIndex = 0;
        for (int i = 0; i < constellation.size(); i++) {
            double distance = std::norm(symbol - constellation[i]);
            if (distance < minDistance) {
                minDistance = distance;
                nearestIndex = i;
            }
        }
        return nearestIndex;
    }
};

int main() {
    int M = 16; // QAM16
    int numBits = 16; // количество генерируемых бит
    std::vector<double> noiseVariances = { 0.1, 0.5, 1, 1.5,2 }; // Значения дисперсии шума

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> bitDist(0, 1);

    std::vector<int> bits(numBits);
    for (int& bit : bits) {
        bit = bitDist(generator);
    }
  
    // Модуляция
    QAMModulator modulator(M);
    auto modulatedSymbols = modulator.modulate(bits);
    
    std::cout << "Original Bits:" << std::endl;;
    for (int bit : bits) {

        std::cout << bit;
    }
    std::cout << std::endl;
    std::cout << "Noise Variance\tBit Error Rate (BER)" << std::endl;
    for (double noiseVariance : noiseVariances) {
        // Добавление шума
        GaussianNoise noise(noiseVariance);
        auto noisySymbols = noise.addNoise(modulatedSymbols);

        // Демодуляция
        QAMDemodulator demodulator(M);
        auto demodulatedBits = demodulator.demodulate(noisySymbols);

        // Подсчёт ошибок
        int errors = 0;
        for (int i = 0; i < bits.size(); i++) {
            if (bits[i] != demodulatedBits[i]) {
                errors++;
            }
        }

        // Расчёт BER
        double ber = static_cast<double>(errors) / numBits;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << noiseVariance << "\t\t" << ber <<std::endl;
 
        // Вывод данных
        std::cout << std::endl << "Modulated Symbols:" << std::endl;;
        for (const auto& symbol : modulatedSymbols) {
            std::cout << "(" << symbol.real() << ", " << symbol.imag() << ") ";
        }
        std::cout << std::endl;
        std::cout << "Noisy Symbols:" << std::endl;;
        for (const auto& symbol : noisySymbols) {
            std::cout << "(" << symbol.real() << ", " << symbol.imag() << ") ";
        }
        std::cout << std::endl;
        std::cout << "---------------------------------" << std::endl;;
        std::cout << "Demodulated Bits:" << std::endl;;
        for (int bit : demodulatedBits) {
            std::cout << bit;
        }
        std::cout << std::endl << "---------------------------------" << std::endl;;
       
    }

    return 0;
}
