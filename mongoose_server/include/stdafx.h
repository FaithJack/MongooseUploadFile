#ifndef STDAFX_H
#define STDAFX_H

#include "mongoose.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct file_writer_data
{
    FILE *fp;
    size_t bytes_written;
};

#endif