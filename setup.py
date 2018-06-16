from distutils.core import setup, Extension

ext = Extension('sparse_py',
                include_dirs = ['.'],
                extra_compile_args=['-O3'],
                sources = ['main.cc'])

setup (name = 'sparse_py',
       version = '0.1',
       description = 'Fast writer for the near-svmlight/libsvm sparse data format.',
       ext_modules = [ext],
       py_modules = ['sparse_py',])
