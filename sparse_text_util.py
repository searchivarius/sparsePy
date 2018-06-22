# 
#  Authors: Leonid Boytsov <leo@boytsov.info>
#           heavily relying on the code of 
#           Mathieu Blondel <mathieu@mblondel.org>
#        
#           https://github.com/mblondel/svmlight-loader
# 
#  License: Simple BSD
#
import numpy as np
import scipy as sp

from _sparse_text_util import _sparse_matr_to_text

def sparse_matr_to_text(X, file_name, append_flag):
    """Dump the dataset in nearly svmlight / libsvm file format.

      This format is a text-based format, with one sample per line. It does
      not store zero valued features hence is suitable for sparse dataset.
      Unlike the svmlight format, we do not save the label.

      @param X  a CSR sparse matrix to save
      @param file_name an output file name
      @param append_flag if true, appends to the file
    """
    X = sp.sparse.csr_matrix(X, dtype=np.float64)
    _sparse_matr_to_text(file_name, X.data, X.indices, X.indptr, append_flag)


