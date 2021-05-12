##
## EPITECH PROJECT, 2020
## OOP_babel_2019
## File description:
## conanfile
##

from conans import ConanFile, CMake

class Conan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = "portaudio/v190600.20161030@bincrafters/stable", \
              "qt/5.14.2@bincrafters/stable",\
              "bzip2/1.0.6@conan/stable",\
              "opus/1.3.1@bincrafters/stable",\
              "zlib/1.2.11@conan/stable",\
              "boost/1.71.0@conan/stable",\
              "sqlite3/3.21.0@bincrafters/stable"

   generators = "cmake", "cmake_find_package", "cmake_paths"
   def configure(self):
       self.options["Qt"].shared = True
       if self.settings.os == "Windows":
           self.options["portaudio"].shared = True
       if "shared" in self.options:
           pass
       
   def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin

   def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()