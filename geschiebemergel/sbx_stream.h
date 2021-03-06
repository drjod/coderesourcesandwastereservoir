#ifndef SBX_STREAM_H_
#define SBX_STREAM_H_

#include <fstream>
#include <string>
#include <iostream>

#define CONSOLE std::cout




namespace sbx {

    int verbosity = 1;  // console and file output

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Stream
    // Task:
    //         parent class for input and output stream class


    class Stream
    {
     public:
        Stream() {}
        template <typename T_STREAM> void _open( std::string filename, T_STREAM& stream )
        {
            stream.open( filename );
            stream.precision(10);
            if (!stream)
                std::cout << "Error - Could not open stream for " << filename << std::endl;
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Output : public Stream
    {
        std::ofstream outstream;

    public:
        Output(std::string filename) { _open( filename, outstream); }

        ~Output() { outstream.close(); }

        std::ofstream& get_stream(void) { return outstream; }

        template <typename T_VARIABLE> void write( std::string variableName, T_VARIABLE variable, int verbosityLevel = 0 )
        { writeVariable( variableName , variable, outstream, verbosityLevel ); }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Input : public Stream
    {
        std::ifstream instream;

        Input(std::string filename) { _open( filename, instream); }

        ~Input() { instream.close(); }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Non-member functions that support also console output (T_STREAM& std::cout)

    template <typename T_VARIABLE, typename T_STREAM> void writeVariable( std::string info, T_VARIABLE* variable, T_STREAM& outstream, int verbosityLevel )
    {
        if( verbosityLevel < verbosity )
        {
            outstream << info << ": " << std::endl;
            outstream << *variable << std::endl << std::endl;
        }
    }

    template <typename T_VARIABLE, typename T_STREAM> void show( std::string info, T_VARIABLE* variable, T_STREAM& outstream, int verbosityLevel )
    { writeVariable( info, variable, outstream, verbosityLevel ); }

    template <typename T_STREAM> void inform( std::string info, T_STREAM& outstream, int verbosityLevel )
    {
        if( verbosityLevel < verbosity )
            outstream << info << std::endl << std::endl;
    }


}
#endif /* SBX_STREAMS_H_ */
