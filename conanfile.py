from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conans.tools import Git


class XVIZ(ConanFile):
    name = "xviz"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
        "coverage": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": True,
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
        "docs/*",
    )

    def _configure_cmake(self) -> CMake:
        cmake = CMake(self)
        variables = {"XVIZ_VERSION": f"{self.version}"}
        if self.options.build_tests:
            variables["XVIZ_BUILD_TESTS"] = "ON"
        if self.options.coverage:
            variables["XVIZ_BUILD_COVERAGE"] = "ON"
        cmake.configure(variables=variables)
        return cmake

    def set_version(self):
        try:
            self.version = (
                Git(folder=self.recipe_folder).run("describe --tags --abbr=0") or "1.0.0"
            )
        except:
            self.version = "1.0.0"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
            self.options.coverage = False
        if not self.options.build_tests:
            self.options.coverage = False
        if self.settings.build_type != "Debug":
            self.options.coverage = False
        self.options["websocketpp"].asio = "standalone"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("protobuf/3.21.9")
        self.requires("fmt/9.1.0")
        self.requires("websocketpp/0.8.2")
        if self.options.build_tests:
            self.requires("gtest/cci.20210126")

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