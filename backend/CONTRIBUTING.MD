# Beyond RGB Backend Coding Style Guide

This is a short document describing the preferred coding style for the backend
of Beyond RGB. While coding styles differ per person, a consistant style makes
software more understandable and maintainable.

## General

Unless otherwise specified, follow the PEP-8 coding style guide.

## Docstrings

All functions should have some form of docstring. Docstrings should contain a
description of the function as well as any inputs and outputs. The description
should be kept to one line if at all possible without loosing clarity. Input and
output descriptions should be aligned vertically. An example of this is shown
below.

```python
def foobar(foo, bar):
    """ Function description goes here. Try to keep it to one line.
    [in] foo: foo description
    [in] bar: bar description
    [out] description of one returned variable
    [out] description of another returned variable
    """
```

If the function is trivial, a single line describing the
function is acceptable.

The top of each module should contain a docstring with the name of the file,
description, functions, authors, and license as shown below.
```python
""" foo.py
This is a description of the foo module

Functions:
    foobar(foo, bar)
    foobar2(foo2, bar2)

Authors:
    John Smith <john@smith.com>
    Jane Doe   <jane@doe.com>

License:
    Copywrite 2022
    See license file for more information
"""
```
