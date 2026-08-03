/*
 * The Python Imaging Library.
 *
 * C API compatibility shims.
 *
 * pythoncapi_compat.h requires the full C API, so when building with the
 * Limited API, provide the few helpers Pillow needs directly instead.
 */

#ifndef PILLOW_COMPAT_H
#define PILLOW_COMPAT_H

#ifndef Py_LIMITED_API

#include "thirdparty/pythoncapi_compat.h"

#elif Py_LIMITED_API < 0x030D0000

/* gh-114329 added PyList_GetItemRef() to Python 3.13 */
static inline PyObject *
PyList_GetItemRef(PyObject *op, Py_ssize_t index) {
    PyObject *item = PyList_GetItem(op, index);
    Py_XINCREF(item);
    return item;
}

/* gh-106521 added PyDict_GetItemRef() to Python 3.13 */
static inline int
PyDict_GetItemRef(PyObject *mp, PyObject *key, PyObject **result) {
    PyObject *item = PyDict_GetItemWithError(mp, key);
    if (item != NULL) {
        *result = Py_NewRef(item);
        return 1;
    }
    *result = NULL;
    return PyErr_Occurred() ? -1 : 0;
}

#endif /* Py_LIMITED_API */

#ifndef Py_BEGIN_CRITICAL_SECTION
/* Py_LIMITED_API builds always have the GIL */
#define Py_BEGIN_CRITICAL_SECTION(op) {
#define Py_END_CRITICAL_SECTION() }
#endif

/* Create a heap type from a spec once, caching it in *type */
static inline int
pil_create_type(PyTypeObject **type, PyType_Spec *spec) {
    if (*type == NULL) {
        *type = (PyTypeObject *)PyType_FromSpec(spec);
    }
    return *type == NULL ? -1 : 0;
}

/* Free an instance of a heap type and release its reference to the type */
static inline void
pil_object_free(void *op) {
    PyTypeObject *type = Py_TYPE((PyObject *)op);
    ((freefunc)PyType_GetSlot(type, Py_tp_free))(op);
    Py_DECREF(type);
}

/* Borrowed-reference item access for list-or-tuple objects, e.g. those
   returned by PySequence_Fast */
static inline PyObject *
pil_fast_getitem(PyObject *seq, Py_ssize_t i) {
    if (PyList_Check(seq)) {
        return PyList_GetItem(seq, i);
    }
    return PyTuple_GetItem(seq, i);
}

#endif /* PILLOW_COMPAT_H */
