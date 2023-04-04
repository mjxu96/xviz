from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conans.tools import Git
import os


class XVIZ(ConanFile):
    name = "xviz"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
        "build_examples": [True, False],
        "coverage": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": False,
        "build_examples": False,
        "coverage": False,
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = (
        "src/*",
        "include/*",
        "proto/*",
        "cmake/*",
        "CMakeLists.txt",
        "README.md",
        "LICENSE.md",
        "tests/*",
        "examples/*"
    )

    def _configure_cmake(self) -> CMake:
        cmake = CMake(self)
        variables = {"XVIZ_VERSION": f"{self.version}"}
        if self.options.build_tests:
            variables["XVIZ_BUILD_TESTS"] = "ON"
        if self.options.build_examples:
            variables["XVIZ_BUILD_EXAMPLES"] = "ON"
        if self.options.coverage:
            variables["XVIZ_TEST_COVERAGE"] = "ON"
        cmake.configure(variables=variables)
        return cmake

    def set_version(self):
        try:
            self.version = (
                Git(folder=self.recipe_folder).run("describe --tags --abbr=0") or "0.0.1"
            )
        except:
            # this version is for local recipe consumer test
            self.version = "0.0.1"

    def requirements(self):
        self.requires("protobuf/3.21.9")
        self.requires("fmt/9.1.0")
        if self.options.build_tests:
            self.requires("gtest/cci.20210126")
        if self.options.build_examples:
            self.requires("websocketpp/0.8.2")
            self.requires("lodepng/cci.20200615")
            self.options["websocketpp"].asio = "standalone"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
            self.options.coverage = False
        if not self.options.build_tests:
            self.options.coverage = False
        if self.settings.build_type != "Debug":
            self.options.coverage = False

    def layout(self):
        cmake_layout(self)

    def generate(self):
        dp = CMakeDeps(self)
        dp.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = self._configure_cmake()
        if self.should_build or self.should_test:
            cmake.build()
        if self.options.build_tests and self.should_test:
            cmake.test(cli_args=["ARGS=-V"])

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["xviz"]
        # We will use our installed cmake files
        self.cpp_info.set_property("cmake_find_mode", "none")
        self.cpp_info.builddirs.append(os.path.join("lib", "cmake", "xviz"))
