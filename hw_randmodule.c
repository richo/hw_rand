#include <Python.h>

static PyObject *HwRand_RandUnavailable;

int _rdrand64_step(unsigned long long int *therand)
{
    unsigned long long int foo;
    int cf_error_status;
    asm("\n\
            rdrand %%rax;\n\
            mov $1,%%edx;\n\
            cmovae %%rax,%%rdx;\n\
            mov %%edx,%1;\n\
            mov %%rax, %0;":"=r"(foo),"=r"(cf_error_status)::"%rax","%rdx");
        *therand = foo;
    return cf_error_status;
}

/* Returns a 64 bit random integer by default.
 * Extending to allow selection of integer length and support for more then
 * 64bit linux is on the TODO list
 */

/* Returns either an _int_ or raises hw_rand.RandUnavailable
 */
static PyObject *
hw_rand_rand(PyObject *self, PyObject *args)
{
    unsigned long long int rand;
    if (_rdrand64_step(&rand) == 0)
    {
        PyErr_SetString(HwRand_RandUnavailable, "Couldn't obtain random int");
        return NULL;
    }
    else
        return Py_BuildValue("i", rand);
}

static PyMethodDef HwRandMethods[] = {
    {"rand",  hw_rand_rand, METH_VARARGS,
        "Return a 64bit wide random integer"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
inithw_rand(void)
{
    PyObject *m;

    m = Py_InitModule("hw_rand", HwRandMethods);
    if (m == NULL)
        return;

    HwRand_RandUnavailable = PyErr_NewException("hw_rand.RandUnavailable", NULL, NULL);
    Py_INCREF(HwRand_RandUnavailable);
    PyModule_AddObject(m, "RandUnavailable", HwRand_RandUnavailable);
}
