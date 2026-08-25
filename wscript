# =========================================================================
#     CException - Simple Exception Handling in C
#     ThrowTheSwitch.org
#     Copyright (c) 2007-24 Mark VanderVoord
#     SPDX-License-Identifier: MIT
# =========================================================================

"""Test CException for different compilers and platforms.

Supported variants are:
- Linux: GCC, Clang
- Windows:. GCC, MSVC
"""

from waflib import Context, Errors, Logs, Utils
from waflib.Tools import waf_unit_test

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from waflib.Build import BuildContext
    from waflib.Configure import ConfigurationContext
    from waflib.Options import OptionsContext


def options(opt: OptionsContext):
    """Provide basic C compiler and testing options"""
    # we can select the C compiler we want to test with using the
    # --check-c-compiler option
    opt.load("compiler_c")
    opt.load("waf_unit_test")


def configure(cnf: ConfigurationContext):
    """Configure the C compiler and environment for the host platform"""
    cnf.load("compiler_c")
    cnf.load("waf_unit_test")

    # this setup is for running bare tests on CException
    # and not the integration tests that require Ceedling/Unity
    # and friends
    cnf.env.append_unique("DEFINES", ["BARE_TEST"])

    if cnf.env.CC_NAME == "gcc":
        # when using GCC we can build using coverage
        cnf.env.append_unique("CFLAGS", ["--coverage"])
        # we need the coverage library during linking
        cnf.check_cc(lib="gcov", uselib_store="GCOV")
        cnf.env.append_unique("LINKFLAGS", ["-coverage"])

    if cnf.env.CC_NAME == "clang":
        # when using GCC we can build using coverage
        cnf.env.append_unique(
            "CFLAGS", ["-fprofile-instr-generate", "-fcoverage-mapping"]
        )
        cnf.env.append_unique(
            "LINKFLAGS", ["-fprofile-instr-generate", "-fcoverage-mapping"]
        )

    if cnf.env.CC_NAME in ("clang", "gcc"):
        cnf.env.append_unique(
            "CFLAGS",
            [
                "-Wall",
                "-Wextra",
                "-Werror",
                "-Wunused-function",  # we need this so we can ensure all test functions are called!
            ],
        )

        # We need Python and gcovr to create a coverage report
        # from the coverage data
        cnf.find_program("python", var="PYTHON", mandatory=False)
        cnf.find_program("gcovr", mandatory=False)
        if cnf.env.GCOVR:
            cnf.env.gcovr_module = "gcovr"


def build(bld: BuildContext):
    """Build the library and tests and then run the tests."""
    # build CException using the custom configuration file as this is
    # how later want to test it, so we need to define
    # CEXCEPTION_USE_CONFIG_FILE
    # We also need to define TEST, as otherwise extended tests would run
    # that are not meaningful when multithreading is NOT enabled
    bld(
        features="c",
        source="lib/CException.c",
        target="obj-cexception",
        defines=["TEST", "CEXCEPTION_USE_CONFIG_FILE"],
        includes="../test/support",
    )
    # archive the CException object into a library
    bld(features="c cstlib", use="obj-cexception", target="cexception")

    # build, link and run the test binary
    bld(
        features="c cprogram test",
        source="test/TestException.c",
        target="TestException",
        use="cexception GCOV",
        includes="lib test/support",
        defines=["TEST", "CEXCEPTION_USE_CONFIG_FILE"],
    )

    # postprocess unit testing results
    bld.add_post_fun(std)  # print entire stdout/stderr
    bld.add_post_fun(gcovr)  # create the coverage report
    bld.add_post_fun(waf_unit_test.summary)
    bld.add_post_fun(waf_unit_test.set_exit_code)


def gcovr(bld: BuildContext):
    """Create the coverage report."""
    # gcovr is only loaded for gcc and clang as reporting does not work for
    # MSVC
    if not bld.env.GCOVR or not bld.env.PYTHON:
        Logs.warn("Cannot generate coverage report.")
        return
    gcovr_options = []
    if bld.env.CC_NAME == "clang":
        gcovr_options = [
            "--gcov-executable=llvm-cov",
            "--llvm-cov-binary=llvm-cov",
            "--llvm-profdata-executable=llvm-profdata",
        ]
    python = Utils.subst_vars("${PYTHON}", bld.env)
    gcovr_module = Utils.subst_vars("${gcovr_module}", bld.env)
    root = bld.srcnode.abspath()
    cwd = bld.bldnode
    cmd = (
        [
            python,
            "-m",
            gcovr_module,
        ]
        + gcovr_options
        + [
            "-r",
            root,
            "--html-details",
            "-o",
            "index.html",
            "lib",
        ]
    )

    try:
        bld.cmd_and_log(cmd, cwd=cwd, quiet=True, output=Context.BOTH)
    except Errors.WafError as e:
        if e.stdout:
            Logs.pprint("NORMAL", e.stdout)
        if e.stderr:
            Logs.error(e.stderr)
        bld.fatal("gcovr error.")
    report_file = bld.bldnode.find_node("index.html")
    Logs.pprint("NORMAL", f"\nReport: {report_file}")


def std(bld: BuildContext):
    """Print stdout and stderr output to the terminal after running tests."""
    lst = getattr(bld, "utest_results", [])
    if not lst:
        return

    nr_tests = len(lst)
    nr_failed_tests = len([x for x in lst if x[1]])

    val = 100 * (nr_tests - nr_failed_tests) / (1.0 * nr_tests)
    Logs.pprint("CYAN", f"test report {val:3.0f}% success")

    Logs.pprint("CYAN", f"  tests that fail  {nr_failed_tests}/{nr_tests}")
    for result in lst:
        Logs.pprint("CYAN", f"    {result.test_path} ({result.generator.name})")
        color = "RED" if result.exit_code else "GREEN"
        Logs.pprint(color, f"status: {result.exit_code}")
        if result.out:
            Logs.pprint("NORMAL", f"out:\n{result.out.decode('utf-8')}")
        if result.err:
            Logs.pprint("RED", f"err:\n{result.err.decode('utf-8')}")
