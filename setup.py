from distutils.core import setup, Extension
import numpy as np

ext = Extension('_sparse_text_util',
                include_dirs = [np.get_include(), '.'],
                extra_compile_args=['-std=c++11', '-O3'],
                sources = ['sparse_text_util.cc'])

setup (name = 'sparse_text_util',
       version = '0.1',
       description = 'Fast writer for the near-svmlight/libsvm sparse data format.',
       ext_modules = [ext],
       py_modules = ['sparse_text_util',])
