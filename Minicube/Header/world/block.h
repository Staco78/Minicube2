#pragma once

enum BlockId
{
    AIR,
    GRASS,
    STONE,
    SAND,
    BLUE
};

typedef struct
{
    BlockId id;
} Block;