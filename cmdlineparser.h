#ifndef CMDLINEPARSER_H_
#define CMDLINEPARSER_H_

#include <getopt.h>

class CmdLineParser
{
public:

    CmdLineParser( int num_args, char **arg_vector )
    {
        if ( num_args != 1 )
        {
            while ( 1 )
            {
                int option_index = 0;

                int opt = getopt_long( num_args,
                                       arg_vector,
                                       "i:",
                                       long_options,
                                      &option_index );

                if ( opt == -1 )
                    break;

                switch ( opt )
                {
                case 'i':
                    input_script_filename_ = optarg;
                    break;
                default:
                    abort( );
                }
            }
        }
    }

    std::string getInputScriptFilename( void )
    {
        return input_script_filename_;
    }

private:

    static struct option long_options[];

    std::string input_script_filename_;
};

struct option CmdLineParser::long_options[] = {
    { "input-lua-script", required_argument,  0, 'i' },
    { 0, 0, 0, 0 } };

#endif /* CMDLINEPARSER_H_ */
