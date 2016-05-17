#ifndef SBX_DOUBLE_H_
#define SBX_DOUBLE_H_

#include <iostream>
#include <cmath>

class Double
{
    double value;

public:

    Double( void ) {}
    Double( double _double ) : value( _double ) {}
    Double( int _int ) : value( double( _int ) ) {} // cast
    Double( const Double& _Double ) : value( _Double.get_double() ) {}

    double get_double() const { return value; }
    void set_double( const double _value ) { value = _value; }
    Double& operator=( const Double& _Double ) { value = _Double.get_double(); return *this; }
    friend Double fabs(const Double& _Double );

    Double operator+=( const Double& _Double ) { value += _Double.get_double(); return *this; }
    Double operator-=( const Double& _Double ) { value -= _Double.get_double(); return *this; }
    Double operator*=( const Double& _Double ) { value *= _Double.get_double(); return *this; }
    Double operator/=( const Double& _Double )
    {
    	if ( fabs( _Double.get_double() ) < 1e-15 )
    	{
    		std::cerr << "ERROR - Division by zero" << std::endl;
    	    return 0.;
    	}
    	value /= _Double.get_double();
    	return *this;
    }

    Double operator-() { value = -value; return *this;}

    Double operator++() { ++value; return *this;}
    Double operator--(){ --value; return *this;}
    Double operator++( int ){ value++; return *this;}
    Double operator--( int ){ value--; return *this;}

    friend Double operator+( const Double&, const Double& );
    friend Double operator-( const Double&, const Double& );
    friend Double operator*( const Double&, const Double& );
    friend Double operator/( const Double&, const Double& );

    friend bool operator==( const Double&, const Double& );
    friend bool operator!=( const Double&, const Double& );
    friend bool operator<( const Double&, const Double& );
    friend bool operator<=( const Double&, const Double& );
    friend bool operator>( const Double&, const Double& );
    friend bool operator>=( const Double&, const Double& );

    friend std::ostream& operator<<( std::ostream&, const Double& );
    friend std::istream& operator>>( std::istream&, Double& );
};

Double fabs(const Double& _Double ) { return Double( fabs(_Double.get_double()) ); }

Double operator+( const Double& a, const Double& b ) { Double c( a.get_double() + b.get_double() ); return c; }
Double operator-( const Double& a, const Double& b ) { Double c( a.get_double() - b.get_double() ); return c; }
Double operator*( const Double& a, const Double& b ) { Double c( a.get_double() * b.get_double() ); return c; }
Double operator/( const Double& a, const Double& b )
{
	if ( fabs( b.get_double() ) < 1e-15 )
	{
		std::cerr << "ERROR - Division by zero" << std::endl;
	    return 0.;
	}
	Double c( a.get_double() / b.get_double() );
	return c;
}

bool operator==( const Double& a, const Double& b ) { return a.get_double() == b.get_double(); }
bool operator!=( const Double& a, const Double& b ) { return a.get_double() != b.get_double(); }
bool operator<( const Double& a, const Double& b ) { return a.get_double() < b.get_double(); }
bool operator<=( const Double& a, const Double& b ) { return a.get_double() >= b.get_double(); }
bool operator>( const Double& a, const Double& b ) { return a.get_double() > b.get_double(); }
bool operator>=( const Double& a, const Double& b ) { return a.get_double() >= b.get_double(); }

std::ostream& operator<<( std::ostream& stream, const Double& _Double ) { stream << _Double.get_double(); return stream;}
std::istream& operator>>( std::istream& stream, Double& _Double ) { double value ;stream >> value; _Double.set_double( value ); return stream;}

#endif /* SBX_DOUBLE_H_ */
