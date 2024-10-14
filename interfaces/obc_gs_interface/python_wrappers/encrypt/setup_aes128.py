from distutils.core import Extension, setup

module = Extension(
    "_obc_gs_aes128",
    sources=[
        "obc_gs_aes128_wrap.c",
        "../../aes128/obc_gs_aes128.c",
    ],
    include_dirs=[
        "../../aes128",  # AES128 implementation headers
        "../../common",  # includes "obc_gs_errors.h"
        "../../fec",  # defines RS_DECODED_SIZE
        "../../../../libs/tiny_aes/include/",  # includes aes.h
        "../../../../libs/libcorrect/include",  # includes correct.h
    ],
    extra_objects=["../../libs/tiny_aes/source/aes.o"],
    define_macros=[],
    undef_macros=[],
    library_dirs=[],
    libraries=[],
)

setup(name="obc_gs_aes128", version="1.0", ext_modules=[module])
