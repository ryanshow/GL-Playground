# What Is This? #

GL-Playground is a place for me to experiment with OpenGL functionality without any obligations.
I make no guarantees that my code will work for you, but I try to make everything as standards compliant as possible.
All of the examples will utilize OpenGL 3.2 (core profile) or above.  This means deprecated API functionality will not be used.

The following examples are included in this project:

## Example 1 ##
Opens up a basic OpenGL window using SDL 1.3 and a OpenGL 3.2 context

## Example 2 ##
Renders a very simple wireframe cube to the window using vbos and shaders.

## Example 3 ##
Introduces texturing, lighting, and normal maps.  Models are loaded from a YAML file (in progress)

# Compiling #

create a directory in the top-level directory called "build"

```
cd build
cmake -G "Unix Makefiles" ../
make
```

Feel free to replace "Unix Makefiles" with whatever platform you happen to 
be using.
