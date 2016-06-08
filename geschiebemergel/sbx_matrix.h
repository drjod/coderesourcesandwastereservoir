#include <iostream>
#include <stdlib.h>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Matrix and Vector class based on matrix.h by Stroustrup
// !!! allows fast matrix vector operations
// restricted to double entries, no rows, added matrix vector multiplication
//
//

#ifndef SBX_MATRIX_H_
#define SBX_MATRIX_H_

namespace sbx {

	struct Matrix_error {
		std::string name;
		Matrix_error( const char* q ) :name(q) { }
		Matrix_error( std::string n ) :name(n) { }
	};

	//-----------------------------------------------------------------------------

	inline void matrix_error( const char* p )
	{
		throw Matrix_error(p);
	}

	//-----------------------------------------------------------------------------

	// function objects for various apply() operations:

	struct Assign {
		void operator()( double& a, const double& c ) { a = c; }
	};

	struct Add_assign {
		void operator()( double& a, const double& c ) { a += c; }
	};
	struct Mul_assign {
		void operator()( double& a, const double& c ) { a *= c; }
	};
	struct Minus_assign {
		void operator()( double& a, const double& c ) { a -= c; }
	};
	struct Div_assign {
		void operator()( double& a, const double& c ) { a /= c; }
	};

	struct Unary_minus {
		double operator()( double& a ) { return -a; }
	};

	//-----------------------------------------------------------------------------

	class Matrix_base {
	protected:
		double* elem;
		SIZE_T sz;
		mutable bool owns;
		mutable bool xfer;

	public:
		Matrix_base() : owns(true), xfer(false)  { elem = NULL; sz = -1; }
		Matrix_base( SIZE_T n ) : elem(new double[n]()), sz(n), owns(true), xfer(false) {}
		Matrix_base( SIZE_T n, double* p ) : elem(p), sz(n), owns(false), xfer(false) {}

	    ~Matrix_base() { if (owns) delete [] elem; }

	    void allocate_base( SIZE_T _sz ) { sz = _sz; elem = new double[_sz]; } // for use with Matrix_base()

	    double* data()       { return elem; }
		const double* data() const { return elem; }
		SIZE_T size() const { return sz; }

		void copy_elements( const Matrix_base& a )
		{
		    if (sz!=a.sz) matrix_error("copy_elements()");
			   for (SIZE_T i=0; i<sz; ++i) elem[i] = a.elem[i];
		}

		void base_assign( const Matrix_base& a ) { copy_elements(a); }

		void base_copy( const Matrix_base& a )
		{
		   if (a.xfer) {     // shallow copy
			   elem = a.elem;
			   a.xfer = false;    // note: modifies source
			   a.owns = false;
		   }
		   else {   // deep copy
			   elem = new double[a.sz];
			   copy_elements(a);
		   }
		   owns = true;
		   xfer = false;
		}

		void base_xfer( Matrix_base& x )
		{ // to get the elements of a local matrix out of a function without copying
 			   if (owns==false) matrix_error("cannot xfer() non-owner");
			   owns = false;     // now the elements are safe from deletion by original owner
			   x.xfer = true;    // target asserts temporary ownership
			   x.owns = true;
	    }

		template<class F> void base_apply( F f ) { for (SIZE_T i = 0; i<size(); ++i) f(elem[i]); }
		template<class F> void base_apply( F f, const double& c ) { for (SIZE_T i = 0; i<size(); ++i) f(elem[i],c); }
		template<class F> void base_apply( F f, const double* data ) { for (SIZE_T i = 0; i<size(); ++i) f(elem[i],data[i]); } // data from second matrix (vector)

		double base_norm0( void ) const
		{
			   double value = 0;
			   for (SIZE_T i = 0; i<sz; ++i)
				   value = std::max( value, std::fabs(elem[i]) );
			   return value;
		}

		std::ostream& base_outstream( std::ostream& stream, SIZE_T dim2 = 0 )
		{
			   for (SIZE_T i = 0; i<sz; ++i)
			   {
				   if (dim2 > 0 && i%dim2 == 0 )
					   stream << std::endl; // next row in matrix
				   stream << " " << i << ": " << elem[i] << ",";
			   }
			   return stream;
		}

		void base_multiply( const double* matrix_data, const double* vector_data, SIZE_T dim2 )
		{
			   int offset=0; // runningRow * numberOfColumnsInMatrix
			   for (SIZE_T i=0; i<sz; ++i)
			   {
				   elem[i] = 0;
				   for (SIZE_T k = 0;k < dim2; k++ )
					   elem[i] += matrix_data[offset + k] * vector_data[k];
				   offset += sz;
			   }
		}


		friend std::ostream& operator<<( std::ostream&, const Matrix_base& );
		friend std::istream& operator>>( std::istream&, Matrix_base& );

	   private:
		   void operator=(const Matrix_base&);    // no ordinary copy of bases
		   Matrix_base(const Matrix_base&);         // used anywhere????
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------

	class Matrix : public Matrix_base {
		SIZE_T d1;
		SIZE_T d2;

	private:
		Matrix(SIZE_T n1, SIZE_T n2, double* p) : Matrix_base(n1*n2,p), d1(n1), d2(n2) { }// for use by Row, xfer:


	public:
		Matrix() : Matrix_base() {}

		Matrix(SIZE_T n1, SIZE_T n2) : Matrix_base(n1*n2), d1(n1), d2(n2) { }
		Matrix(const Matrix& a) : Matrix_base(a.size(),0), d1(a.d1), d2(a.d2) { this->base_copy(a); }		// copy constructor: let the base do the copy

		~Matrix() { }

		void allocate(SIZE_T n1, SIZE_T n2) { // for use with Matrix()
			this->allocate_base(n1*n2);
			d1 = n1;
			d2 = n2;
		}

		template<int n1, int n2> Matrix(const double (&a)[n1][n2]) : Matrix_base(n1*n2), d1(n1), d2(n2)
		{
			for (SIZE_T i = 0; i<n1; ++i)
				for (SIZE_T j = 0; j<n2; ++j) this->elem[i*n2+j]=a[i][j];
		}

		template<class F> Matrix(const Matrix& a, F f) : Matrix_base(a.size()), d1(a.d1), d2(a.d2)
		{
			for (SIZE_T i = 0; i<this->sz; ++i) this->elem[i] = f(a.elem[i]);
		}

		template<class F, class Arg> Matrix(const Matrix& a, F f, const Arg& t1) : Matrix_base(a.size()), d1(a.d1), d2(a.d2)
		{
			for (SIZE_T i = 0; i<this->sz; ++i) this->elem[i] = f(a.elem[i],t1);
		}

		Matrix& operator=(const Matrix& a)
		{
			if (d1!=a.d1 || d2!=a.d2) matrix_error("length error in 2D =");
			this->base_assign(a);
			return *this;
		}

		SIZE_T dim1() const { return d1; }    // number of elements in a row
		SIZE_T dim2() const { return d2; }    // number of elements in a column

		Matrix xfer()    // make Matrix to move elements out of a scope
		{
			Matrix x(dim1(),dim2(),this->data()); // make a descriptor
			this->base_xfer(x);            // transfer (temporary) ownership to x
			return x;
		}
		Matrix xfer(Matrix& a) { return a.xfer(); }

		void range_check(SIZE_T n1, SIZE_T n2) const
		{
			if (n1<0 || d1<=n1) matrix_error("2D range error: dimension 1");
			if (n2<0 || d2<=n2) matrix_error("2D range error: dimension 2");
		}

		// subscripting:
		double& operator()(SIZE_T n1, SIZE_T n2)       { range_check(n1,n2); return this->elem[n1*d2+n2]; }
		const double& operator()(SIZE_T n1, SIZE_T n2) const { range_check(n1,n2); return this->elem[n1*d2+n2]; }


		template<class F> Matrix& apply(F f)            { this->base_apply(f);   return *this; }
		template<class F> Matrix& apply(F f,const double& c) { this->base_apply(f,c); return *this; }

		Matrix& operator=(const double& c)  { this->base_apply(Assign(),c);       return *this; }

		Matrix& operator*=(const double& c) { this->base_apply(Mul_assign(),c);   return *this; }
		Matrix& operator/=(const double& c) { this->base_apply(Div_assign(),c);   return *this; }
		Matrix& operator+=(const double& c) { this->base_apply(Add_assign(),c);   return *this; }
		Matrix& operator-=(const double& c) { this->base_apply(Minus_assign(),c); return *this; }

		std::ostream& outstream( std::ostream& stream) { return this->base_outstream(stream, d2); }

	};

	//---------------------------------------------------------------------------------------------------


	class Vector : public Matrix_base {
		SIZE_T d1;

	public:

	   Vector() : Matrix_base() {}

	   Vector(SIZE_T n1) : Matrix_base(n1), d1(n1) { }



	   Vector(const Vector& a) : Matrix_base(a.size(),0), d1(a.d1) { this->base_copy(a); } // copy constructor: let the base do the copy

	   template<int n> Vector(const double (&a)[n]) : Matrix_base(n), d1(n) { for (SIZE_T i = 0; i<n; ++i) this->elem[i]=a[i]; }

	   Vector(const double* p, SIZE_T n) : Matrix_base(n), d1(n) { for (SIZE_T i = 0; i<n; ++i) this->elem[i]=p[i]; }

	   void allocate(SIZE_T n1) { // foe use with Vector()
		   this->allocate_base(n1);
			d1 = n1;
		}


	   template<class F> Vector(const Vector& a, F f) : Matrix_base(a.size()), d1(a.d1)
		   // construct a new Matrix with element's that are functions of a's elements:
		   // does not modify a unless f has been specifically programmed to modify its argument
		   // T f(const T&) would be a typical type for f
	   {
		   for (SIZE_T i = 0; i<this->sz; ++i) this->elem[i] = f(a.elem[i]);
	   }

	   template<class F, class Arg> Vector(const Vector& a, F f, const Arg& t1) : Matrix_base(a.size()), d1(a.d1)
		   // construct a new Matrix with element's that are functions of a's elements:
		   // does not modify a unless f has been specifically programmed to modify its argument
		   // T f(const T&, const Arg&) would be a typical type for f
	   {
		   for (SIZE_T i = 0; i<this->sz; ++i) this->elem[i] = f(a.elem[i],t1);
	   }

	   Vector& operator=(const Vector& a)
		   // copy assignment: let the base do the copy
	   {
		   // std::cerr << "copy assignment (" << this->size() << ',' << a.size()<< ")\n";
		   if (d1!=a.d1) matrix_error("length error in 1D=");
		   this->base_assign(a);
		   return *this;
	   }

	   ~Vector() { }

	   SIZE_T dim1() const { return d1; }    // number of elements in a row

	   Vector xfer()    // make an Matrix to move elements out of a scope
	   {
		   Vector x(this->data(), dim1()); // make a descriptor   ????? swapped arguments
		   this->base_xfer(x);                  // transfer (temporary) ownership to x
		   return x;
	   }
	   Vector xfer(Vector a) { return a.xfer(); }

	   void range_check(SIZE_T n1) const
	   {
		   // std::cerr << "range check: (" << d1 << "): " << n1 << "\n";
		   if (n1<0 || d1<=n1) matrix_error("1D range error: dimension 1");
	   }

	   // subscripting:
	   double& operator()(SIZE_T n1)       { range_check(n1); return this->elem[n1]; }
	   const double& operator()(SIZE_T n1) const { range_check(n1); return this->elem[n1]; }

	   // slicing (the same as subscripting for 1D matrixs):
	   double& operator[](SIZE_T n)       { return row(n); }
	   const double& operator[](SIZE_T n) const { return row(n); }

	   double& row(SIZE_T n)       { range_check(n); return this->elem[n]; }
	   const double& row(SIZE_T n) const { range_check(n); return this->elem[n]; }

	   // element-wise operations:
	   template<class F> Vector& apply(F f) { this->base_apply(f); return *this; }
	   template<class F> Vector& apply(F f,const double& c) { this->base_apply(f,c); return *this; }

	   Vector& operator=(const double& c)  { this->base_apply(Assign(),c);       return *this; }

	   Vector& operator*=(const double& c) { this->base_apply(Mul_assign(),c);   return *this; }
	   Vector& operator/=(const double& c) { this->base_apply(Div_assign(),c);   return *this; }
	   Vector& operator+=(const double& c) { this->base_apply(Add_assign(),c);   return *this; }
	   Vector& operator-=(const double& c) { this->base_apply(Minus_assign(),c); return *this; }

	   Vector& operator+=(const Vector& w) { this->base_apply(Add_assign(),w.data() );  return *this; }
	   Vector& operator-=(const Vector& w) { this->base_apply(Minus_assign(),w.data() );  return *this; }

	   Vector operator-() { return xfer(Vector(*this,Unary_minus())); }

	   double norm0(void) const { return this->base_norm0(); }

	   std::ostream& outstream( std::ostream& stream) { return this->base_outstream(stream); }


	   void multiply(const Matrix& m, const Vector& v) { this->base_multiply(m.data(),v.data(), m.dim2()); }

	   friend std::ostream& operator<<( std::ostream&, const Vector& );
	   friend std::istream& operator>>( std::istream&, Vector& );

	};


	double dot_product(const Vector&a , const Vector& b)
	{
		if (a.size() != b.size()) matrix_error("sizes wrong for dot product");
		double sum = 0;
		for (SIZE_T i = 0; i<a.size(); ++i) sum += a[i]*b[i];
		return sum;
	}

	Vector operator*( const Vector& v, const double& c ) { Vector r(v); return r*=c; }
	Vector operator/( const Vector& v, const double& c ) { Vector r(v); return r/=c; }
	Vector operator+( const Vector& v, const double& c ) { Vector r(v); return r+=c; }
	Vector operator-( const Vector& v, const double& c ) { Vector r(v); return r-=c; }

	Vector operator+( const Vector& v, const Vector& w ) { Vector r(v); return r+=w; }
	Vector operator-( const Vector& v, const Vector& w ) { Vector r(v); return r-=w; }

	void multiply( const Matrix& m, const Vector& v, Vector& w ) { w.multiply(m, v); }

	double norm0( const Vector& v ) { return v.norm0(); }

	std::ostream& operator<<( std::ostream& stream, Vector& v )  /* const Vector&*/ { return v.outstream(stream); }
	std::ostream& operator<<( std::ostream& stream, Matrix& m )  /* const Matrix&*/ { return m.outstream(stream); }
}

#endif /* SBX_MATRIX_H_ */
