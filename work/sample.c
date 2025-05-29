#include "foxlog.h"

int main()
{
    foxlog_set_level(NULL, FOXLOG_LEVEL_WARN);
    foxlogerrorm(NULL, "%s, %f", "aaa", 0.45f);
    foxlogerror("%s, %f", "aaa", 0.45f);
}