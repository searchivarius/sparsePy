/*
 * Authors: Leonid Boytsov <leo@boytsov.info>
 *          heavily relying on the code of 
 *          Mathieu Blondel <mathieu@mblondel.org>
 *       
 *          https://github.com/mblondel/svmlight-loader
 *
 * License: Simple BSD
 */

#include <Python.h>
#include <numpy/arrayobject.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

extern "C" {
  static PyObject *_sparse_matr_to_text(PyObject *self, PyObject *args) {
    try {
      // Read function arguments.
      char const *file_path;
      PyArrayObject *indices_array = nullptr, 
                    *indptr_array = nullptr, 
                    *data_array = nullptr; 
      int append_flag;

      if (!PyArg_ParseTuple(args,
                            "sO!O!O!p",
                            &file_path,
                            &PyArray_Type, &data_array,
                            &PyArray_Type, &indices_array,
                            &PyArray_Type, &indptr_array,
                            &append_flag))
        return 0;

      int n_samples = indptr_array->dimensions[0] - 1;
      double *data = (double*) data_array->data;
      int *indices = (int*) indices_array->data;
      int *indptr = (int*) indptr_array->data;

      std::ofstream fout;
      if (append_flag)
      	fout.open(file_path, std::ofstream::out | std::ofstream::app);
      else
      	fout.open(file_path, std::ofstream::out);

      int idx;
      for (int i=0; i < n_samples; i++) {
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

static PyMethodDef module_methods[] = {
  {"_sparse_matr_to_text", _sparse_matr_to_text, METH_VARARGS, nullptr},

  {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef sparse_text_util = {
    PyModuleDef_HEAD_INIT,
    "_sparse_text_util", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    module_methods,
    nullptr
};

extern "C" {
 PyMODINIT_FUNC PyInit__sparse_text_util(void) {
    _import_array();

    return PyModule_Create(&sparse_text_util);
  }
}
