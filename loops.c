#ifndef _C4_LOOPS_
#define _C4_LOOPS_

#define count_up(min, max, name) \
  count_up_by (1, from(min), to(max), as(name))

#define count_up_by(amount, min, max, name) \
    for (                                   \
        unsigned long int name = (min);     \
        name <= (max);                      \
        name += (amount)                    \
    )

#define count_down(max, min, name)          \
  count_down_by (1, from(max), to(min), as(name))

#define count_down_by(amount, max, min, name)   \
    for (                                       \
        unsigned long name = (max);             \
        name >= (min);                          \
        name -= (amount)                        \
    )

#define item_from(x) x
#define      from(x) x
#define        to(x) x
#define        as(x) x

#define  until(x) while (! (x))
#define unless(x) if    (! (x))

#define for_each(array, item)                           \
    for (                                               \
      unsigned long _i = 0, _keep = 1;                  \
      _keep && _i < sizeof(array) / sizeof((array)[0]); \
      _i++, _keep = !_keep                              \
    )                                                   \
      for (item = (array)[_i]; _keep; _keep = !_keep)

#endif // _C4_LOOPS_
