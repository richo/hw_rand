from distutils.core import setup, Extension

module1 = Extension('hw_rand',
        sources = ['hw_randmodule.c'])

setup (name = 'HwRand',
        version = '1.0',
        description = 'Interface for poking at intel\'s RDRAND',
        ext_modules = [module1])
