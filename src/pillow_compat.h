/*
 * The Python Imaging Library.
 *
 * C API compatibility shims on top of pythoncapi_compat.h.
 */

#ifndef PILLOW_COMPAT_H
#define PILLOW_COMPAT_H

#include "thirdparty/pythoncapi_compat.h"

#ifndef Py_BEGIN_CRITICAL_SECTION
/* Py_LIMITED_API builds always have the GIL. Python 3.15 is the first to
   expose critical sections in the Limited API, so provide no-ops before it */
#define Py_BEGIN_CRITICAL_SECTION(op) {
#define Py_END_CRITICAL_SECTION() }
#define Py_BEGIN_CRITICAL_SECTION2(a, b) {
#define Py_END_CRITICAL_SECTION2() }
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
    /* Exact type checks are inline pointer compares even under the Limited
       API, whereas PyList_Check() is a PyType_GetFlags() call there */
    if (PyTuple_CheckExact(seq)) {
        return PyTuple_GetItem(seq, i);
    }
    if (PyList_CheckExact(seq) || PyList_Check(seq)) {
        return PyList_GetItem(seq, i);
    }
    return PyTuple_GetItem(seq, i);
}

#endif /* PILLOW_COMPAT_H */
