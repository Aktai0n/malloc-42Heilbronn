
# Small block layout:

```


Pos:    0 1 2 3 4 ... 63

Value:  0 1 0 0 0 ... 0
        | | | |       |
        | | | ---------
        | | |     |     
        | | |     size = 0
        | | |
        | | reserve (unused)
        | |
        | IS_LAST_BLOCK = true
        |
        IS_ALLOCATED = false


```