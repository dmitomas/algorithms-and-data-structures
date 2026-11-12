#include "Matrix.h"
#include <complex>
#include <iostream>
#include <string>
#include <utility>


template <typename T>
void demonstrate_matrix(const std::string& title) {
    std::cout << title << "\n----------------------------------\n";

    Matrix<T> empty;
    std::cout << "Empty matrix size: " << empty.rows() << "x" << empty.cols() << '\n';

    Matrix<T> filled(3, 3, T{ 1 });
    std::cout << "Filled matrix:\n" << filled;

    Matrix<T> random(3, 3, static_cast<T>(0), static_cast<T>(9));
    std::cout << "Random matrix:\n" << random;
    std::cout << "Size: " << random.rows() << "x" << random.cols() << '\n';

    Matrix<T> copy = random;
    std::cout << "Copy of random:\n" << copy;

    Matrix<T> moved = std::move(copy);
    std::cout << "Moved matrix:\n" << moved;

    Matrix<T> assigned(2, 2, T{});
    assigned = random;
    std::cout << "After assigned = random:\n" << assigned;

    Matrix<T> moved_assigned(2, 2, T{});
    moved_assigned = std::move(assigned);
    std::cout << "After moved_assigned = std::move(assigned):\n" << moved_assigned;

    random(0, 0) = static_cast<T>(42);
    std::cout << "After random(0,0) = 42:\n" << random;
    std::cout << "random(1,1) = " << random(1, 1) << '\n';

    Matrix<T> tmp(3, 3, T{ 1 });
    tmp += random;
    std::cout << "tmp += random:\n" << tmp;
    tmp -= random;
    std::cout << "tmp -= random:\n" << tmp;
    tmp *= static_cast<T>(2);
    std::cout << "tmp *= 2:\n" << tmp;
    tmp /= static_cast<T>(2);
    std::cout << "tmp /= 2:\n" << tmp;

    Matrix<T> sum = random + filled;
    Matrix<T> diff = random - filled;
    Matrix<T> prod = random * filled;
    Matrix<T> scaled_left = static_cast<T>(2) * random;
    Matrix<T> scaled_right = random * static_cast<T>(2);
    Matrix<T> divided = random / static_cast<T>(2);

    std::cout << "random + filled:\n" << sum;
    std::cout << "random - filled:\n" << diff;
    std::cout << "random * filled:\n" << prod;
    std::cout << "2 * random:\n" << scaled_left;
    std::cout << "random * 2:\n" << scaled_right;
    std::cout << "random / 2:\n" << divided;

    std::cout << "random == random: " << (random == random) << '\n';
    std::cout << "random != filled: " << (random != filled) << '\n';

    std::cout << "Trace of random: " << random.trace() << '\n';

    std::cout << '\n';
}

template <typename T>
void demonstrate_complex_matrix(const std::string& title) {
    std::cout << title << "\n----------------------------------\n";

    Matrix<T> empty;
    std::cout << "Empty matrix size: " << empty.rows() << "x" << empty.cols() << '\n';

    Matrix<T> filled(3, 3, T{ 1, 1 });
    std::cout << "Filled matrix:\n" << filled;

    Matrix<T> random(3, 3, T{ -1, -1 }, T{ 1, 1 });
    std::cout << "Random matrix:\n" << random;

    Matrix<T> copy = random;
    Matrix<T> moved = std::move(copy);

    Matrix<T> assigned(2, 2, T{});
    assigned = random;

    Matrix<T> moved_assigned(2, 2, T{});
    moved_assigned = std::move(assigned);

    random(0, 0) = T{ 42, 42 };
    std::cout << "After random(0,0) = 42+42i:\n" << random;
    std::cout << "random(1,1) = " << random(1, 1) << '\n';

    Matrix<T> tmp(3, 3, T{ 1, 1 });
    tmp += random;
    tmp -= random;
    tmp *= T{ 2, 0 };
    tmp /= T{ 2, 0 };
    std::cout << "After compound operations:\n" << tmp;

    Matrix<T> sum = random + filled;
    Matrix<T> diff = random - filled;
    Matrix<T> prod = random * filled;
    Matrix<T> scaled_left = T{ 2, 0 } * random;
    Matrix<T> scaled_right = random * T{ 2, 0 };
    Matrix<T> divided = random / T{ 2, 0 };

    std::cout << "random + filled:\n" << sum;
    std::cout << "random * filled:\n" << prod;
    std::cout << "2 * random:\n" << scaled_left;
    std::cout << "random / 2:\n" << divided;

    std::cout << "random == random: " << (random == random) << '\n';
    std::cout << "random != filled: " << (random != filled) << '\n';

    std::cout << "Trace of random: " << random.trace() << '\n';

    std::cout << '\n';
}

template <typename T>
void demonstrate_decomposition() {
    std::cout << "Matrix Decompozition\n";

    Matrix<T> a(3, 3, T{});
    a(0, 0) = static_cast<T>(2);
    a(0, 1) = static_cast<T>(1);
    a(0, 2) = static_cast<T>(1);
    a(1, 0) = static_cast<T>(4);
    a(1, 1) = static_cast<T>(3);
    a(1, 2) = static_cast<T>(3);
    a(2, 0) = static_cast<T>(8);
    a(2, 1) = static_cast<T>(7);
    a(2, 2) = static_cast<T>(9);

    std::cout << "Matrix A:\n" << a;

    const auto lu = decompose(a);
    std::cout << "Lower\n" << lu.lower;
    std::cout << "Upper\n" << lu.upper;

    std::cout << lu.lower * lu.upper;

}


int main() {
    demonstrate_matrix<int>("int");
    demonstrate_matrix<float>("float");
    demonstrate_matrix<double>("double");

    demonstrate_complex_matrix<std::complex<float>>("std::complex<float>");
    demonstrate_complex_matrix<std::complex<double>>("std::complex<double>");

    demonstrate_decomposition<double>();
    demonstrate_decomposition<float>();

    return 0;
}
