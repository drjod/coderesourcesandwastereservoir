#ifndef SBX_SOLVER_H_
#define SBX_SOLVER_H_

#include "sbx_matrix.h"
#include "sbx_double.h"
#include "sbx_stream.h"
#include "sbx_equation.h"


namespace sbx {

    const Double eps = 1.e-15;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Solver
    // Task:
    //         Parent class for solver of Ax=b

    class Solver
    {
    protected:

        Matrix* A; // gives individual solver access to Equation member *A, *b, *x by declaring Solver as friend
        Matrix* b;
        Matrix* x;

        Double error;
        int maxIterations;
        Double(*norm)(Matrix*);

        void set_matrix_pointer( Equation* eqn ) { A = eqn->A; b = eqn->b; x = eqn->x; }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Class Bcgs
    // Task:
    //         biconjugate gradient method

    class Bcgs : public Solver
    {
        Double r_rs, A_p_rs;
        Double alpha, beta, omega;
        Matrix r, s, p, rs;
        Matrix A_s, A_p;

    public:
        Bcgs ( Double _error, int _maxIterations, Double(*_norm)(Matrix*) )
        {
            error = _error;
            maxIterations = _maxIterations;
            norm = _norm;
        }

        void execute( Equation* );

        template <typename T> void writeIterationValues( int iterationNDX, Double runningError, T& outstream )
            {
                //show( "    Iterations", (double*)iterationNDX, outstream, 1 );
                show( "        Error", &runningError, outstream, 1 );

                show( "!x",x, outstream, 2, TRANSPOSE );
                show( "!p",&p, outstream, 2, TRANSPOSE );
                show( "!r",&r, outstream, 2, TRANSPOSE );
                show( "!s",&s, outstream, 2, TRANSPOSE );
                show( "alpha",&alpha, outstream, 2 );
                show( "beta",&beta, outstream, 2 );
                show( "omega",&omega, outstream, 2 );
            }
    };

    void Bcgs::execute( Equation* eqn )
    {
        inform( "Solver bcgs" , CONSOLE, 0 );
        set_matrix_pointer( eqn );
        int i=0;
        Double runningError = 0.;

        alpha = beta = omega = 0.;
        s=(Double)0.;
        r = *b - *A * *x;
        p = r;
        rs = r + *x;
        writeIterationValues( i, runningError, CONSOLE );

        for( ; i < maxIterations; i++ )
        {
            A_p = *A * p;
            r_rs = (r|rs);
            A_p_rs = (A_p|rs);
            alpha = r_rs/(A_p_rs);
            s = r - alpha * A_p;
            A_s = *A * s;
            omega = (A_s|s)/(A_s|A_s);
            *x = *x + alpha * p + omega * s;
            r = s - omega * A_s;
            beta = (r|rs) * alpha / (r_rs * omega);
            p = r + beta * (p - omega * A_p);

            runningError = norm(&r)/ norm(b);
            writeIterationValues( i, runningError, CONSOLE );
            if ( fabs(runningError) < error)
                break;
        } // end for

        //show( "Iteration", (double*)&i, CONSOLE, 0 );
        show( "Error", &runningError, CONSOLE, REGULAR );
    }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Class Gauss
        // Task:
        //         Gauss elimination

        class Gauss : public Solver
        {
             std::size_t size;

        public:
            Gauss () {}

            void execute( Equation* );

            template <typename T> void writeIterationValues( int iterationNDX, T& outstream )
            {
                double* ptr = new double;  // to cast int
                *ptr = iterationNDX;
                show( "    Iterations", ptr, outstream, 1 );

                show( "A", A, outstream, 2 );
                show( "!b", b, outstream, 1 );

                delete ptr;
            }
        };

        void Gauss::execute( Equation* eqn )
        {
            inform( "Solver Gauss" , CONSOLE, 0 );
            set_matrix_pointer( eqn );
            size = A->get_columns(); // n x n
            Double factor, temp;

            // elimination - mode 0
            for ( std::size_t p = 0; p < size-1; p++) // pivot
            {
                for ( std::size_t i = p+1; i < size; i++)
                {
                    if( fabs(A->at(i, p )) > eps )
                    {
                        factor = A->at(i, p) / A->at(p, p);
                        for ( std::size_t j = p+1; j < size; j++)
                        {
                            A->set( i, j, A->at(i, j) - factor * A->at(p,j) );
                        }
                        b->set( i, 0,  b->at(i,0) - factor * b->at(p,0) );
                    }
                }

                writeIterationValues( p, CONSOLE );
            }
            // substitution - mode 1
            for ( std::size_t i = size-1; i >=0; i--)
            {
                temp = 0;
                for ( std::size_t j = i+1; j < size; j++)
                    temp += A->at(i,j)* x->at(j,0);
                x->set(i, 0, (b->at(i,0) - temp  ) / A->at(i,i) );
            }
        }
}


#endif /* SBX_SOLVER_H_ */
