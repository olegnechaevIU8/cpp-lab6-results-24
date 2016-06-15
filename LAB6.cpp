#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <typeinfo>
using namespace std;

class Rational
{
	int num, den;

public:

	Rational()
	{
		num = 0;
		den = 0;
	}

	Rational(int num_, int den_) //Rationalnie chisla
	{
		this->num = num_;
		this->den = den_;
	}

	void print()
	{
		cout << num << "/" << den << " ";
	}

	~Rational() {}

	//Peregruzka operatorov >> i <<
	friend ostream& operator<< (ostream&, Rational const&);
	friend istream& operator>> (istream&, Rational&);

	//Peregruzka operatorov + i -
	friend Rational const operator+ (Rational const&, Rational const&);
	friend Rational const operator* (Rational const&, Rational const&);

};


int Euclid(int a, int b)
{
	while (b) b ^= a ^= b ^= a %= b;
	return a;
}

Rational const operator+ (Rational const& L, Rational const& R)
{
	int a, b, c;
	a = L.num * R.den + R.num * L.den;
	b = L.den * R.den;
	c = Euclid(a, b);
	return Rational(a / c, b / c);
}

Rational const operator* (Rational const& L, Rational const& R)
{
	int a, b, c;
	a = L.num * R.num;
	b = L.den * R.den;
	c = Euclid(a, b);
	return Rational(a / c, b / c);
}

ostream& operator<<(ostream& ost, Rational const& rat)
{
	return ost << "[" << rat.num << '/' << rat.den << "]";
}

istream& operator>>(istream& ist, Rational& r)
{
	string rational;
	ist >> rational;
	auto d = find(rational.begin(), rational.end(), '/');
	if (rational.front() != '[' || rational.back() != ']' || d == rational.end())
		throw runtime_error("Bad input");
	*d = ' ';
	string tmp(rational.begin() + 1, --(rational.end()));
	stringstream ss;
	ss << tmp;
	ss >> r.num >> r.den;
	return ist;
}

//---------------------------------------------------------------------------//

template<class X>
class matrix {
	X** T;
	int M;
	int N;
public:
	matrix(int m, int n) : T(NULL), M(m), N(n)
	{
		T = new X*[M];
		for (int i = 0; i < M; i++)
		{
			T[i] = new X[N];
			for (int j = 0; j < N; j++)
				T[i][j] = X();
		}
	}
	X& operator () (int i, int j)
	{
		return T[i][j];
	}

	template<typename X>
	friend ostream&  operator <<(ostream& os, const matrix<X>&a);
	template <typename X>
	matrix<X>  operator*(const matrix<X>& a) const
	{
		matrix<X> U(M, a.N);

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < a.N; j++)
			{
				for (int t = 0; t < N; t++)
				{
					U(i, j) = U(i, j) + T[i][t] * a.T[t][j];
				}
			}
		}
		return U;
	}

	void print() //
	{
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
				cout << T[i][j];
			cout << endl;
		}
		cout << endl << endl;
	}
	matrix<X> & operator ~() //TRANSPONIROVANIE
	{
		matrix<X> U(N, M);

		for (int i = 0; i<N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				U(i, j) = T[j][i];
			}
		}
		return U;
	}
	matrix<X>  operator *(X a) // YMNOZHENIE
	{

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
				T[i][j] = T[i][j] * a;
		}
		matrix U(M, N);

		for (int i = 0; i<M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				U(i, j) = T[i][j];
			}
		}
		return U;


	}

	matrix<X>  operator |(matrix<X>& a) //Konkatenacia
	{

		matrix U(M, N + a.N);

		for (int i = 0; i<M; i++)
		{
			for (int j = 0; j < N + a.N; j++)
			{
				if (j < N)
				{
					U(i, j) = T[i][j];
				}
				else
				{
					U(i, j) = a.T[i][j - N];
				}
			}
		}
		return U;
	}

	~matrix(){}

	class iterator {
		matrix* Ma;
		size_t c;
	public:
		iterator(matrix* a, size_t b) : Ma(a), c(b) {};

		iterator& operator++() {
			c++;
			return *this;
		}

		X& operator*() {
			return Ma->e[c];
		}

		X* operator->() {
			return &(Ma->e[c]);
		}

		bool operator !=(const iterator& it) {
			return !(*this == it);
		}

		bool operator ==(const iterator& it) {
			return (c == it.c);
		}
	};
	iterator begin() {
		return iterator(this, 0);
	}

	iterator end() {
		return iterator(this, N*M);
	}

};

template <typename X>
ostream&  operator <<(ostream& os, const matrix<X> &a)
{
	for (int i = 0; i < a.M; i++)
	{
		for (int j = 0; j <a.N; j++)
			os << a.T[i][j] << " ";
		cout << endl;
	}
	cout << endl << endl;
	return os;
}

template<template <class> class cont, typename X, typename F>
cont<F> caster(cont<X> a) {
	cont<X> b(m.n, m.m);
	for (int i = 0; i < n*m; i++) b.e[i] = static_cast<X>(a.e[i]);
	return b;
}

template <class X>
void function(matrix<X> G, X n)
{


	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			G(i, j) = n;

		}
	}

	cout << "Nachalnaya matrica: \n";
	G.print();

	cout << "Umnozhenie na chislo = 5: \n";
	G = G * n;
	G.print();

	cout << "Transponirovanie: \n";
	G = ~G;
	G.print();

	cout << "Konkatenacia: \n";
	G = G | G;
	G.print();
}


int main()
{

	cout << "\n Test2 - matrix \n";
	matrix<Rational> M(5, 6);
	matrix<int> A(5, 6);
	function(M, Rational(1, 2));
	cout << "--------------------------------" << endl;
	function(A, 3);
	system("pause");
	return 0;
}
