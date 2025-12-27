#pragma once

#ifdef __EMSCRIPTEN__

struct high_score_struct;

class idb_high_score
{
public:
    static int read(high_score_struct* table);
    static int write(high_score_struct* table);
    static int clear();
};

#endif // __EMSCRIPTEN__
