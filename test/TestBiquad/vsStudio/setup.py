from setuptools import setup, Extension
import pybind11

cpp_args = ['-std=c++14', '-stdlib=libc++']

sfc_module = Extension(
    'biquadWrapper',
    sources=['./biquadWrapper.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
    extra_compile_args=cpp_args,
)

setup(
    name='biquadWrapper',
    version='1.0',
    description='Python package with biquadWrapper C++ extension (PyBind11)',
    ext_modules=[sfc_module],
)