#pragma once

#include <iostream>
#include <complex>
#include <cstddef>
#include <stdexcept>
#include <random>


template <typename T>
class Matrix {
private:
	size_t rows_ = 0;
	size_t cols_ = 0;
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

	size_t rows() const noexcept { return rows_; }
	size_t cols() const noexcept { return cols_; }

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
void Matrix<T>::deallocate_memory() {
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
	return this*;
}

int random_value(int low, int high) {
	static std::mt19937 gen{std::random_device{}()};
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

