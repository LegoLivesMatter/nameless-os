/* Common bit flag ops */
#ifndef BITFLAGS_H
#define BITFLAGS_H

#define SET(flags, flag) flags |= (1 << flag)
#define CLEAR(flags, flag) flags &= ~(1 << flag)
#define IS_SET(flags, flag) (flags & (1 << flag) == (1 << flag))

#endif
