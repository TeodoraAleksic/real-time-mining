from distutils.core import setup, Extension, DEBUG

sfc_module = Extension('realtimemining', sources = ['realtimemining.cpp'])

setup(name='realtimemining', 
      version='1.0',
      description='A module that simulates real-time mining',
      ext_modules=[sfc_module])
