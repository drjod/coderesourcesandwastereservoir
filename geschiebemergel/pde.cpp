#include <iostream>
#include "sbx_matrix.h"
#include "sbx_solver.h"
#include "sbx_double.h"
#include "sbx_clock.h"
#include "sbx_stream.h"
#include "sbx_equation.h"


using namespace std;
using namespace sbx;


int main(int argc, char** argv)
{
    sbx::Clock time;
    Output result( "result.txt" );
    Bcgs solver( 1.e-5, 100000, norm_0 ); // error, max iterations
    // Gauss solver;
    Equation groundwater( 10 ); // number of finite difference nodes

    time.start("Assembly");
    groundwater.assemble( diffusion_1D );
    groundwater.incorporate_boundary_conditions( fixed_heads );
    time.measure( CONSOLE );

    time.start("Solver");
    solver.execute( &groundwater );
    time.measure( CONSOLE );

    show( "Result x",groundwater.get_result(), CONSOLE, REGULAR );
    result.write( "x", groundwater.get_result() );

    return 0;
}
