#pragma once

#include <complex>
#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>

constexpr double precision = 1e-9;


int random_value(int low, int high) {
	static std::mt19937 gen{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(low, high);
	return dist(gen);
}

float random_value(float low, float high) {
	static std::mt19937 gen{ std::random_device{}() };
	std::uniform_real_distribution<float> dist(low, high);
	return dist(gen);
}

double random_value(double low, double high) {
	static std::mt19937 gen{ std::random_device{}() };
	std::uniform_real_distribution<double> dist(low, high);
	return dist(gen);
}

std::complex<float> random_value(std::complex<float> low, std::complex<float> high) {
	static std::mt19937 gen{ std::random_device{}() };
	std::uniform_real_distribution<float> real_dist(low.real(), high.real());
	std::uniform_real_distribution<float> imag_dist(low.imag(), high.imag());
	return std::complex<float>(real_dist(gen), imag_dist(gen));
}

std::complex<double> random_value(std::complex<double> low, std::complex<double> high) {
	static std::mt19937 gen{ std::random_device{}() };
	std::uniform_real_distribution<double> real_dist(low.real(), high.real());
	std::uniform_real_distribution<double> imag_dist(low.imag(), high.imag());
	return std::complex<double>(real_dist(gen), imag_dist(gen));
}

template <typename T>
class Matrix {
private:
	std::size_t rows_ = 0;
	std::size_t cols_ = 0;
	T* data_ = nullptr;

	void allocate_memory(std::size_t rows, std::size_t cols);
	void deallocate_memory() noexcept;
	void check_index(std::size_t row, std::size_t col) const;
	void check_same_size(const Matrix& other) const;

public:
	Matrix() = default;

	Matrix(std::size_t rows, std::size_t cols, const T& fill_value);
	
	Matrix(std::size_t rows, std::size_t cols, const T& low, const T& high);

	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	Matrix& operator=(const Matrix& other);
	Matrix& operator=(Matrix&& other) noexcept;
	~Matrix();

	std::size_t rows() const noexcept { return rows_; }
	std::size_t cols() const noexcept { return cols_; }

	T& operator()(std::size_t row, std::size_t col);
	const T& operator()(std::size_t row, std::size_t col) const;

	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);
	Matrix& operator*=(const T& scalar);
	Matrix& operator/=(const T& scalar);

	T trace() const;

	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;

	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);
};


template <typename T>
void Matrix<T>::allocate_memory(std::size_t rows, std::size_t cols) {
	rows_ = rows;
	cols_ = cols;
	data_ = new T[rows * cols]();
}

template <typename T>
void Matrix<T>::deallocate_memory() noexcept {
	delete[] data_;
	data_ = nullptr;
	rows_ = 0;
	cols_ = 0;
}

template <typename T>
Matrix<T>::~Matrix() {
	deallocate_memory();
}

template <typename T>
Matrix<T>::Matrix(const Matrix& other) {
	if (other.data_ != nullptr) {
		allocate_memory(other.rows_, other.cols_);
		for (std::size_t i = 0; i < rows_ * cols_; ++i) {
			data_[i] = other.data_[i];
		}
	}
}

template <typename T>
Matrix<T>::Matrix(Matrix&& other) noexcept : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {
	other.rows_ = 0;
	other.cols_ = 0;
	other.data_ = nullptr;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) {
	if (this != &other) {
		Matrix<T> temp(other);
		*this = std::move(temp);
	}
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other) noexcept {
	if (this != &other) {
		deallocate_memory();
		rows_ = other.rows_;
		cols_ = other.cols_;
		data_ = other.data_;
		other.rows_ = 0;
		other.cols_ = 0;
		other.data_ = nullptr;
	}
	return *this;
}

template <typename T>
void Matrix<T>::check_index(std::size_t row, std::size_t col) const {
	if (row >= rows_ || col >= cols_) {
		throw std::out_of_range("Matrix index out of range");
	}
}

template <typename T>
T& Matrix<T>::operator()(std::size_t row, std::size_t col) {
	check_index(row, col);
	return data_[row * cols_ + col];
}

template <typename T>
const T& Matrix<T>::operator()(std::size_t row, std::size_t col) const {
	check_index(row, col);
	return data_[row * cols_ + col];
}

template <typename T>
void Matrix<T>::check_same_size(const Matrix& other) const {
	if (rows_ != other.rows_ || cols_ != other.cols_) {
		throw std::invalid_argument("Matrix dimensions do not match");
	}
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) {
	check_same_size(other);
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		data_[i] += other.data_[i];
	}
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) {
	check_same_size(other);
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		data_[i] -= other.data_[i];
	}
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& scalar) {
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		data_[i] *= scalar;
	}
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const T& scalar) {
	if (scalar == T{}) {
		throw std::invalid_argument("Division by zero");
	}
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		data_[i] /= scalar;
	}
	return *this;
}

template <typename T>
Matrix<T> operator+(Matrix<T> left, const Matrix<T>& right) {
	left += right;
	return left;
}

template <typename T>
Matrix<T> operator-(Matrix<T> left, const Matrix<T>& right) {
	left -= right;
	return left;
}

template <typename T>
Matrix<T> operator*(Matrix<T> left, const Matrix<T>& right) {
	if (left.cols() != right.rows()) {
		throw std::invalid_argument("Matrix cannot be multiplied");
	}

	Matrix<T> result(left.rows(), right.cols(), T{});

	for (std::size_t i = 0; i < left.rows(); ++i) {
		for (std::size_t k = 0; k < left.cols(); ++k) {
			const T& tmp = left(i, k);
			for (std::size_t j = 0; j < right.cols(); ++j) {
				result(i, j) += tmp * right(k, j);
			}
		}
	}
	return result;
}

template <typename T>
Matrix<T> operator*(Matrix<T> left, const T& scalar) {
	left *= scalar;
	return left;
}

template <typename T>
Matrix<T> operator*(const T& scalar, Matrix<T> left) {
	left *= scalar;
	return left;
}

template <typename T>
Matrix<T> operator/(Matrix<T> left, const T& scalar) {
	left /= scalar;
	return left;
}

inline bool almost_equal(int a, int b, double /*epsilon*/) {
	return a == b;
}

inline bool almost_equal(float a, float b, double epsilon) {
	const double difference = static_cast<double>(a) - static_cast<double>(b);
	return difference < epsilon && difference > -epsilon;
}

inline bool almost_equal(double a, double b, double epsilon) {
	const double difference = a - b;
	return difference < epsilon && difference > -epsilon;
}

inline bool almost_equal(std::complex<float> a, std::complex<float> b, double epsilon) {
	return almost_equal(a.real(), b.real(), epsilon) && almost_equal(a.imag(), b.imag(), epsilon);
}

inline bool almost_equal(std::complex<double> a, std::complex<double> b, double epsilon) {
	return almost_equal(a.real(), b.real(), epsilon) && almost_equal(a.imag(), b.imag(), epsilon);
}

template <typename T>
bool Matrix<T>::operator==(const Matrix& other) const {
	if (rows_ != other.rows_ || cols_ != other.cols_) {
		return false;
	}
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		if (!almost_equal(data_[i], other.data_[i], precision)) {
			return false;
		}
	}
	return true;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix& other) const {
	return !(*this == other);
}

template <typename T>
T Matrix<T>::trace() const {
	if (cols_ != rows_) {
		throw std::invalid_argument("The matrix has no trace");
	}
	
	T trace{};
	for (std::size_t i = 0; i < rows_; ++i) {
		trace += (*this)(i, i);
	}
	return trace;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& m) {
	for (std::size_t i = 0; i < m.rows(); ++i) {
		for (std::size_t j = 0; j < m.cols(); ++j) {
			os << m(i, j);
			if (j + 1 < m.cols()) {
				os << ' ';
			}
		}
		os << '\n';
	}
	return os;
}

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, const T& fill_value) {
	if (rows == 0 || cols == 0) {
		throw std::invalid_argument("The matrix size must be positive");
	}
	allocate_memory(rows, cols);
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		data_[i] = fill_value;
	}
}

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, const T& low, const T& high) {
	if (rows == 0 || cols == 0) {
		throw std::invalid_argument("The matrix size must be positive");
	}
	allocate_memory(rows, cols);
	for (std::size_t i = 0; i < rows_ * cols_; ++i) {
		data_[i] = random_value(low, high);
	}
}

