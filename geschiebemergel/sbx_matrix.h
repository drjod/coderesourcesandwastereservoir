#ifndef SBX_MATRIX_H_
#define SBX_MATRIX_H_

#include <iostream>
#include <stdlib.h>
#include <exception>
#include "sbx_double.h"
#include "sbx_configuration.h"

namespace sbx {

typedef sbx::Double DOUBLE ;

    class out_of_range_error : public std::exception
    {
    public:
        const char* what() const throw()
        {
            return "Out off range error - sbx::Matrix";
        }
    };

    class matrix_mismatch_error : public std::exception
    {
    public:
        const char* what() const throw()
        {
            return "Matrix dimension mismatch error - sbx::Matrix";
        }
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //    class Matrix
    //
    //
    //

    class Matrix
    {
        std::size_t rows, columns;
        DOUBLE** elems;

    public:
        // con-/destructors
        Matrix() { rows = 0; columns = 0; }
        Matrix( std::size_t _rows ) : rows( _rows ), columns( _rows ) // symmetric
        {
            try
            {
                elems = new DOUBLE*[_rows];
                for(std::size_t i=0; i< _rows; i++)
                    elems[i] = new DOUBLE[_rows];
            }
            catch ( std::bad_alloc& ba )
            {
                std::cout << "ERROR in Matrix() - " << ba.what() << std::endl; // cerr
            }
        }


        Matrix( std::size_t _rows, std::size_t _columns ) : rows( _rows ), columns( _columns )
        {
            try
            {
                elems = new DOUBLE*[_rows];
                for( std::size_t i=0; i< _rows; i++ )
                    elems[i] = new DOUBLE[_columns];
            }
            catch ( std::bad_alloc& ba )
            {
                 std::cout << "ERROR in Matrix() - " << ba.what() << std::endl; // cerr
            }
        }

        Matrix( std::size_t _rows, std::size_t _columns, DOUBLE value ) : rows( _rows ), columns( _columns ) // with initialization
        {
            try
            {
                elems = new DOUBLE*[_rows];
                for( std::size_t i=0; i< _rows; i++ )
                    elems[i] = new DOUBLE[_columns];
            }
            catch ( std::bad_alloc& ba )
            {
                std::cout << "ERROR in Matrix() - " << ba.what() << std::endl; // cerr
            }

            for( std::size_t i=0;  i < _rows; i++ )
                for( std::size_t j=0; j< _columns; j++ )
                    elems[i][j] = value;
        }

        Matrix( const Matrix& other ) : rows( other.get_rows() ), columns( other.get_columns() ) // copy Matrix
        {
            try
            {
                elems = new DOUBLE*[other.get_rows()];
                for( std::size_t i=0; i< other.get_rows(); i++ )
                    elems[i] = new DOUBLE[other.get_columns()];
            }
            catch ( std::bad_alloc& ba )
            {
                std::cout << "ERROR in Matrix() - " << ba.what() << std::endl; // cerr
            }


            for( std::size_t i=0;  i < other.get_rows(); i++ )
                for( std::size_t j=0; j< other.get_columns(); j++ )
                    elems[i][j] = other.at( i,j );
        }

        Matrix( DOUBLE value ) : rows( 1 ), columns( 1 ) // for cast
        {
            try
            {
                elems = new DOUBLE*[1];
                elems[0] = new DOUBLE[1];
            }
            catch ( std::bad_alloc& ba )
            {
                std::cout << "ERROR in Matrix() - " << ba.what() << std::endl; // cerr
            }

            elems[0][0] = value;
        }

        ~Matrix( void )
        {
            for( std::size_t i =0; i < rows; i++ )
                delete [] elems[i];
            delete [] elems;
        }

        /////

        DOUBLE at( std::size_t row, std::size_t column ) const
        {
            try
            {
                if( row < 0 || row >= rows || column < 0 || column >= columns )
                    throw out_of_range_error();
                else
                    return elems[row][column];
            }
            catch ( out_of_range_error& e )
            {
                 std::cout << e.what() << std::endl; // cerr
                 return 0.;
            }
        }

        void set( std::size_t row, std::size_t column, DOUBLE value )
        {
            try
            {
                if( row < 0 || row >= rows || column < 0 || column >= columns )
                    throw out_of_range_error();
                else
                    elems[row][column] = value;
            }
            catch ( out_of_range_error& e )
            {
                std::cout << e.what() << std::endl; // cerr
            }
        }

        DOUBLE** get_elems( void ) { return elems; }
        std::size_t get_rows( void ) const { return rows; }
        std::size_t get_columns( void ) const { return columns; }
        Matrix unity( void ) {
             for( std::size_t i=0; i<rows; i++ )
                        for( std::size_t j=0; j<columns; j++ )
                            if( i == j )
                                elems[i][j] = 1.;
                            else
                                elems[i][j] = 0.;
             return *this;
        }

        Matrix& operator=( const Matrix&);
        Matrix operator-();
        Matrix operator!(); // transpose

        friend Matrix operator+( const Matrix&, const Matrix& );
        friend Matrix operator-( const Matrix&, const Matrix& );
        friend Matrix operator*( const Matrix&, const Matrix& );
        friend DOUBLE operator|( const Matrix&, const Matrix& ); // inner product

        friend std::ostream& operator<<( std::ostream&, const Matrix& );
        friend std::istream& operator>>( std::istream&, Matrix& );

        void assemble( std::size_t row, std::size_t locationOfFirstColumn, std::size_t numberOfColumns, DOUBLE* values )
        {
            for( std::size_t i = 0; i< numberOfColumns; i++ )
                elems[row][locationOfFirstColumn+i]  = values[i];
        }
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Assignment

    Matrix& Matrix::operator=( const Matrix& other )
    {
        if (this == &other)
            return *this;

        DOUBLE **ptr= new DOUBLE*[other.get_rows()];
        for( std::size_t i=0; i< other.get_rows(); i++ )
            ptr[i] = new DOUBLE[other.get_columns()];

        for( std::size_t i=0;  i < other.get_rows(); i++ )
            for( std::size_t j=0; j< other.get_columns(); j++ )
                ptr[i][j] = other.at( i,j );

        for( std::size_t i=0;  i < rows; i++ )
            delete [] elems[i];
        if ( columns > 0)
            delete [] elems;

        rows =  other.get_rows();
        columns =  other.get_columns();

        elems = new DOUBLE*[rows];
        for( std::size_t i=0; i< rows; i++ )
            elems[i] = new DOUBLE[columns];

        for( std::size_t i=0; i< rows; i++ )
                elems[i] = ptr[i];

        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Negation

    Matrix Matrix::operator-()
    {
         Matrix C( rows, columns );

         for( std::size_t i=0; i<rows; i++ )
             for( std::size_t j=0; j<columns; j++ )
                 C.set(i,j,-elems[i][j]);

         return C;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Transpose


    Matrix Matrix::operator!()
    {
         Matrix C( columns, rows );

         for( std::size_t i=0; i<columns; i++ )
             for( std::size_t j=0; j<rows; j++ )
                 C.set(i,j,elems[j][i]);

         return C;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix operator+( const Matrix& A, const Matrix& B )
    {
        Matrix C( A.get_rows(),A.get_columns() );
        try
        {
            if ( A.get_rows() != B.get_rows() || A.get_columns() != B.get_columns() )
                throw matrix_mismatch_error();
            else
            {
                for( std::size_t i=0;  i < A.get_rows(); i++ )
                    for( std::size_t j=0; j< A.get_columns(); j++ )
                         C.set( i,j, A.at(i,j) + B.at(i,j) );
            }
        }
        catch ( matrix_mismatch_error& e )
        {
            std::cout << e.what() << std::endl; // cerr
        }

        return C;
     }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix operator-( const Matrix& A, const Matrix& B )
    {
        Matrix C( A.get_rows(),A.get_columns() );
        try
        {
            if ( A.get_rows() != B.get_rows() || A.get_columns() != B.get_columns() )
                throw matrix_mismatch_error();
            else
            {
                for( std::size_t i=0;  i < A.get_rows(); i++ )
                    for( std::size_t j=0; j< A.get_columns(); j++ )
                        C.set( i,j, A.at(i,j) - B.at(i,j) );
            }
        }
        catch ( matrix_mismatch_error& e )
        {
            std::cout << e.what() << std::endl; // cerr
        }

        return C;
     }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix operator*( const Matrix& A, const Matrix& B )
    {
        DOUBLE value;

        if ( A.get_rows() == 1 && A.get_columns() == 1 )
        {  // DOUBLE * Matrix
            Matrix C(B.get_rows(), B.get_columns() );
            value = A.at(0,0);
            for( std::size_t i = 0;  i < C.get_rows(); i++ )
                for( std::size_t j=0; j< C.get_columns(); j++ )
                    C.set( i,j, value * B.at(i,j) );
            return C;
        }
        else
        { // matrix multiplication
            Matrix C(A.get_rows(), B.get_columns() );
            try
            {
                if ( A.get_columns() != B.get_rows() )
                    throw matrix_mismatch_error();
                else
                {
                    for( std::size_t i = 0;  i < C.get_rows(); i++ )
                        for( std::size_t j=0; j< C.get_columns(); j++ )
                        {
                            value = 0;
                            for( std::size_t k=0; k < A.get_columns(); k++ )
                                value +=  A.at(i,k) * B.at(k,j);
                            C.set( i,j, value );
                        }
                }
            }
            catch ( matrix_mismatch_error& e )
            {
                std::cout << e.what() << std::endl; // cerr
            }
            return C;
        } // end else (matrix multiplication)
    }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //
   // Inner product


   DOUBLE operator|( const Matrix& A, const Matrix& B )
   {
       DOUBLE innerProduct = 0;
       try
       {
           if ( A.get_rows() != B.get_rows() || A.get_columns() != B.get_columns() )
               throw matrix_mismatch_error();
           else
           {
               for( std::size_t i=0; i<A.get_rows(); i++ )
                   for( std::size_t j=0; j<A.get_columns(); j++ )
                       innerProduct += A.at(i,j) *  B.at(i,j);
           }
       }
       catch ( matrix_mismatch_error& e )
       {
           std::cout << e.what() << std::endl; // cerr
       }
       return innerProduct;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::ostream& operator<<( std::ostream& stream, const Matrix& A)
    {
        for( std::size_t i=0;  i < A.get_rows(); i++ )
        {
            for( std::size_t j=0; j< A.get_columns(); j++ )
                stream << "[" << i <<  "," << j << "]: " << A.at(i,j) << "  ";
            stream << std::endl;
        }
        return stream;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::istream& operator>>( std::istream& stream, Matrix& A)
    {
        DOUBLE value;
        for( std::size_t i=0;  i < A.get_rows(); i++ )
        {
            for( std::size_t j=0; j< A.get_columns(); j++ )
            {
                std::cout << "[" << i<< "," << j << "]: ";
                stream >> value;
                A.set(i,j, value);
            }
        }
        return stream;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // NON-MEMBER FUNCTIONS

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Unit matrix

    Matrix unity( std::size_t rows, std::size_t columns=0 )
    {
        if ( columns == 0 )
            columns = rows; // symmetric

        Matrix C( rows, columns );
        for( std::size_t i=0; i<rows; i++ )
            for( std::size_t j=0; j<columns; j++ )
                if( i == j )
                    C.set(i,i,1.);
                else
                    C.set(i,i,0.);
        return C;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Norms

    DOUBLE norm_0( Matrix* A ) // max norm
    {
        DOUBLE value = 0;
        DOUBLE ** elems = A->get_elems();

        for( std::size_t i=0; i<A->get_rows(); i++ )
            for( std::size_t j=0; j<A->get_columns(); j++ )
            {
                if( fabs( elems[i][j]) > value )
                    value = fabs( elems[i][j] );
            }
        return value;
    }

    DOUBLE norm_1( Matrix* A )
    {
        DOUBLE value = 0;
        DOUBLE ** elems = A->get_elems();

        for( std::size_t i=0; i<A->get_rows(); i++ )
            for( std::size_t j=0; j<A->get_columns(); j++ )
            {
                if( elems[i][j] > 0 )
                    value += elems[i][j];
                else
                    value -= elems[i][j];
            }
        return value;
    }

}
#endif /* SBX_MATRIX_H_ */
