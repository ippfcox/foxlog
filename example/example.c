#define FOXLOG_IMPLEMENTATION
#include "foxlog.h"

int main()
{
    foxlog_set_level(NULL, FOXLOG_LEVEL_INFO);
    foxloginfo("Hello %s", "World !");
}