This guide defines the "style" of the source code of this application. Having a unified style makes reading the code easier.

- Use `snake_case` for functions, types, and local variables.
- Prefix all macros, functions, and types with `C8`. 
	- E.g. `c8_register`, `C8_H_REGISTER`, etc.
- For functions with no parameters, include `void`.
	- E.g. `c8_init(void)`
- Put the star next to the type if it's a pointer - I much prefer `int* a` over `int *a`.
- Place the starts of blocks on their own line.
```c
// Prefer this
void c8_init(void)
{
    // ...
}

// Over this
void c8_bad(void) {
    // ....
}
```
- Prepend static members with underscores to denote them as private to the module.
- Separate includes, typedefs + statics, and public-facing functions to the module body:
```c
#include "test.h"

/*********************************************************************************/

enum test {};
static int _my_int = 0;

static void c8_util_func(void) 
{
	// ...
}

/*********************************************************************************/

void c8_public(void)
{
	// ...
}
```