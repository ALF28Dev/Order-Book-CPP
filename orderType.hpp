#include <stdio.h>
#include <iostream>

enum orderType {
    ORDER_MARKET = 1,
    ORDER_LIMIT = 2,
    ORDER_STOP_LIMIT = 3,
    ORDER_FILL_OR_KILL = 4
};