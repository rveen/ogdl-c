#!/usr/bin/env python

"""
setup.py file for SWIG
"""

from distutils.core import setup, Extension

#TODO : generate ogdlPYTHON_wrap dynamically on installation (check how pyzmq does it)
# CMake Ref : http://www.benjack.io/2017/06/12/python-cpp-tests.html
# Python Ref : https://packaging.python.org/guides/packaging-binary-extensions/

# but lets KISS

ogdl_module = Extension(
	'_ogdl',
    sources=[
		'src/ogdlPYTHON_wrap.c',
		'src/graph.c',
		'src/ogdlbin.c',
		'src/ogdllog.c',
		'src/ogdlparser.c',
		'src/path.c'
	],
)

setup (name = 'ogdl',
       version = '0.1',
       author      = "AlexV",
       description = """OGDL extension for python""",
       ext_modules = [ogdl_module],
       py_modules = ["ogdl"],
       )

