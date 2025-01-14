#include <stdlib.h>
#include <string.h>

#include "parser.h"

/*
Contains all the functions to parse the operator part of the grammar
--> Pipelines
--> And/Or
*/

/*
pipeline = ['!'] command { '|' {'\n'} command } ;
not in step 1
*/
struct ast *pipeline(struct parser *parser)
{
    return command(parser);
}

/*
and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
not in step 1
*/
struct ast *and_or(struct parser *parser)
{
    return pipeline(parser);
}
