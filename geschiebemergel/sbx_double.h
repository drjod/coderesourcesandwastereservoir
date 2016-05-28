#ifndef SBX_DOUBLE_H_
#define SBX_DOUBLE_H_

#include <iostream>
#include <cmath>

namespace sbx{

    class Double;  // forward declaration
    extern const Double eps; //  in solver

    class Double
    {
        double value;

    public:

        Double( void ) {}
        Double( double _double ) : value( _double ) {}
        Double( int _int ) : value( double( _int ) ) {} // cast
        Double( const Double& _Double ) : value( _Double.get_value() ) {}

        double get_value() const { return value; }
        void set_value( const double _value ) { value = _value; }
        Double& operator=( const Double& _Double ) { value = _Double.get_value(); return *this; }
        friend Double fabs(const Double& _Double );

        Double operator!() { return *this; } // transpose - for template in sbx_stream.h writeVariable()
        Double operator+=( const Double& _Double ) { value += _Double.get_value(); return *this; }
        Double operator-=( const Double& _Double ) { value -= _Double.get_value(); return *this; }
        Double operator*=( const Double& _Double ) { value *= _Double.get_value(); return *this; }
        Double operator/=( const Double& );

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

    Double Double::operator/=( const Double& _Double )
    {
        try
        {
            if ( fabs( _Double.get_value() ) < eps )
                throw "ERROR - Division by zero";
        }
        catch (const char* msg)
        {
            std::cout << msg << std::endl;
        }
        value /= _Double.get_value();
        return *this;
    }

    Double fabs(const Double& _Double ) { return Double( std::fabs(_Double.get_value())); }
    Double sqrt(const Double& _Double) { return Double( std::sqrt(_Double.get_value()));}

    Double operator+( const Double& a, const Double& b ) { Double c( a.get_value() + b.get_value() ); return c; }
    Double operator-( const Double& a, const Double& b ) { Double c( a.get_value() - b.get_value() ); return c; }
    Double operator*( const Double& a, const Double& b ) { Double c( a.get_value() * b.get_value() ); return c; }
    Double operator/( const Double& a, const Double& b )
    {
        Double c;
        try
        {
            if ( b.get_value() == 0 )
                throw "ERROR - Division by zero";
        }
        catch (const char* msg)
        {
            std::cout << msg << std::endl;
        }
        c.set_value ( a.get_value() / b.get_value() );
        return c;
    }

    bool operator==( const Double& a, const Double& b ) { return a.get_value() == b.get_value(); }
    bool operator!=( const Double& a, const Double& b ) { return a.get_value() != b.get_value(); }
    bool operator<( const Double& a, const Double& b ) { return a.get_value() < b.get_value(); }
    bool operator<=( const Double& a, const Double& b ) { return a.get_value() >= b.get_value(); }
    bool operator>( const Double& a, const Double& b ) { return a.get_value() > b.get_value(); }
    bool operator>=( const Double& a, const Double& b ) { return a.get_value() >= b.get_value(); }

    std::ostream& operator<<( std::ostream& stream, const Double& _Double ) { stream << _Double.get_value(); return stream;}
    std::istream& operator>>( std::istream& stream, Double& _Double ) { double value ;stream >> value; _Double.set_value( value ); return stream;}

}

#endif /* SBX_DOUBLE_H_ */
