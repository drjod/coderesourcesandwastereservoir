#ifndef SBX_EQUATION_H_
#define SBX_EQUATION_H_

#include "sbx_matrix.h"
#include "sbx_math.h"
#include "sbx_stream.h"

namespace sbx {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Equation
    // Task:
    //         Assembly (only 1D diffusion implemented)
    //         incorporate BCs

    class Equation
    {
        friend class Solver; // that solver has access to A, b, x
        std::size_t size;

    protected:
        Matrix* A;
        Matrix* b;
        Matrix* x;

    public:
        Equation ( std::size_t _size ) : size(_size)
        {
            // config
            A = new Matrix(size,size,0.);
            b = new Matrix(size,1,0.);
            x = new Matrix(size,1,1.);
        }

        ~Equation() { delete A; delete b; delete x; }

        Matrix* get_result() { return x; }

        void assemble ( void(*func)( std::size_t, Matrix*) )
        {
            func(size, A); // currently only diffusion_1D( std::size_t size, Matrix* A )
        }

        void incorporate_boundary_conditions( void(*func)( std::size_t, Matrix*) )
        {
            func( size, b);
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Finite difference method - 1D diffusion equation

    void diffusion_1D( std::size_t size, Matrix* A )
    {
        Double* values = new Double[5];
        values[0]=-1.; values[1]=2; values[2]= -1; values[3]= 1.;

        A->assemble( 0, 0, 1, values+3 );
        A->assemble( size-1, size-1, 1, values+3 );

        for(std::size_t i = 1; i < size-1; i++ )
            A->assemble( i, i-1, 3, values);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // 3x3 example

    void example_3x3( std::size_t size, Matrix* A )
    {
        A->set(0,0, 9.);
        A->set(0,1, -3.);
        A->set(0,2, -6.);
        A->set(1,0, -3.);
        A->set(1,1, 10.);
        A->set(1,2, 5.);
        A->set(2,0, -6.);
        A->set(2,1, 5.);
        A->set(2,2, 6.);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    void fixed_heads( std::size_t size, Matrix* b )
    {
        b->set(0,0,2.);       // inlet, h(0) = 2
        b->set(size-1,0,1.);  // outlet, h(size-1) = 1
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    void for_example_3x3( std::size_t size, Matrix* b )
    {
        b->set(0,0,2.);
        b->set(1,0,2.);
        b->set(2,0,0.);
    }
}

#endif /* SBX_EQUATIONS_H_ */
