#define FOXLOG_IMPLEMENTATION
#include "foxlog.h"
#include "sample.h"

int main()
{
    sample1_call();
    sample2_call();
    foxlogerror("main call");
}