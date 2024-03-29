from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
import os


class XVIZConsumerExample(ConanFile):
    name = "xviz_consumer_example"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    def _configure_cmake(self) -> CMake:
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def requirements(self):
        # read version from CI env
        xviz_version = os.environ["XVIZ_CI_VERSION_OVERRIDE"]
        self.requires(f"xviz/{xviz_version}@local/test")
        self.requires("websocketpp/0.8.2")
        self.requires("lodepng/cci.20200615")
        self.options["websocketpp"].asio = "standalone"
        # we don't need openssl for example
        self.options["websocketpp"].with_openssl = False

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

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
