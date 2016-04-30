#ifndef SBX_COMPLEX_H
#define SBX_COMPLEX_H

#include <iostream>

namespace sbx {

	////////////////////////////////////////////////////////////////////////////////////
	//
	// sbx class Complex
	//
	// Complex numbers (real,imag)
	//     including casts from real int or double
	// Operations: 
	//     c=-a, a++(=++a), a--(=--a),  c=a+b, c=a-b, c=a*b, c=a/b
	//     ==, != istream, ostream, norm, complex conjugate c=!a
	// Imaginary number i:
	//     IMAG (0,1)
	//
	// Example code:
	//     Complex a(2,3), b(2,5), c;
	//     c = a + b;
	//     a++;
	//     std::cout << a << " " << c << std::endl;
	//
	//     Generates output: (3,3) (4,8)   
	//

	class Complex{

		double real, imag;

	public:
		// constructors
		Complex() : real(0.), imag(0.) {}
		Complex(double _real, double _imag)
		    { real = _real, imag = _imag; }
		//     for casts
		Complex(int _real)
		    { real = (double)_real, imag = 0.; }
		Complex(double _real)
		    { real = _real, imag = 0.; }

		// operations
		//     unary
		Complex operator-();  // c = -a

		// real number in-/decrement
		//     pre- and postfix operators give equal results 
		Complex operator++(); // prefix ++a
		Complex operator--();
		Complex operator++(int); // postfix a++ 
		Complex operator--(int);

		//     binary
		//         selected option of friend fuctions 
		//         to allow casts with non-complex number in first argument, 
		//         e.g. c = 1 + a
		friend Complex operator+(const Complex&, const Complex&);
		friend Complex operator-(const Complex&, const Complex&);
		friend Complex operator*(const Complex&, const Complex&);
		friend Complex operator/(const Complex&, const Complex&);

		Complex operator+=(const Complex&);
		Complex operator-=(const Complex&);
		Complex operator*=(const Complex&);
		Complex operator/=(const Complex&);

		//     complex conjugate
		Complex operator!(void);

		//     comparisons
		bool operator==(const Complex&);
		bool operator!=(const Complex&);

		//     streams
		friend std::ostream& operator<<(std::ostream&, const Complex&);
		friend std::istream& operator>>(std::istream&, Complex&);

		// getter
		friend double real(const Complex& a) 
		    { return a.real; }
		friend double imag(const Complex& a) 
		    { return a.imag; }
		friend double norm(const Complex& a) 
		    { return a.real * a.real + a.imag * a.imag; }

	}; // class Complex

	////////////////////////////////////////////////////////////////////////////////////
	// predefined number i

	const Complex IMAG(0., 1.);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// negation (deep)
	//     c = -a : c = a.operator-()

	inline Complex Complex::operator-()
	{
		Complex c;
		c.real = -real;
		c.imag = -imag;
		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// addition (shallow) 
	//     a = a + b, a+=b : a = a.operator+=( b )

	inline Complex Complex::operator+=(const Complex& b)
	{
		real += b.real;
		imag += b.imag;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// subtraction (shallow)
	//     a = a - b, a-=b : a = a.operator-=( b )

	inline Complex Complex::operator-=(const Complex& b)
	{
		real -= b.real;
		imag -= b.imag;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// multiplication (shallow)
	//     a = a * b, a*=b : a = a.operator*=( b )

	inline Complex Complex::operator*=(const Complex& b)
	{
		double temp0 = imag * b.real, temp1 = b.imag * real;
		real = (real - imag) * (b.real + b.imag) + temp0 - temp1;
		imag = temp0 + temp1;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// division (shallow) 
	//     a = a / b, a/=b : a = a.operator/=( b )

	inline Complex Complex::operator/=(const Complex& b)
	{
		double denominator = b.real * b.real + b.imag * b.imag;
		double temp = (real * b.real + imag * b.imag) / denominator;
	    imag = (imag * b.real - real * b.imag) / denominator;
		real = temp;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// praefix real number increment 
	//     c = a + (1,0)
	//     equal to postfix real number increment 
	//     ++a : c = a.operator++()

	inline Complex Complex::operator++()
	{
		++real;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// praefix real number decrement
	//     c = a - (1,0)
	//     equal to postfix real number decrement 
	//     --a : c = a.operator--()

	inline Complex Complex::operator--()
	{
		--real;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// postfix real number increment 
	//     c = a + (1,0) 
	//     equal to prefix real number increment 
	//     a++ : c = a.operator++(1)

	inline Complex Complex::operator++(int spam)
	{
		real++;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// postfix real number decrement 
	//     c = a - (1,0), while c = a +  1 
	//     equal to prefix real number decrement 
	//     a-- : c = a.operator--(1)

	inline Complex Complex::operator--(int spam)
	{
		real--;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// complex conjugate 
	//     c = real(a) - i * imag(b) 

	inline Complex Complex::operator!(void)
	{
		Complex c(real, -imag);

		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// equality
	//     a == b : a.operator==(b)

	inline bool Complex::operator==(const Complex& b)
	{
		if (real == b.real && imag == b.imag)
			return true;
		else
			return false;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// inequality
	//     a != b : a.operator!=(b)

	inline bool Complex::operator!=(const Complex& b)
	{
		return !(*this == b);

	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// addition (deep)
	//     c = a + b : c = operator+( a, b )

	inline Complex operator+(const Complex& a, const Complex& b)
	{
		Complex c;
		c.real = a.real + b.real;
		c.imag = a.imag + b.imag;
		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// subtraction (deep)
	//     c = a - b : c = operator-( a, b )

	inline Complex operator-(const Complex& a, const Complex& b)
	{
		Complex c;
		c.real = a.real - b.real;
		c.imag = a.imag - b.imag;
		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// multiplication (deep)
	//     c = a * b : c = a.operator*( b )

	inline Complex operator*(const Complex& a, const Complex& b)
	{
		double temp0 = a.imag * b.real, temp1 = b.imag *  a.real;
		Complex c((a.real - a.imag) * (b.real + b.imag) +
			temp0 - temp1, temp0 + temp1);
		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// division (deep)
	//     c = a / b : c = a.operator/( b )

	inline Complex operator/(const Complex& a, const Complex& b)
	{
		double denominator = b.real * b.real + b.imag * b.imag;
		Complex c(
			(a.real * b.real + a.imag * b.imag) / denominator,
			(a.imag * b.real - a.real * b.imag) / denominator);
		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// output stream
	//     std::cout << "(" << real(a) << "," << imag(b) << ")" :  operator<<(std::cout, a)
	//     std::cout << a << b : operator<<(operator<<(std::cout, a), b)

	inline std::ostream& operator<<(std::ostream& stream, const Complex& b)
	{
		return stream << "(" << b.real << "," << b.imag << ")";
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	//  input stream 
	//      operator>>(std::cin, a)
	//          Real: ....
	//          Imag: ....

	inline std::istream& operator>>(std::istream& stream, Complex& a)
	{
		std::cout << "Real: ";
		stream >> a.real;
		std::cout << "Imag: ";
		stream >> a.imag;
		return stream;
	}

}  // namespace sbx

#endif
