/*
 * Authors: Leonid Boytsov <leo@boytsov.info>
 *          heavily relying on the code of 
 *          Mathieu Blondel <mathieu@mblondel.org>
 *       
 *          https://github.com/mblondel/svmlight-loader/blob/master/_svmlight_loader.cpp
 *
 * License: Simple BSD
 */

#include <Python.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

extern "C" {
  static PyObject *sparse_matr_to_text(PyObject *self, PyObject *args)
  {
    try {
      // Read function arguments.
      char const *file_path;
      PyArrayObject *indices_array, *indptr_array, *data_array, *label_array;

      if (!PyArg_ParseTuple(args,
                            "sO!O!O!",
                            &file_path,
                            &PyArray_Type, &data_array,
                            &PyArray_Type, &indices_array,
                            &PyArray_Type, &indptr_array))
        return 0;

      int n_samples = indptr_array->dimensions[0] - 1;
      double *data = (double*) data_array->data;
      int *indices = (int*) indices_array->data;
      int *indptr = (int*) indptr_array->data;
      double *y = (double*) label_array->data;

      std::ofstream fout;
      fout.open(file_path, std::ofstream::out);

      int idx;
      for (int i=0; i < n_samples; i++) {
        fout << y[i] << " ";
        for (int jj=indptr[i]; jj < indptr[i+1]; jj++) {
          idx = indices[jj];
          fout << idx << ":" << data[jj] << " ";
        }
        fout << std::endl;
      }

      fout.close();

      Py_INCREF(Py_None);
      return Py_None;

    } catch (std::exception const &e) {
      std::string msg("error in SparsePy writer: ");
      msg += e.what();
      PyErr_SetString(PyExc_RuntimeError, msg.c_str());
      return 0;
    }
  }
}

/*
 * Python module setup.
 */

static PyMethodDef sparse_methods[] = {
  {"sparse_matr_to_text", sparse_matr_to_text, METH_VARARGS, NULL},

  {NULL, NULL, 0, NULL}
};

extern "C" {
PyMODINIT_FUNC init_sparse_py(void)
{
  Py_InitModule3("sparse_py", sparse_methods, NULL);
       
}
