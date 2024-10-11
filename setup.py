from setuptools import Extension, setup

module = Extension("symnmf", sources=["symnmfmodule.c", "symnmf.c", "matrix.c"])

setup(
    name="symnmf",
    version="1.0",
    description="Integrate SymNMF between Python and C",
    ext_modules=[module],
)
