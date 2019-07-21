//
// Created by keyhan on 7/21/19.
//

#include <chrono>
#include "Time.h"

using namespace std;

long Time::getCurrentTimeMillis() {
    auto time = chrono::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto millis = chrono::duration_cast<chrono::milliseconds>(since_epoch);
    long now = millis.count();
    return now;
}
