#pragma once

#include <inttypes.h>

class IdGenerator
{
public:

    uint64_t GenerateId()
    {
        auto lReturnedId = mNextId;
        mNextId++;
        return lReturnedId;
    }

    void Reset()
    {
        mNextId = 0;
    }


private:

    uint64_t mNextId;

};