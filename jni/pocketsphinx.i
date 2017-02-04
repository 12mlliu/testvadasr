/*pocketsphonx.i by meili
*2017/01/05
*part of original pocketsphinx.i
*/

#if SWIGJAVA
%module PocketSphinx
#endif

/*not sure, maybe can automatically generated some file? .java .class*/
%feature("autodoc","1");

/*question1: look typemaps.i and iterators.i and sphinxbase.i*/
%include typemaps.i
%include iterators.i
%import sphinxbase.i
/*the two typedef for Config and Decoder can be merge as one ? try*/
%{
#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif


#include <pocketsphinx.h>

typedef cmd_ln_t Config;


%}

%inline %{
char const *decoder_test(Config* config,const int16 *SDATA, size_t NSAMP) {
        char const *hypstr;
        hypstr = ps_decoder_test(config,SDATA, NSAMP);
        return hypstr;
        }
%}
/*extern char const *ps_decoder_test(cmd_ln_t *config, int16 const *data,size_t n_samples);*/
