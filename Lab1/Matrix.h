#pragma once

#include<iostream>
#include<string>


template <typename T>
class Matrix {
private:
	size_t rows_ = 0;
	size_t cols_ = 0;
	T& data_ = nullptr;

	void allocate_memory(std::size_t rows, std::size_t cols);
	void deallocate_memory() noexept;

public:
	Matrix() = default;

	Matrix(std::size_t rows, std::size_t cols, const T& fill_value);
	
	Matrix(std::size_t rows, std::size_t cols, const T& low, const T& high);

	Matrix(const Matrix& other);
	Matrix(Matrix&& other);
	Matrix& operator=(const Matrix& other);
	Matrix& operator=(Matrix&& other) noexcept;
	~Matrix();

	size_t rows() const noexcept {return rows_};
	size_t cols() const noexcept {return cols_}

	T& operator()(std::size_t row, std::size_t col);
	const T& operator()(std::size_t row, size_t col) const;

	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);
	Matrix& operator*=(const T& scalar);
	Matrix& operator/=(const T& scalar);

	T trace() const:

	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;

	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);
};