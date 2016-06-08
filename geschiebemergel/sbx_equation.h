#ifndef SBX_EQUATION_H_
#define SBX_EQUATION_H_

#include "sbx_matrix.h"



namespace sbx {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Equation
    // Task:
    //         Assembly (only 1D diffusion implemented)
    //         incorporate BCs

    class Equation
    {
        //friend class Solver; // that solver has access to A, b, x
        SIZE_T size;

    //protected:
    public:
        Matrix* A;
        Vector* b;
        Vector* x;

    public:
        Equation ( SIZE_T _size ) : size(_size), A(new Matrix), b(new Vector), x(new Vector)
        {
            A->allocate(_size, _size);
            b->allocate(_size);
            x->allocate(_size);

            *A = 0;
            *b = 0;
            *x = 0;
        }

        ~Equation() { delete A; delete b; delete x; }

       Vector* get_result(void) { return x; }
       SIZE_T get_size(void) { return size; }

        void assemble ( void(*func)( SIZE_T, Matrix*) )
        {
            func(size, A); // currently only diffusion_1D( std::SIZE_T size, Matrix* A )
        }

        void incorporate_boundary_conditions( void(*func)( SIZE_T, Vector*) )
        {
            func( size, b);
        }

        Matrix* get_matrix() { return A; }
        Vector* get_RHS() { return b; }

        //template <typename T_STREAM> void showMatrix( T_STREAM& outstream, int verbosity ) { show( "A", A, outstream, verbosity ); }

    };

    void assemble( Matrix* A, SIZE_T row, SIZE_T locationOfFirstColumn, SIZE_T numberOfColumns, double* values )
     {
         for( SIZE_T i = 0; i< numberOfColumns; i++ )
             (*A)(row, locationOfFirstColumn+i)  = values[i];
     }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Finite difference method - 1D diffusion equation

    void diffusion_1D( SIZE_T size, Matrix* A )
    {
        double* values = new double[4];
        values[0]=-1.;
        values[1]=2;
        values[2]= -1;
        values[3]= 1.;

        assemble( A, 0, 0, 1,values+3 );
        assemble( A, size-1, size-1, 1, values+3 );

        for(SIZE_T i = 1; i < size-1; i++ )
        	assemble( A, i, i-1, 3, values);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // 3x3 example

    void example_3x3( SIZE_T size, Matrix* A )
    {
        (*A)(0,0) =  9.;
        (*A)(0,1) = -3.;
        (*A)(0,2) = -6.;
        (*A)(1,0) = -3.;
        (*A)(1,1) = 10.;
        (*A)(1,2) = 5.;
        (*A)(2,0) = -6.;
        (*A)(2,1) = 5.;
        (*A)(2,2) = 6.;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    void fixed_heads( SIZE_T size, Vector* b )
    {
    	(*b)(0) = 2.;       // inlet, h(0) = 2
    	(*b)(size-1) = 1.;  // outlet, h(size-1) = 1
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    void for_example_3x3( SIZE_T size, Vector* b )
    {
        (*b)(0) = 2.;
        (*b)(1) = 2.;
        (*b)(2) = 0.;
    }

}

#endif /* SBX_EQUATIONS_H_ */
