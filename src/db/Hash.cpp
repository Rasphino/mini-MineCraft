//
//  Hash.cpp
//  shader
//
//  Created by jones on 2019/5/22.
//  Copyright © 2019 jones. All rights reserved.
//
#include <iostream>
#include <string>
#include "Hash.h"
using namespace std;


int hash_getchunkid(int x, int z) {
    int id = 0;
    id = x / 16 + z / 16 * 32;
    return id;
}
